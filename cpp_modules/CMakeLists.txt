cmake_minimum_required(VERSION 3.28)
project(financial_ML_CPP)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/utils)

add_executable(financial_ML_CPP
        src/data/IVE_tickbidask_sample.txt
        src/utils/DataStructures.cpp
        src/utils/DataStructures.h
        src/utils/HelpingCalculations.cpp
        src/utils/HelpingCalculations.h
        main.cpp
        ../README.md)
