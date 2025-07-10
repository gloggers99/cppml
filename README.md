<div align="center">

# CPPML

![C++](https://img.shields.io/badge/c++-%2300599C.svg?logo=c%2B%2B&logoColor=white)

</div>

# Explaination

CPPML is a C++ library for writing HTML and CSS within the C++ language.

The main purpose of this project is to demonstrate the capabilities of metaprogramming
in C++ and to provide a way to write HTML and CSS in a more structured and type-safe manner.

# Requirements

- GCC with C++26

# Example

The following output of this C++ is a proper renderable HTML document:

```c++
#include <cppml.hpp>

#include <iostream>

int main() {
    using namespace tags;

    html test_html {
        h1 { std::string("Test!!!") },
        p {
            std::string("hello")
        },
    };

    std::cout << std::string(test_html) << "\n";

    return 0;
}
```