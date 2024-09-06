#pragma once
#include <string>

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
    bool gameOver = false;
    bool gameWon = false;
};
