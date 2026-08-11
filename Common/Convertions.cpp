#include "Convertions.h"

glm::vec2 WorldGridToIsometric(const glm::vec2& coords, const glm::vec2& tileSize, const glm::vec2& start)
{
    //https://pikuma.com/blog/isometric-projection-in-games
    return { start.x + (coords.x - coords.y) * tileSize.x * 0.5f, start.y + (coords.x + coords.y) * tileSize.y * 0.5f};

    /*
    we can also express this transform in matrix form

    xscreen = xStart + (x - y) * tileSize.x*0.5
    yscreen = yStart + (x + y) * tileSize.y*0.5

    x * tileSize.x*0.5 - y * tileSize.x*0.5 + xStart = xscreen
    x * tileSize.y*0.5 + y * tileSize.y*0.5 + yStart = yscreen

    x * tileSize.x*0.5 + y * -tileSize.x*0.5 + xStart = xscreen
    x * tileSize.y*0.5 + y * tileSize.y*0.5 + yStart = yscreen

              |tileSize.x*0.5   tileSize.y*0.5 0|
    |x y 1| * |-tileSize.x*0.5  tileSize.y*0.5 0| = |xscreen yscreen 1|
              |xStart           yStart         1|

    glm::mat3x3 transform(
        tileSize.x * 0.5f,  tileSize.y * 0.5f, 0.0f,
        -tileSize.x * 0.5f, tileSize.y * 0.5f, 0.0f,
        start.x,             start.y,            1.0f
    );

    return transform * glm::vec3(coords, 1.0f);
    */
}

glm::vec2 IsometricToWorldGrid(const glm::vec2& coords, const glm::vec2& tileSize, const glm::vec2& start)
{
    /*
    https://chatgpt.com/share/6a5ea1cf-f7b8-83eb-be8a-d0b253f31a6f

    we have

      x_screen = x_start + (x - y) * TILE_WIDTH/2
      y_screen = y_start + (x + y) * TILE_HEIGHT/2

    where x_screen and y_screen is our screen coords, x_start and y_start is 
    grid offset. We need to solve this system for x and y

    let's define

      A = x_screen - x_start
      B = y_screen - y_start

    then

      A = (x - y) * TILE_WIDTH/2
      B = (x + y) * TILE_HEIGHT/2
      x - y = 2*A/TILE_WIDTH
      x + y = 2*B/TILE_HEIGHT

    add two equations to eliminate y

      2*x = 2*A/TILE_WIDTH + 2*B/TILE_HEIGHT
      x = A/TILE_WIDTH + B/TILE_HEIGHT

    subtract first equation from the second to eliminate x

      2*y = 2*B/TILE_HEIGHT - 2*A/TILE_WIDTH
      y = B/TILE_HEIGHT - A/TILE_WIDTH

    now we need to substitute A and B

      x = (x_screen - x_start)/TILE_WIDTH + (y_screen - y_start)/TILE_HEIGHT
      y = (y_screen - y_start)/TILE_HEIGHT - (x_screen - x_start)/TILE_WIDTH
    */

    return {
        (coords.x - start.x) / tileSize.x + (coords.y - start.y) / tileSize.y,
        (coords.y - start.y) / tileSize.y - (coords.x - start.x) / tileSize.x
    };

    /*
    like in gridToIsometric() we can express grid to isometric transform in matrix form
    and than multiply isometric coords by its inverse

    glm::mat3x3 transform(
        tileSize.x * 0.5f,  tileSize.y * 0.5f, 0.0f,
        -tileSize.x * 0.5f, tileSize.y * 0.5f, 0.0f,
        xStart,             yStart,            1.0f
    );

    return glm::inverse(transform) * glm::vec3(coords, 1.0f);
    */
}
