#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include "consoleLog.h"
#include <Grid/Sorter.h>
#include <Grid/Tile.h>
#include <Grid/WorldBoundBox.h>
#include <Rendering/ColoredVertex.h>
#include <Rendering/Shader.h>
#include <Rendering/Texture.h>
#include <Rendering/VertexBuffer.h>
#include "window.h"

int windowWidth = 1024;
int windowHeight = 768;
float aspect = 0.0f;
float texturePixelsPerUint = 2000.0f;

Window window;
Shader tilesShader;
Shader linesShader;

Texture treeTexture;
Texture stallTexture;
Texture grassTileTexture;

std::vector<Tile> grassTiles;
std::vector<Tile> objectsTiles;

std::vector<ColoredVertex> debugVertices;

bool showDebug = false;

static void CreateWindow()
{
    window.Init(windowWidth, windowHeight, "simple isometrig grid");
    window.AddResizeHandler(
        [] (int width, int height)
        {
            windowWidth = width;
            windowHeight = height;

            aspect = static_cast<float>(width) / static_cast<float>(height);

            const auto& contentScale = window.GetContentScale();
            glViewport(0, 0, width * contentScale.x, height * contentScale.y);
        }
    );
    window.AddKeyboardKeyHandler(
        [] (int key, int action)
        {
            if (key == GLFW_KEY_D && action == GLFW_PRESS) {
                showDebug = showDebug == false;
            }
        }
    );
}

static void CreateTiles()
{
    const auto grassTileTextureWidthS = grassTileTexture.GetWidthInUints();
    const auto grassTileTextureHeightS = grassTileTexture.GetHeightInUints();

    const glm::vec2 tileSize{grassTileTextureWidthS, grassTileTextureHeightS};

    const glm::vec2 gridStart{0.5f, 0.5f};
    const glm::vec2 grassGridStart{0.5f, 0.5f - tileSize.y};

    const glm::vec3 debugTilesColor{1.0f, 1.0f, 0.0f};
    const glm::vec3 debugTreesColor{1.0f, 0.0f, 0.0f};

    std::vector<glm::vec2> treesCoords =
    {
        {1.0f, 0.0f},
        {3.0f, 2.0f},
        {3.0f, 0.0f},
        {1.0f, 2.0f},
    };

    const auto stallTileBoundBoxW = WorldBoundBox::CalculateForObject({0, 2, 0.001f}, {1.0f, 1.6f, 0.08f});
    Tile stallTile;
    stallTile.Init(&stallTexture, stallTileBoundBoxW, gridStart, tileSize, {0.5f, 0.25f});
    stallTile.GetBoundBoxW().AddToDebugVertices(debugTreesColor, debugVertices, gridStart, tileSize);

    objectsTiles.push_back(std::move(stallTile));

    for (const auto& treeCoord : treesCoords) {
        const auto tileBoundBoxW = WorldBoundBox::CalculateForObject({treeCoord.x, treeCoord.y, 0.001f}, {1.5f, 1.5f, 0.25f});

        Tile treeTile;
        treeTile.Init(&treeTexture, tileBoundBoxW, gridStart, tileSize, {0.5f, 0.0f});
        treeTile.GetBoundBoxW().AddToDebugVertices(debugTreesColor, debugVertices, gridStart, tileSize);
        objectsTiles.push_back(std::move(treeTile));
    }

    const glm::ivec2 grassGridSize{5, 5};
    for (int c = 0; c < grassGridSize.x; ++c) {
        for (int r = 0; r < grassGridSize.y; ++r) {
            const auto grassTileBoundBoxW = WorldBoundBox::CalculateForObject({c, r, 0.0f}, {1.0f, 1.0f, 0.0f});

            Tile grassTile;
            grassTile.Init(&grassTileTexture, grassTileBoundBoxW, grassGridStart, tileSize);
            grassTile.GetBoundBoxW().AddToDebugVertices(debugTilesColor, debugVertices, grassGridStart, tileSize);

            grassTiles.push_back(std::move(grassTile));
        }
    }
}

static int LoadAssets()
{
    bool res = tilesShader.LoadFromFiles("./Shaders/sorting.vs.glsl", "./Shaders/sorting.fs.glsl");
    if (res == false) {
        LOG_ERROR("Unable to load tiles shaders");
        return false;
    }

    res = linesShader.LoadFromFiles("./Shaders/lines.vs.glsl", "./Shaders/lines.fs.glsl");
    if (res == false) {
        LOG_ERROR("Unable to load lines shaders");
        return false;
    }

    res = treeTexture.Load("./Textures/tree2.png", texturePixelsPerUint);
    if (res == false) {
        LOG_ERROR("Unable to load tree2.png texture");
        return false;
    }

    res = grassTileTexture.Load("./Textures/grasstextile.png", texturePixelsPerUint);
    if (res == false) {
        LOG_ERROR("Unable to load grasstextile.png texture");
        return false;
    }

    res = stallTexture.Load("./Textures/stall4.png", texturePixelsPerUint);
    if (res == false) {
        LOG_ERROR("Unable to load stall4.png texture");
        return false;
    }

    return true;
}

int main(int argc, char** argv)
{
    aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    CreateWindow();

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false) {
        LOG_ERROR("Unable to initialize GLAD");
        return -1;
    }

    if (LoadAssets() == false) {
        return -1;
    }

    CreateTiles();

    grassTiles = Sorter::SortTiles(grassTiles);
    objectsTiles = Sorter::SortTiles(objectsTiles);

    VertexBuffer<ColoredVertex> debugVertexBuffer;
    debugVertexBuffer.Init();
    debugVertexBuffer.Update(debugVertices);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (window.ShouldClose() == false) {

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        tilesShader.Use();
        tilesShader.SetUniform("aspect", aspect);

        for (const auto& tile : grassTiles) {
            tile.Draw();
        }

        for (const auto& tile : objectsTiles) {
            tile.Draw();
        }

        linesShader.Use();
        linesShader.SetUniform("aspect", aspect);

        if (showDebug == true) {
            debugVertexBuffer.Draw(GL_LINES, debugVertices.size());
        }

        window.ProcessEvents();
    }

    return 0;
}
