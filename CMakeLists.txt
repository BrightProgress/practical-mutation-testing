cmake_minimum_required(VERSION 3.16)

project(Dictionary 
    VERSION 0.1.0
    LANGUAGES CXX)

enable_testing()
add_subdirectory(dbc)
add_subdirectory(Dictionary)
