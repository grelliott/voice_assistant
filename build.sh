#!/bin/bash

#  conan profile detect --force
conan install -s build_type=Release . --output-folder=conan --build=missing

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=conan/build/Release/generators/conan_toolchain.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

run-clang-tidy -p ./build

cmake --build build
