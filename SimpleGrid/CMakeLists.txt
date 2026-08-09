set(SOURCES
	ColoredVertex.h
	IsometricGrid.cpp
	IsometricGrid.h
	main.cpp
)

SETUP_GROUPS(SOURCES)

add_executable(SimpleGrid ${SOURCES})
target_link_libraries(SimpleGrid PRIVATE glfw glm OpenGL::GL common)
target_include_directories(SimpleGrid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/Common)

if (MSVC)
	set_target_properties(SimpleGrid PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()
