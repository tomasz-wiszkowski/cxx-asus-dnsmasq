#!/usr/bin/ccmake .
cmake_minimum_required (VERSION 3.5)

project(dnsmsasq-surrogate
  VERSION 1.20.0
  LANGUAGES C CXX
)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/CMakeModules
)

set(BUILD_DIST false CACHE BOOL
    "Whether to build package for Asus target. When false, will build tests for host.")
set(CXX_FLAGS "--std=c++17" CACHE STRING
    "Compile flags for Distribution and Tests.")

if (BUILD_DIST)
    include(SurrogateDist)
else()
    include(SurrogateTest)
endif()

