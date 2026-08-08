cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(pa5-6 CXX)
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(hello_test hello_test.cpp)
target_link_libraries(hello_test GTest::gtest_main)
gtest_discover_tests(hello_test)

add_executable(driver driver.cpp)
install(TARGETS driver RUNTIME DESTINATION bin)
