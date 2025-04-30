#include "cppml.hpp"

#include <iostream>

int main() {
    std::string test = "a";

    using namespace tags;

    html test_html {
        p { "hello" }
    };

    std::cout << std::string(test_html) << "\n";

    return 0;
}
