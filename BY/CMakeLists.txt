cmake_minimum_required(VERSION 3.26)
project(BY C)

set(CMAKE_C_STANDARD 11)

add_executable(BY main.c
        Lexical.h
        Lexical.c
        GramAnalysis.c
        GramAnalysis.h
        Recursive.c
        Recursive.h
        SuanFu.c
        SuanFu.h)
