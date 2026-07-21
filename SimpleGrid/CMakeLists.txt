cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SimpleGrid)

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

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

file(GLOB SOURCES
	consoleLog.h
	IsometricGrid.cpp
	IsometricGrid.h
	main.cpp
	rendering/shader.cpp
	rendering/shader.h
	rendering/vertexBuffer.cpp
	rendering/vertexBuffer.h
	window.cpp
	window.h
	glad/glad.c
)

add_executable(SimpleGrid ${SOURCES})
target_link_libraries(SimpleGrid PRIVATE glfw glm OpenGL::GL)
target_include_directories(SimpleGrid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
