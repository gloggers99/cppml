#pragma once

#include <string>
#include <variant>
#include <vector>

template <typename ...children_types>
using children = std::vector<std::variant<std::nullptr_t, children_types...>>;

template <typename T>
concept is_string_convertable = requires (T t) {}

template <typename ...children_types>
    requires (() && ...)
class element {
    children<children_types...> children_nodes;
public:
    explicit element(const children<children_types...> &children_nodes = {})
        : children_nodes(children_nodes) {}
    virtual ~element() = default;

    virtual explicit operator std::string() = 0;
};

template <typename ...children_types>
class html final : public element<children_types...> {
public:
    explicit html(const children<children_types...> &children_nodes = {})
        : element<children_types...>(children_nodes) {}

    explicit operator std::string() override {
        return "";
    };
};