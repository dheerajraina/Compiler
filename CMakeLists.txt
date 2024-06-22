cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

project(DCompiler C)

include_directories(include)

add_executable(
        dcom
        src/main.c
        src/lexer.c
        src/parser.c
        src/semantic.c
        src/codegen.c
        src/runner.c
)