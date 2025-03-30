project(test_app_test)

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(${PROJECT_NAME}
    unit/test.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${GTest_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    GTest::GTest
    GTest::Main
)

install( TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

gtest_discover_tests(${PROJECT_NAME})
