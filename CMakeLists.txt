cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 11)

project(nestegg CXX)

include_directories("${PROJECT_SOURCE_DIR}")
enable_testing()
add_subdirectory("tests")
add_library(nestegg Instructions.cpp ComputerState.cpp)
target_link_libraries(nestegg stdc++)