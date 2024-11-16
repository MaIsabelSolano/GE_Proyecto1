#pragma once
#include <string>
#include <iostream>

struct NameComponent {
    const std::string& name;
};

struct PositionComponent {
    int x;
    int y;
};

struct VelocityComponent {
    int x;
    int y;
};

struct SpriteComponentSimple {
    int width;
    int height;
    SDL_Color color;
};

struct PaddleComponent {
    int width;
    int height;
    SDL_Color color;
};

struct BallComponent {
    int width;
    int height;
    SDL_Color color;
};

struct PlayerComponent {

};

struct BlockComponent {
    int width;
    int height;
    SDL_Color color;
};

// Components.h
struct GameStateComponent {
    bool running = true;
};

struct PoinsComponent {
    int poins = 0;
};

struct TileColliderComponent {
    
};

struct PowerUpComponent {
    std::vector<std::vector<int>> possiblePositions = {
        {400, 80},
        {600, 600},
        {200, 400},
        {200, 100},
        {500, 500},
    };
};