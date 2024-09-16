#!/bin/bash

SCRIPT_LOCATION="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
OUTPUT_DIR="$SCRIPT_LOCATION/build/"
BUILD_MODE="Release"
BUILD_CONAN="True"
PRESET_FILE="$SCRIPT_LOCATION/CMakeUserPresets.json"

if [ ! -d "$OUTPUT_DIR" ]; then
    mkdir -p "$OUTPUT_DIR"
fi
if [ "$#" -eq 1 ] && [ "$1" = "-d" ]; then
    echo "-- DEBUG MODE"
    BUILD_MODE="Debug"
fi

if [ "$#" -eq 1 ] && [ "$1" = "--build-conan" ]; then
    echo "-- BUILDING CONAN DEPENDENCIES"
    BUILD_CONAN="True"
else
    echo "-- SKIPPING CONAN BUILD"
    BUILD_CONAN="False"
fi

if [ -f "$PRESET_FILE" ]; then
    echo "-- USING PRESET FILE"
    if [ "$BUILD_CONAN" = "True" ]; then
        echo "-- RUNNING CONAN INSTALL"
        conan install . --output-folder=build --build=missing -c tools.system.package_manager:sudo=True
    fi
    cmake --preset conan-release -DCMAKE_BUILD_TYPE="$BUILD_MODE" -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="$SCRIPT_LOCATION" -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="$SCRIPT_LOCATION"
    cmake --build "$OUTPUT_DIR" --preset conan-release
else
    echo "-- RUNNING CONAN INSTALL"
    conan install . --output-folder=build --build=missing -c tools.system.package_manager:sudo=True
    cmake --preset conan-release -DCMAKE_BUILD_TYPE="$BUILD_MODE" -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="$SCRIPT_LOCATION" -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="$SCRIPT_LOCATION"
    cmake --build "$OUTPUT_DIR" --preset conan-release
fi
