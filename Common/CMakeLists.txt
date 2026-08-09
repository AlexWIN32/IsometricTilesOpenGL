set(SOURCES
    consoleLog.h
    glad/glad.c
    glad/glad.h
    KHR/khrplatform.h
    Rendering/Shader.cpp
    Rendering/Shader.h
    Rendering/Texture.cpp
    Rendering/Texture.h
    Rendering/VertexAttribute.h
    Rendering/VertexBuffer.h
    stb_image.cpp
    window.cpp
    window.h
)

SETUP_GROUPS(SOURCES)

add_library(common STATIC ${SOURCES})
target_link_libraries(common PRIVATE glfw glm stb OpenGL::GL common)
target_include_directories(common PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
