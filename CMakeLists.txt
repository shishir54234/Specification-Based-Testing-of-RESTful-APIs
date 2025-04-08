cmake_minimum_required(VERSION 3.10)
project(ASTTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(ast_tests
    ast.hpp
    PrintVisitor.hpp
    ASTVis.hpp
    atc_ast.hpp
    ast_tests.cpp
)

# Enable warnings
if(MSVC)
    target_compile_options(ast_tests PRIVATE /W4)
else()
    target_compile_options(ast_tests PRIVATE -Wall -Wextra)
endif()