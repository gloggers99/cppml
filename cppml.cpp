#include "cppml.hpp"

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
