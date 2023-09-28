add_definitions(-DSIL_MEM_TEST)

set(TNAME test_lib)

add_executable(${TNAME} main.cpp)
target_include_directories(${TNAME} PUBLIC "..")
target_link_libraries(${TNAME} PRIVATE GTest::GTest ${PROJECT_NAME})

include(GoogleTest)
gtest_discover_tests(${TNAME})
