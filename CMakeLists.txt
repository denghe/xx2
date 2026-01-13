SET(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW_BUILD_DOCS" FORCE)
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW_BUILD_EXAMPLES" FORCE)
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW_BUILD_TESTS" FORCE)
SET(GLFW_INSTALL OFF CACHE BOOL "GLFW_INSTALL" FORCE)
add_subdirectory(glfw glfw)

SET(BUILD_TESTING OFF CACHE BOOL "BUILD_TESTING" FORCE)
SET(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "USE_MSVC_RUNTIME_LIBRARY_DLL" FORCE)
SET(ZSTD_BUILD_PROGRAMS OFF CACHE BOOL "ZSTD_BUILD_PROGRAMS" FORCE)
SET(ZSTD_BUILD_SHARED OFF CACHE BOOL "ZSTD_BUILD_SHARED" FORCE)
SET(ZSTD_BUILD_TESTS OFF CACHE BOOL "ZSTD_BUILD_SHARED" FORCE)
SET(ZSTD_LEGACY_SUPPORT OFF CACHE BOOL "ZSTD_LEGACY_SUPPORT" FORCE)
SET(ZSTD_MULTITHREAD_SUPPORT_DEFAULT OFF CACHE BOOL "ZSTD_MULTITHREAD_SUPPORT_DEFAULT" FORCE)
SET(ZSTD_MULTITHREAD_SUPPORT OFF CACHE BOOL "ZSTD_MULTITHREAD_SUPPORT" FORCE)
add_subdirectory(zstd/build/cmake zstd)

SET(BOX2D_AVX2 ON CACHE BOOL "BOX2D_AVX2" FORCE)
add_subdirectory(box2d box2d)

SET(ENKITS_BUILD_C_INTERFACE OFF CACHE BOOL "ENKITS_BUILD_C_INTERFACE" FORCE)
SET(ENKITS_BUILD_EXAMPLES OFF CACHE BOOL "ENKITS_BUILD_EXAMPLES" FORCE)
add_subdirectory(enkiTS enkiTS)

add_subdirectory(lmdb lmdb)

add_subdirectory(spine-runtimes-cpp-38 spine)

add_subdirectory(soloud soloud)

file(GLOB xx_sources
	include/*.h
	include/*.natvis
	src/xx.cpp
	deps/*.h
	deps/*.c
	deps/*.cpp
	stb/*.h
)
add_library(xx
    ${xx_sources}
)

if (WIN32)
	target_link_libraries(xx winmm)
endif()

source_group("" FILES ${xx_sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${xx_sources})

target_include_directories(xx PUBLIC
#	glfw/include
	include
	deps
	stb
)
target_link_libraries(xx glfw)
target_link_libraries(xx libzstd_static)
target_link_libraries(xx lmdb)
target_link_libraries(xx spine)
target_link_libraries(xx box2d)
target_link_libraries(xx enkiTS)
target_link_libraries(xx soloud)
if (APPLE)
	find_library(AUDIOTOOLBOX_FRAMEWORK AudioToolbox)
	target_link_libraries(xx ${AUDIOTOOLBOX_FRAMEWORK})
endif()


if(MSVC)

	add_compile_definitions(UNICODE _UNICODE)
	add_compile_options("/utf-8")

	target_link_options(xx PRIVATE
		$<$<CONFIG:MinSizeRel>:/LTCG>
		$<$<CONFIG:Release>:/LTCG>
		$<$<CONFIG:RelWithDebInfo>:/LTCG>
	)

	target_compile_options(xx PRIVATE
		"$<$<CONFIG:MinSizeRel>:/GL>"
		"$<$<CONFIG:Release>:/GL>"
		"$<$<CONFIG:RelWithDebInfo>:/GL>"
	)

	target_link_options(xx PRIVATE
		$<$<CONFIG:Debug>:/NODEFAULTLIB:libcmtd.lib>
		$<$<CONFIG:MinSizeRel>:/NODEFAULTLIB:libcmt.lib>
		$<$<CONFIG:Release>:/NODEFAULTLIB:libcmt.lib>
		$<$<CONFIG:RelWithDebInfo>:/NODEFAULTLIB:libcmt.lib>
	)
	
	target_compile_definitions(xx PRIVATE
		"$<$<CONFIG:MinSizeRel>:_SECURE_SCL=0>"
		"$<$<CONFIG:MinSizeRel>:_HAS_ITERATOR_DEBUGGING=0>"
		"$<$<CONFIG:Release>:_SECURE_SCL=0>"
		"$<$<CONFIG:Release>:_HAS_ITERATOR_DEBUGGING=0>"
		"$<$<CONFIG:RelWithDebInfo>:_SECURE_SCL=0>"
		"$<$<CONFIG:RelWithDebInfo>:_HAS_ITERATOR_DEBUGGING=0>"
	)

	add_compile_options($<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>)
	target_compile_options(xx PRIVATE
		$<$<PLATFORM_ID:Windows>:/fp:fast>
		$<$<PLATFORM_ID:Windows>:/std:c17>
		$<$<PLATFORM_ID:Windows>:/bigobj>
		$<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>
	)
	
else()
	
	target_compile_options(${name} PRIVATE -Wno-invalid-offsetof)

endif()
