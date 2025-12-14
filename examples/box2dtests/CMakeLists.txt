cmake_minimum_required(VERSION 3.24)

# Default to release build if not specified
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

set(name examples_box2dtests)
project(${name})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
	src/
	../../include
	../../deps
	../../nameof/include
	../../zstd/lib
	../../stb
	../../soloud/include
	../../glfw/include
	../../lmdb/libraries/liblmdb
	../../spine-runtimes-cpp-38/include
)

file(GLOB sources
	src/*.cpp
	src/*.c
	src/*.h
	src/*.hpp
	src/*.inc
	../../include/*.h
)

if (MSVC)
	list(APPEND sources "../../include/xx.natvis")
endif()

add_executable(${name} ${sources})

source_group("" FILES ${sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../../ FILES ${sources})

SET(BUILD_TESTING OFF CACHE BOOL "BUILD_TESTING" FORCE)
SET(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "USE_MSVC_RUNTIME_LIBRARY_DLL" FORCE)
SET(ZSTD_BUILD_PROGRAMS OFF CACHE BOOL "ZSTD_BUILD_PROGRAMS" FORCE)
SET(ZSTD_BUILD_SHARED OFF CACHE BOOL "ZSTD_BUILD_SHARED" FORCE)
SET(ZSTD_BUILD_TESTS OFF CACHE BOOL "ZSTD_BUILD_SHARED" FORCE)
SET(ZSTD_LEGACY_SUPPORT OFF CACHE BOOL "ZSTD_LEGACY_SUPPORT" FORCE)
SET(ZSTD_MULTITHREAD_SUPPORT_DEFAULT OFF CACHE BOOL "ZSTD_MULTITHREAD_SUPPORT_DEFAULT" FORCE)
SET(ZSTD_MULTITHREAD_SUPPORT OFF CACHE BOOL "ZSTD_MULTITHREAD_SUPPORT" FORCE)
add_subdirectory(../../zstd/build/cmake zstd)
target_link_libraries(${name} libzstd_static)

SET(BOX2D_AVX2 ON CACHE BOOL "BOX2D_AVX2" FORCE)
add_subdirectory(../../box2d box2d)
target_link_libraries(${name} box2d)

SET(ENKITS_BUILD_C_INTERFACE OFF CACHE BOOL "ENKITS_BUILD_C_INTERFACE" FORCE)
SET(ENKITS_BUILD_EXAMPLES OFF CACHE BOOL "ENKITS_BUILD_EXAMPLES" FORCE)
add_subdirectory(../../enkiTS enkiTS)
target_link_libraries(${name} enkiTS)

add_subdirectory(../../lmdb lmdb)
target_link_libraries(${name} lmdb)

add_subdirectory(../../spine-runtimes-cpp-38 spine)
target_link_libraries(${name} spine)

add_subdirectory(../../soloud soloud)
target_link_libraries(${name} soloud)
if (APPLE)
	find_library(AUDIOTOOLBOX_FRAMEWORK AudioToolbox)
	target_link_libraries(${name} ${AUDIOTOOLBOX_FRAMEWORK})
endif()

if (WIN32)
	target_link_libraries(${name} winmm)
endif()

SET(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW_BUILD_DOCS" FORCE)
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW_BUILD_EXAMPLES" FORCE)
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW_BUILD_TESTS" FORCE)
SET(GLFW_INSTALL OFF CACHE BOOL "GLFW_INSTALL" FORCE)
add_subdirectory(../../glfw glfw)
target_link_libraries(${name} glfw)

if(MSVC)

	# target_link_options(${name} PRIVATE "/SUBSYSTEM:WINDOWS")
	add_compile_definitions(UNICODE _UNICODE)
	add_compile_options("/utf-8")
	
	target_link_options(${name} PRIVATE
		$<$<CONFIG:MinSizeRel>:/LTCG>
		$<$<CONFIG:Release>:/LTCG>
		$<$<CONFIG:RelWithDebInfo>:/LTCG>
	)

	target_compile_options(${name} PRIVATE
		"$<$<CONFIG:MinSizeRel>:/GL>"
		"$<$<CONFIG:Release>:/GL>"
		"$<$<CONFIG:RelWithDebInfo>:/GL>"
	)

	target_link_options(${name} PRIVATE
		$<$<CONFIG:Debug>:/NODEFAULTLIB:libcmtd.lib>
		$<$<CONFIG:MinSizeRel>:/NODEFAULTLIB:libcmt.lib>
		$<$<CONFIG:Release>:/NODEFAULTLIB:libcmt.lib>
		$<$<CONFIG:RelWithDebInfo>:/NODEFAULTLIB:libcmt.lib>
	)
	
	target_compile_definitions(${name} PRIVATE
		"$<$<CONFIG:MinSizeRel>:_SECURE_SCL=0>"
		"$<$<CONFIG:MinSizeRel>:_HAS_ITERATOR_DEBUGGING=0>"
		"$<$<CONFIG:Release>:_SECURE_SCL=0>"
		"$<$<CONFIG:Release>:_HAS_ITERATOR_DEBUGGING=0>"
		"$<$<CONFIG:RelWithDebInfo>:_SECURE_SCL=0>"
		"$<$<CONFIG:RelWithDebInfo>:_HAS_ITERATOR_DEBUGGING=0>"
	)

	add_compile_options($<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>)
	target_compile_options(${name} PRIVATE
		$<$<PLATFORM_ID:Windows>:/fp:fast>
		$<$<PLATFORM_ID:Windows>:/std:c17>
		$<$<PLATFORM_ID:Windows>:/bigobj>
		$<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>
	)

	set_target_properties(${name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

	set_source_files_properties(src/pch.cpp PROPERTIES COMPILE_FLAGS "/Ycpch.h")
	target_compile_options(${name} PRIVATE $<$<PLATFORM_ID:Windows>:/Yupch.h>)
	
	target_compile_options(${name} PRIVATE $<$<PLATFORM_ID:Windows>:/arch:AVX2>)
else()

	target_precompile_headers(${name}
		PUBLIC $<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>
	)
	
	target_compile_options(${name} PRIVATE -Wno-invalid-offsetof)

endif()
