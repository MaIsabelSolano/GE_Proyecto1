#include "Game.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Sprites.h"
#include "Systems.h"
#include <print>
#include <entt/entt.hpp>
#include <vector>
#include <string>
#include "Background.h"
#include "Tilemap.h"
#include <random>


class EntitiesSpawnSetupSystem : public SetupSystem {
    void run() {
        Entity* square = scene->createEntity("PLAYER", 500, 400);
        square->addComponent<VelocityComponent>(150, 150);
        square->addComponent<TextureComponent>("Assets/Sprites/chickfish.png");
        square->addComponent<SpriteComponent>("Assets/Sprites/chickfish.png", 16, 8, 10, 2, 500);
        square->addComponent<PlayerComponent>();

        Entity* item = scene->createEntity("ITEM0", 100, 500);
        item->addComponent<TextureComponent>("Assets/Sprites/item.png");
        item->addComponent<SpriteComponent>("Assets/Sprites/item.png", 8, 8, 10, 4, 1000);

        /*Entity* bar = scene->createEntity("PADDLE", 500, 600);
        bar->addComponent<VelocityComponent>(0, 0);
        bar->addComponent<SpriteComponentSimple>(100, 20, SDL_Color{ 0, 0, 255 });
        bar->addComponent<PaddleComponent>();*/

        //// Create blocks
        //for (int i = 0; i < 5; ++i) {
        //    for (int j = 0; j < 8; ++j) {
        //        Entity* block = scene->createEntity("BLOCK", 102 + j * 100, 100 + i * 40);
        //        block->addComponent<SpriteComponentSimple>(80, 30, SDL_Color{ 0, 255, 0 });
        //        block->addComponent<BlockComponent>();

        //    }
        //}

        // estado del juego 
        // Entity* gameStateEntity = scene->createEntity("GameState");
        // gameStateEntity->addComponent<GameStateComponent>();
    }
};

class MovementSystem : public UpdateSystem {
    void run(float dT) {
        auto view = scene->r.view<PositionComponent, VelocityComponent>();

        for (auto e : view) {
            auto& pos = view.get<PositionComponent>(e);
            auto vel = view.get<VelocityComponent>(e);

            pos.x += vel.x * dT;
            pos.y += vel.y * dT;

            if (pos.y >= 768) {
                auto gameStateView = scene->r.view<GameStateComponent>();

                for (auto entity : gameStateView) {
                    gameStateView.get<GameStateComponent>(entity).gameOver = true;

                }
            }
        }
    }
};

class WallHitSystem : public UpdateSystem {
    void run(float dT) {

        auto view = scene->r.view<PositionComponent, VelocityComponent, SpriteComponent>();

        for (auto e : view) {
            auto pos = view.get<PositionComponent>(e);
            auto spr = view.get<SpriteComponent>(e);
            auto& vel = view.get<VelocityComponent>(e);

            float newPosX = pos.x + vel.x * dT;
            float newPosY = pos.y + vel.y * dT;

            // cambio de dirección horizontal
            if (newPosX < 0 || newPosX + (spr.width * spr.scale) > 1024) {
                vel.x *= -1.05;

                // cambio de fondo
                auto bg = scene->r.view<BackgroundComponent>();
                for (auto ee : bg) {
                    
                    auto& sprt = bg.get<BackgroundComponent>(ee);

                    if (sprt.animationFrames > 0) {
                        // sprt.xIndex %= sprt.width;
                        sprt.xIndex = (sprt.xIndex + 1) % sprt.animationFrames;
                    }
                }

                // cambio de tilemap
                auto tilemap = scene->r.view<TilemapComponent>();
                for (auto ee : tilemap) {
                    std::printf("ee\n");

                    auto& tiles = tilemap.get<TilemapComponent>(ee);

                    std::vector<std::vector<int>> possibleMaps = tiles.possibleWalls;

                    std::random_device rd;
                    std::mt19937 gen(rd());

                    std::uniform_int_distribution<> dis(0, tiles.possibleWalls.size() - 1); // Random index range

                    // Select a random vector from possibleWalls
                    int randomIndex = dis(gen);
                    const std::vector<int>& randomWall_left = tiles.possibleWalls[randomIndex];
                    const std::vector<int>& randomWall_right = tiles.possibleWalls[randomIndex];


                    // actulizar paredes
                    for (int i = 0; i < tiles.map.size(); i ++) {
                    
                        if (i == 0) {
                            // es la primera, se queda como 3
                            tiles.map[i][0] = 3;
                        }
                        else if (i == tiles.map.size() + 1) {
                            // es la última, se queda como 4
                            tiles.map[i][0] = 4;
                        }
                        else {
                            // update
                            if (vel.x < 0) {
                                if (randomWall_left[i] == 0) {
                                    tiles.map[i][0] = 0;
                                }
                                else if (randomWall_left[i] == 1) {
                                    tiles.map[i][0] = 1;
                                }
                            }
                            else {
                                if (randomWall_right[i] == 0) {
                                    tiles.map[i][15] = 0;
                                }
                                else if (randomWall_right[i] == 1) {
                                    tiles.map[i][15] = 2;
                                }
                            }
                        }
                    }

                }

            }

            // cambio de dirección vertical
            if (newPosY < 0 || newPosY + (spr.height * spr.scale) > 768) {
                vel.y *= -1;
            }

        }
    }
};

class PaddleMovementSystem : public UpdateSystem {
    void run(float dT) {

        auto view = scene->r.view<PositionComponent, SpriteComponent>();
        auto paddleView = scene->r.view<PaddleComponent, PositionComponent, VelocityComponent, SpriteComponentSimple>();

        const Uint8* ks = SDL_GetKeyboardState(NULL);

        for (auto paddle : paddleView) {
            auto& padleVelocity = paddleView.get<VelocityComponent>(paddle);

            padleVelocity.x = 0;

            if (ks[SDL_SCANCODE_A]) {
                padleVelocity.x = -500;
            }
            if (ks[SDL_SCANCODE_D]) {
                padleVelocity.x = 500;
            }
        }
    }
};


class GameStateSystem : public SetupSystem {
public:
    void run() override {
        Entity* ponts = scene->createEntity("POINTS");
        std::printf("Game State System\n");
    }
};



class DemoGame : public Game {
public:
    Scene* sampleScene;
    entt::registry r;

public:
    DemoGame()
        : Game("SAMPLE", 1024, 768)
    { }

    void setup() {
        std::printf("HELLO WORLD\n");
        sampleScene = new Scene("SAMPLE SCENE", r, renderer);

        /* --- SETUP SYSTEMS --- */
        // fondos y sprites
        addSetupSystem<BackgroundSetupSystem>(sampleScene);
        addSetupSystem<EntitiesSpawnSetupSystem>(sampleScene);
        addSetupSystem<TilemapSetupSystem>(sampleScene);
        addSetupSystem<TextureSetupSystem>(sampleScene);
        addSetupSystem<GameStateSystem>(sampleScene);

        /* --- UPDATE SYSTEMS --- */
        addUpdateSystem<MovementSystem>(sampleScene);
        // addUpdateSystem<PaddleMovementSystem>(sampleScene);
        addUpdateSystem<WallHitSystem>(sampleScene);
        // addUpdateSystem<CollisionSystem>(sampleScene);
        addUpdateSystem<SpriteAnimationSystem>(sampleScene);
        addUpdateSystem<SpriteMovementSystem>(sampleScene);
        

        /* --- RENDER SYSTEMS --- */
        // addRenderSystem<SquareRenderSystem>(sampleScene);
        addRenderSystem<BackgroundRenderSystem>(sampleScene);
        addRenderSystem<TilemapRenderSystem>(sampleScene);
        addRenderSystem<SpriteRenderSystem>(sampleScene);
        // addUpdateSystem<GameStateSystem>(sampleScene);

        setScene(sampleScene);
    }
};