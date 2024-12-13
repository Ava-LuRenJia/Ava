cmake_minimum_required(VERSION 3.26)
project(BY_3 C)

set(CMAKE_C_STANDARD 11)

add_executable(BY_3 main.c
        GramAnalysis.c
        GramAnalysis.h
        Lexical.c
        Lexical.h
        Recursive.c
        Recursive.h
)
