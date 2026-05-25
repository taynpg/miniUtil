cmake_minimum_required(VERSION 3.16)

project(miniUtil VERSION 0.3.1 LANGUAGES CXX)

if (MSVC)
    add_compile_options(/utf-8)
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_library(miniUtil STATIC miniUtil.h miniUtil.cpp)
target_include_directories(miniUtil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(third_party/googletest)

add_subdirectory(test)

enable_testing()