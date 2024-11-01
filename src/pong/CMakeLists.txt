cmake_minimum_required(VERSION 3.10)
project(pong)

# Set the default C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# If in debug mode, enable debug flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        add_compile_options(/Od /Zi)
        add_compile_definitions(DEBUG)
    else()
        add_compile_options(-O0 -g3)
        add_compile_definitions(DEBUG)
    endif()
endif()

# Enable glibc assertions for non-MSVC compilers
if(NOT MSVC)
    add_definitions(-D_GLIBCXX_ASSERTIONS)
endif()

# Enable fortify by default but not in debug mode
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 12)
        if(NOT MSVC)
            add_compile_options(-O)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 ")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 ")
        endif()
    else()
        if(NOT MSVC)
            add_definitions(-D_FORTIFY_SOURCE=2)
        endif()
    endif()
endif()

# Required packages
find_package(SFML COMPONENTS graphics window system audio network REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(Lua REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

# Add executable
add_executable(pong ${SOURCES})

# Include directories
target_include_directories(pong
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/includes
)

# Link libraries to the target
target_link_libraries(pong
        PRIVATE
        sfml-graphics sfml-window sfml-system sfml-audio sfml-network
        SDL2::SDL2
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        ${LUA_LIBRARIES}
)

# Install the target
install(TARGETS pong DESTINATION . COMPONENT pong)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION . COMPONENT pong)
cpack_add_component(pong
        DISPLAY_NAME "Pong"
        DESCRIPTION "Pong game here to show a simple POC using our engine"
)

# Optional: Add subdirectory for tests
# add_subdirectory(tests)
