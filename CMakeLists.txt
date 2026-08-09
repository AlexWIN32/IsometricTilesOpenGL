cmake_minimum_required(VERSION 3.19)

#taken from https://github.com/PacktPublishing/3D-Graphics-Rendering-Cookbook-Second-Edition/blob/main/CMake/CommonMacros.txt
function (SETUP_GROUPS src_files)

	foreach(file IN LISTS ${src_files})

		get_filename_component(PARENT_DIR "${file}" DIRECTORY)

		# skip src or include and changes /'s to \\'s
		set(GROUP "${PARENT_DIR}")
		string(REPLACE "/" "\\" GROUP "${GROUP}")

		source_group("${GROUP}" FILES "${file}")
	endforeach()

endfunction()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(IsometricTilesOpenGL)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
	message("-- Using ccache")
	set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif(CCACHE_FOUND)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
	glfw3
	GIT_REPOSITORY https://github.com/glfw/glfw
	GIT_TAG        3.4
	GIT_SHALLOW 1
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw3)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm
	GIT_TAG        1.0.3
	GIT_SHALLOW 1
)

FetchContent_MakeAvailable(glm)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND "")

FetchContent_MakeAvailable(stb)

add_library(stb INTERFACE ${stb_SOURCE_DIR}/stb_image.h)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Common)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Sorting)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Camera)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SimpleGrid)
