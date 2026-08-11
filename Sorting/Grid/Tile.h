#pragma once

#include <Grid/ScreenRect.h>
#include <Grid/WorldBoundBox.h>
#include <Rendering/TexturedVertex.h>
#include <Rendering/VertexBuffer.h>

class Texture;

class Tile
{
public:
    void Init(Texture* texture, WorldBoundBox boundBoxW, const glm::vec2& gridStart, const glm::vec2& tileSize, const glm::vec2& anchor = {0.5f, 0.5f});
    void Draw() const;
    bool IsBehind(const Tile& other) const;
    const WorldBoundBox& GetBoundBoxW() const;
    const ScreenRect& GetScreenRect() const;
private:
    VertexBuffer<TexturedVertex> vertexBuffer;
    Texture* texture = nullptr;
    WorldBoundBox boundBoxW;
    ScreenRect screenRect;
};


