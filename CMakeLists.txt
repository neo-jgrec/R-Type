cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(R-Type)

# Use pthreads if possible
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)

if(CMAKE_USE_PTHREADS_INIT)
	set(CMAKE_THREAD_LIBS_INIT "pthread")
endif()

# Disable developer warnings for CMake
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")

# The default name of the generated component
set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME "default")

# Export all the compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# If ccache is installed, use to speed up the build
find_program(CCACHE_FOUND ccache HINTS /bin /usr/bin)
if(CCACHE_FOUND)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()

if (UNIX)
	add_compile_options(-Wall
		-Wextra
		-Wpedantic
		-fexceptions
		-ffunction-sections
		-fdata-sections
		-fasynchronous-unwind-tables
		-fno-delete-null-pointer-checks
		-fno-common
		-fno-omit-frame-pointer
		-fno-strict-aliasing
		-fno-semantic-interposition
		-fstack-protector-strong
		-fstack-clash-protection
		-flto=auto
		-fdiagnostics-color=auto
		-grecord-gcc-switches
		-pipe
		-pthread
	)
elseif (WIN32)
	add_compile_options(/W4)
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_CONTACT "R-Type Team")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "R-Type is a side-scrolling shoot-em-up arcade game")
set(CPACK_PACKAGE_VENDOR "R-Type Team")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_COMPONENTS_ALL client editor server)

set(CPACK_COMPONENT_GROUPS_ALL R-Type)
set(CPACK_COMPONENT_GROUP_R-Type_DESCRIPTION "R-Type component group")

set(CPACK_COMPONENT_CLIENT_GROUP R-Type)
set(CPACK_COMPONENT_EDITOR_GROUP R-Type)
set(CPACK_COMPONENT_SERVER_GROUP R-Type)

set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP_MANDATORY)

include(CPack)

add_subdirectory(src)

if (WIN32)
	file(GLOB DLLS "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/*.dll")
	install(FILES ${DLLS} DESTINATION . COMPONENT client)
	install(FILES ${DLLS} DESTINATION . COMPONENT editor)
	install(FILES ${DLLS} DESTINATION . COMPONENT server)
endif()
