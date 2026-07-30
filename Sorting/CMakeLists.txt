cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Sorting)

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

file(GLOB SOURCES
	consoleLog.h
	glad/glad.c
	Grid/Convertions.cpp
	Grid/Convertions.h
	Grid/ScreenRect.cpp
	Grid/ScreenRect.h
	Grid/Tile.cpp
	Grid/Tile.h
	Grid/Sorter.cpp
	Grid/Sorter.h
	Grid/WorldBoundBox.cpp
	Grid/WorldBoundBox.h
	main.cpp
	Rendering/LineVertex.h
	Rendering/Shader.cpp
	Rendering/Shader.h
	Rendering/Texture.cpp
	Rendering/Texture.h
	Rendering/TileVertex.h
	Rendering/VertexAttribute.h
	Rendering/vertexBuffer.h
	stb_image.cpp
	window.cpp
	window.h
)

add_executable(Sorting ${SOURCES})
target_link_libraries(Sorting PRIVATE glfw glm stb OpenGL::GL)
target_include_directories(Sorting PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
