#!/bin/bash

SCRIPT_LOCATION="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
OUTPUT_DIR="$SCRIPT_LOCATION/build/"
BUILD_MODE="Release"
CMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

if [ ! -d "$OUTPUT_DIR" ]; then
    mkdir -p "$OUTPUT_DIR"
fi

if [ "$#" -eq 1 ] && [ "$1" = "-d" ]; then
    echo "-- DEBUG MODE"
    BUILD_MODE="Debug"
fi

cmake -S "$SCRIPT_LOCATION" -B "$OUTPUT_DIR" -DCMAKE_BUILD_TYPE="$BUILD_MODE" -DCMAKE_TOOLCHAIN_FILE="$CMAKE_TOOLCHAIN_FILE" -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="$SCRIPT_LOCATION" -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="$SCRIPT_LOCATION"
cmake --build "$OUTPUT_DIR" -- -j$(nproc)
