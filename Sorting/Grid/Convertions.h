#pragma once
#include <glm/vec2.hpp>

glm::vec2 WorldGridToIsometric(const glm::vec2& coords, const glm::vec2& tileSize, const glm::vec2& start);
glm::vec2 IsometricToWorldGrid(const glm::vec2& coords, const glm::vec2& tileSize, const glm::vec2& start);
