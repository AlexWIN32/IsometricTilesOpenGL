#include <Rendering/Texture.h>
#include "Tile.h"

void Tile::Init(Texture* texture, WorldBoundBox boundBoxW, const glm::vec2& gridStart, const glm::vec2& tileSize, const glm::vec2& anchor)
{
    this->texture = texture;
    this->boundBoxW = boundBoxW;

    screenRect = ScreenRect::FromWorldBoundBox(boundBoxW, gridStart, tileSize);

    const auto center = boundBoxW.GetIsometricSpaceCenter(gridStart, tileSize);

    glm::vec2 texSize(texture->GetWidthInUints(), texture->GetHeightInUints());

    const auto left = center.x - texSize.x * (1.0f - anchor.x);
    const auto right = center.x + texSize.x * anchor.x;

    const auto top = center.y - texSize.y * (1.0f - anchor.y);
    const auto bottom = center.y + texSize.y * anchor.y;

    std::vector<TileVertex> tileVertices =
    {
        {{left, top}, {0.0f, 0.0f}},
        {{right, top}, {1.0f, 0.0f}},
        {{right, bottom}, {1.0f, 1.0f}},
        {{right, bottom}, {1.0f, 1.0f}},
        {{left, bottom}, {0.0f, 1.0f}},
        {{left, top}, {0.0f, 0.0f}},
    };

    vertexBuffer.Init();
    vertexBuffer.Update(tileVertices);
}

bool Tile::IsBehind(const Tile& other) const
{
    const auto& boundBoxMax = boundBoxW.GetMaxCoord();
    const auto& otherBoundBoxMin = other.GetBoundBoxW().GetMinCoord();

    if (boundBoxMax.x <= otherBoundBoxMin.x) {
        return true;
    }

    if (boundBoxMax.y <= otherBoundBoxMin.y) {
        return true;
    }

    if (boundBoxMax.z < otherBoundBoxMin.z) {
        return true;
    }

    /*
    // In an isometric view, object 'a' is behind 'b' if its maximum
    // bounds are less than or equal to the minimum bounds of 'b' along the corresponding axes.
    // (Taking into account that the X and Y axes extend "into" the screen)
 
    // Check along the horizontal X and Y axes
    if (boundBoxMax.x <= otherBoundBoxMin.x && boundBoxMax.y <= otherBoundBoxMin.y) {
        return true;
    }

    // If the objects overlap in projection, check that 'a' is below 'b' (Z-axis)
    if (boundBoxMax.z <= otherBoundBoxMin.z) {
        return true;
    }

    // Standard geometric overlap conditions for isometry:
    if (boundBoxMax.x <= otherBoundBoxMin.x || boundBoxMax.y <= otherBoundBoxMin.y) {
        return true;
    }
    */
    return false;
}

void Tile::Draw() const
{
    texture->Use(0, 0);
    vertexBuffer.Draw(GL_TRIANGLES, 6);
}

const WorldBoundBox& Tile::GetBoundBoxW() const
{
    return boundBoxW;
}

const ScreenRect& Tile::GetScreenRect() const
{
    return screenRect;
}
