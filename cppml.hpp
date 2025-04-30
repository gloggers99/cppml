#pragma once

#include <string>
#include <variant>
#include <vector>
#include <sstream>

template <typename ...children_types>
using children = std::vector<std::variant<std::nullptr_t, children_types...>>;


template <typename T>
concept is_string_convertable = requires (T t) {
    std::string(t);
};

/**
 * Virtual base class for all elements.
 * @tparam children_types Each input type must be convertable to a string, this allows for lots of flexibility.
 */
template <typename ...children_types>
    requires ((is_string_convertable<children_types>) && ...)
class element {
    children<children_types...> children_nodes;
public:
    explicit element(const children_types ...children_nodes)
        : children_nodes { std::variant<std::nullptr_t, children_types...>(children_nodes)... } {}
    virtual ~element() = default;

    virtual explicit operator std::string() {
        std::stringstream ss;
        ss << "<" << typeid(*this).name() << ">";

        return ss.str();
    };
};

/**
 * This namespace contains all pre-defined elements.
 */
namespace tags {
    template <typename ...children_types>
    class html final : public element<children_types...> {
    public:
        explicit html(const children_types ...children_nodes)
            : element<children_types...>(children_nodes...){}
    };

    template <typename ...children_types>
    class p final : public element<children_types...> {
    public:
        explicit p(const children_types ...children_nodes)
            : element<children_types...>(children_nodes...) {}

        explicit operator std::string() override {
            return "";
        }
    };
}
