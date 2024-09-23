@echo off

REM Set the script location to the directory of this script
set SCRIPT_LOCATION=%~dp0
set OUTPUT_DIR=%SCRIPT_LOCATION%build\
set BUILD_MODE=Release
set CMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake

REM Check if the output directory exists, if not, create it
if not exist "%OUTPUT_DIR%" (
    mkdir "%OUTPUT_DIR%"
)

REM Check if -d argument is passed for Debug mode
if "%~1"=="-d" (
    echo -- DEBUG MODE
    set BUILD_MODE=Debug
)

REM Run cmake with the defined parameters
cmake -S "%SCRIPT_LOCATION%" -B "%OUTPUT_DIR%" -DCMAKE_BUILD_TYPE=%BUILD_MODE% -DCMAKE_TOOLCHAIN_FILE=%CMAKE_TOOLCHAIN_FILE% -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=%OUTPUT_DIR%
cmake --build "%OUTPUT_DIR%" --config %BUILD_MODE%
