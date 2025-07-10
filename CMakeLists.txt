cmake_minimum_required(VERSION 3.31)
project(cppml)

set(CMAKE_CXX_STANDARD 26)

add_executable(cppml
        include/cppml.hpp
        src/cppml.cpp)

target_include_directories(cppml PUBLIC ${PROJECT_SOURCE_DIR}/include)
