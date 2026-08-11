set(SOURCES
	Grid/ScreenRect.cpp
	Grid/ScreenRect.h
	Grid/Tile.cpp
	Grid/Tile.h
	Grid/Sorter.cpp
	Grid/Sorter.h
	Grid/WorldBoundBox.cpp
	Grid/WorldBoundBox.h
	main.cpp
)

SETUP_GROUPS(SOURCES)

add_executable(Sorting ${SOURCES})
target_link_libraries(Sorting PRIVATE glfw glm stb OpenGL::GL common)
target_include_directories(Sorting PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/Common)

if (MSVC)
	set_target_properties(Sorting PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()
