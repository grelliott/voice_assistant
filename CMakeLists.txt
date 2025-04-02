set(CMAKE_POLICY_DEFAULT_CMP0025 new)
cmake_minimum_required(VERSION 3.22)

project(voice_assistant
    VERSION 0.0.2
    DESCRIPTION "Voice assistant application"
)

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(VERSION_TEMPLATE ${PROJECT_SOURCE_DIR}/include/version.hpp.in)
set(VERSION_FILE ${PROJECT_SOURCE_DIR}/include/version.hpp)
include(Version)

find_package(spdlog REQUIRED)
find_package(argparse REQUIRED)
find_package(SDL2 REQUIRED)

add_executable( ${CMAKE_PROJECT_NAME}
    src/log.cpp
    src/audio/capture/sdl2_capture.cpp
    src/application.cpp
    src/main.cpp)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE SPDLOG_USE_STD_FORMAT)
target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE
        include
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    argparse::argparse
    spdlog::spdlog_header_only
    SDL2::SDL2
)

install( TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
