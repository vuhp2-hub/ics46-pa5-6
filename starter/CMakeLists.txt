find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(name_hash_gtest tests/name_hash_gtest.cpp)
target_include_directories(name_hash_gtest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(name_hash_gtest PRIVATE GTest::gtest_main)
gtest_discover_tests(name_hash_gtest)

add_executable(part2_gtest tests/part2_gtest.cpp)
target_include_directories(part2_gtest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(part2_gtest PRIVATE GTest::gtest_main)
gtest_discover_tests(part2_gtest)
target_compile_definitions(part2_gtest PRIVATE PROJECT_ROOT_DIR=${CMAKE_SOURCE_DIR} NOT_GRADESCOPE="yes")

add_executable(driver driver.cpp)
install(TARGETS driver RUNTIME DESTINATION bin)

# Address Sanitizers
set(SANITIZER_FLAGS "-fsanitize=address,undefined")
set(SANITIZED_TARGETS name_hash_gtest driver)

foreach(target IN LISTS SANITIZED_TARGETS)
  target_compile_options(${target} PRIVATE ${SANITIZER_FLAGS})
  target_link_options(${target} PRIVATE ${SANITIZER_FLAGS})
endforeach()
