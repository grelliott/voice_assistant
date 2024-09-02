cmake_minimum_required(VERSION 3.16)

project(voice_assistant
    VERSION 0.0.1
    DESCRIPTION "Voice assistant application"
)

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(VERSION_TEMPLATE ${PROJECT_SOURCE_DIR}/include/version.hpp.in)
set(VERSION_FILE ${PROJECT_SOURCE_DIR}/include/version.hpp)
include(Version)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(argparse REQUIRED)

add_executable( ${CMAKE_PROJECT_NAME}
    src/log.cpp
    src/application.cpp
    src/main.cpp)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE FMT_HEADER_ONLY)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)
target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        ${fmt_INCLUDE_DIRS}
        ${spdlog_INCLUDE_DIRS}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    spdlog::spdlog
    argparse::argparse
)

enable_testing()
add_subdirectory(test)
