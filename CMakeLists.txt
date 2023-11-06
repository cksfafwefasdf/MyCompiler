cmake_minimum_required(VERSION 3.26)
project(MyCompailer)

set(CMAKE_CXX_STANDARD 11)

add_executable(MyCompailer main.cpp
        "lexical analysis/keyword.h"
        "lexical analysis/analysis.h"
        "lexical analysis/analysis.cpp"
        SLR1/SLR1Analyzer.cpp
        SLR1/SLR1Analyzer.h
        SLR1/SynParser.cpp
        SLR1/SynParser.h)
