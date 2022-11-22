cmake_minimum_required(VERSION 3.23)

project(set)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-fsanitize=address")

add_subdirectory(google-test)

add_executable(play
        main.cpp
        Set.hpp)

add_executable(tests
        Test.cpp
        Set.hpp)
target_link_libraries(tests gtest gtest_main)