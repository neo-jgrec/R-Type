@echo off
setlocal

rem Get the script location
set "SCRIPT_LOCATION=%~dp0"
set "OUTPUT_DIR=%SCRIPT_LOCATION%build"
set "BUILD_MODE=Release"
set "BUILD_CONAN=True"
set "PRESET_FILE=%SCRIPT_LOCATION%CMakeUserPresets.json"

rem Create the output directory if it doesn't exist
if not exist "%OUTPUT_DIR%" (
    mkdir "%OUTPUT_DIR%"
)

rem Check for debug mode
if "%1" == "-d" (
    echo -- DEBUG MODE
    set "BUILD_MODE=Debug"
)

rem Check if conan build should be performed
if "%1" == "--build-conan" (
    echo -- BUILDING CONAN DEPENDENCIES
    set "BUILD_CONAN=True"
) else (
    echo -- SKIPPING CONAN BUILD
    set "BUILD_CONAN=False"
)

rem Check if the preset file exists
if exist "%PRESET_FILE%" (
    echo -- USING PRESET FILE
    if "%BUILD_CONAN%" == "True" (
        echo -- RUNNING CONAN INSTALL
        conan install . --output-folder=build --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
    )
    cmake --preset conan-default -DCMAKE_BUILD_TYPE=%BUILD_MODE% -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="%OUTPUT_DIR%" -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="%OUTPUT_DIR%"
    cmake --build "%OUTPUT_DIR%" --preset conan-release
) else (
    echo -- RUNNING CONAN INSTALL
    conan install . --output-folder=build --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
    cmake --preset conan-default -DCMAKE_BUILD_TYPE=%BUILD_MODE% -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="%OUTPUT_DIR%" -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="%OUTPUT_DIR%"
    cmake --build "%OUTPUT_DIR%" --preset conan-release
)

endlocal
