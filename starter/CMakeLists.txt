find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(name_hash_gtest tests/name_hash_gtest.cpp)
target_link_libraries(name_hash_gtest GTest::gtest_main)
gtest_discover_tests(name_hash_gtest)

add_executable(driver driver.cpp)
install(TARGETS driver RUNTIME DESTINATION bin)
