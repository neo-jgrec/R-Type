cmake_minimum_required(VERSION 3.10)
project(r-type_server)

# Set the default C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# If in debug mode, enable debug flags
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (MSVC)
        add_compile_options(/Od /Zi)
        add_compile_definitions(DEBUG)
    else ()
        add_compile_options(-O0 -g3)
        add_compile_definitions(DEBUG)
    endif ()
endif ()

# Enable glibc assertions for non-MSVC compilers
if (NOT MSVC)
    add_definitions(-D_GLIBCXX_ASSERTIONS)
endif ()

# Enable fortify by default but not in debug mode
if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 12)
        if (NOT MSVC)
            # GCC 12 supports automatically initializing variables to zero
            add_compile_options(-O)

            # Adjust _FORTIFY_SOURCE for GCC/Clang
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 ")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 ")
        endif ()
    else ()
        if (NOT MSVC)
            add_definitions(-D_FORTIFY_SOURCE=2)
        endif ()
    endif ()
endif ()

find_package(asio REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(SFML COMPONENTS graphics window system audio network REQUIRED)
find_package(Lua REQUIRED)

file(GLOB_RECURSE SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)

# Add executable
add_executable(r-type_server ${SOURCES})

# Include directories
target_include_directories(r-type_server
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/includes
)

# Link Vulkan and GLFW libraries
target_link_libraries(r-type_server
        PRIVATE
        asio::asio
        nlohmann_json::nlohmann_json
        sfml-graphics sfml-window sfml-system sfml-audio sfml-network
        ${LUA_LIBRARIES}
)

# Install the target
install(TARGETS r-type_server DESTINATION . COMPONENT server)
cpack_add_component(server
        DISPLAY_NAME "R-Type Server"
        DESCRIPTION "The server program of the R-Type game"
)

# Add subdirectory for tests
# add_subdirectory(tests)
