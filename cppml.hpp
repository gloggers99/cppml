#pragma once

#include <string>
#include <variant>
#include <vector>
#include <sstream>
#include <memory>
#include <cxxabi.h>

/**
 * We define children as a shortcut because the type is very long. We have to use
 * std::nullptr_t as a placeholder because std::variant is not constructable unless
 * at least one type is provided, we can use anything as the placeholder.
 */
template <typename ...children_types>
using children = std::vector<std::variant<std::nullptr_t, children_types...>>;

/**
 * Constraint to ensure a type is convertable to a string
 */
template <typename T>
concept is_string_convertable = requires (T t) {
    std::string(t);
};

constexpr std::string demangle(const char *input_name) {
    const std::unique_ptr<char, decltype(&std::free)> demangled_name_ptr {
        abi::__cxa_demangle(input_name, nullptr, nullptr, nullptr),
        std::free
    };

    /*
     * If the demangled name returns null we will fall back to
     * the input_name parameter
     */
    if (!demangled_name_ptr)
        return input_name;

    std::string demangled_name = demangled_name_ptr.get();

    /*
     * Remove all template parameter information
     */
    std::size_t position = demangled_name.find('<');
    if (position != std::string::npos)
        demangled_name = demangled_name.substr(0, position);

    /*
     * Remove EVERYTHING (namespaces, etc.) before the actual class name
     */
    position = demangled_name.rfind("::");
    if (position != std::string::npos)
        demangled_name = demangled_name.substr(position + 2);

    return demangled_name;
}

/**
 * Virtual base class for all elements.
 * @tparam derived_type The type of the class inheriting ::element, this is used to determine the name of the element.
 * @tparam children_types Each input type must be convertable to a string, this allows for lots of flexibility.
 */
template <typename derived_type, typename ...children_types>
    requires ((is_string_convertable<children_types>) && ...)
class element {
protected:
    children<children_types...> children_nodes;
    std::string element_name;

public:
    /*
     * We mark this as constexpr to allow for even further optimization if all arguments passed
     * are available in a constexpr environment. Essentially compiling the results ahead of time.
     */
    constexpr explicit element(const children_types ...children_nodes)
        : children_nodes { std::variant<std::nullptr_t, children_types...>(children_nodes)... },
          element_name(demangle(typeid(derived_type).name())) {}
    virtual ~element() = default;

    virtual explicit operator std::string() const {
        std::stringstream ss;
        ss << "<" << this->element_name << ">\n";

        for (const auto &child : this->children_nodes) {
            std::visit([&]<typename T>(const T &visited_child) {
                /*
                 * Because we use std::nullptr_t as a placeholder type in ::children
                 * we need to also account for it here.
                 */
                if constexpr (!std::is_same_v<T, std::nullptr_t>)
                    ss << std::string(visited_child) << "\n";

            }, child);
        }

        ss << "</" << this->element_name << ">";

        return ss.str();
    };
};

/**
 * A shortcut to create basic elements.
 * @param __NAME The element and class name.
 */
#define $basic_element(__NAME) \
    template <typename ...children_types> \
    class __NAME final : public element<__NAME<children_types...>, children_types...> { \
    public: \
        constexpr explicit __NAME(children_types &&...children_nodes) \
            : element<__NAME, children_types...>(std::forward<children_types>(children_nodes)...) {} \
    }

/**
 * This namespace contains all pre-defined elements.
 */
namespace tags {
    /**
     * Essentially everything that inherits ::element is a proxy type.
     * Everything in html is node based so every element is "technically"
     * the same, just the name is different and sometimes there are some unique cases.
     * @tparam children_types
     */
    $basic_element(html);

    $basic_element(p);

    $basic_element(h1);
    $basic_element(h2);
    $basic_element(h3);
    $basic_element(h4);
    $basic_element(h5);
}
