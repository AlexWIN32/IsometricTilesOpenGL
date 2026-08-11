set(SOURCES
	IsometricGrid.cpp
	IsometricGrid.h
	main.cpp
	PanAndZoomCamera.cpp
	PanAndZoomCamera.h
)

SETUP_GROUPS(SOURCES)

add_executable(Camera ${SOURCES})
target_link_libraries(Camera PRIVATE glfw glm OpenGL::GL common)
target_include_directories(Camera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/Common)

if (MSVC)
	set_target_properties(Camera PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()
