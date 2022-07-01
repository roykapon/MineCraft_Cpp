cmake_minimum_required(VERSION 3.0.0)
project(MineCraft_cpp VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(MineCraft_cpp MineCraft_cpp.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
