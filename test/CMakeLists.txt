cmake_minimum_required(VERSION 3.16)

project(miniUtilTest)

add_executable(baseTest baseTest.cpp)
target_link_libraries(baseTest 
    PRIVATE
    gtest
    gtest_main
    miniUtil
)

add_test(NAME baseTest COMMAND baseTest)