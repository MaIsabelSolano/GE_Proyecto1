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
#include "Colliders.h"


class EntitiesSpawnSetupSystem : public SetupSystem {
    void run() {
        Entity* player = scene->createEntity("PLAYER", 500, 400);
        player->addComponent<VelocityComponent>(150, 150);
        player->addComponent<TextureComponent>("Assets/Sprites/chickfish.png");
        player->addComponent<SpriteComponent>("Assets/Sprites/chickfish.png", 16, 8, 10, 2, 500);
        player->addComponent<PlayerComponent>();
        player->addComponent<BoxColliderComponent>(SDL_Rect{ 0, 0, 160, 80 }, SDL_Color{ 255, 0, 0 });

        Entity* item = scene->createEntity("ITEM0", 200, 400);
        // item->addComponent<VelocityComponent>(150, 150);
        item->addComponent<TextureComponent>("Assets/Sprites/item.png");
        item->addComponent<SpriteComponent>("Assets/Sprites/item.png", 8, 8, 10, 4, 1000);
        item->addComponent<PowerUpComponent>();
        item->addComponent<BoxColliderComponent>(SDL_Rect{ 0, 0, 80, 80 }, SDL_Color{ 0, 255, 0 });

    }
};

class HorizontalMovementSystem : public UpdateSystem {
    void run(float dT) {
        auto view = scene->r.view<PositionComponent, VelocityComponent, PlayerComponent>();
        auto gameStateView = scene->r.view<GameStateComponent>();

        for (auto ee : gameStateView) {
            auto& game = gameStateView.get<GameStateComponent>(ee);

            if (game.running) {
                for (auto e : view) {
                    auto& pos = view.get<PositionComponent>(e);
                    auto vel = view.get<VelocityComponent>(e);

                    pos.x += vel.x * dT;

                }
            }
          
        }
        
    }
};

class VerticalMovementSystem : public UpdateSystem {
    void run(float dT) {
        auto view = scene->r.view<PositionComponent, VelocityComponent, PlayerComponent>();
        auto gameStateView = scene->r.view<GameStateComponent>();

        for (auto ee : gameStateView) {
            auto& game = gameStateView.get<GameStateComponent>(ee);

            if (game.running) {
                const Uint8* ks = SDL_GetKeyboardState(NULL);

                for (auto e : view) {
                    auto& pos = view.get<PositionComponent>(e);
                    auto& vel = view.get<VelocityComponent>(e);


                    if (ks[SDL_SCANCODE_UP] | ks[SDL_SCANCODE_W]) {
                        vel.y = -150;
                    }
                    else if (ks[SDL_SCANCODE_DOWN] | ks[SDL_SCANCODE_S]) {
                        vel.y = 150;
                    }

                    pos.y += vel.y * dT;
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
                    // std::printf("ee\n");

                    auto& tiles = tilemap.get<TilemapComponent>(ee);

                    std::vector<std::vector<int>> possibleMaps = tiles.possibleWalls;

                    std::random_device rd;
                    std::mt19937 gen(rd());

                    std::uniform_int_distribution<> dis(0, tiles.possibleWalls.size() - 1); // Random index range

                    // Select a random vector from possibleWalls
                    int randomIndex = dis(gen);

                    std::vector<Tile> newTiles = tiles.tiles;

                    const std::vector<int>& randomWall = tiles.possibleWalls[randomIndex];

                    for (int y = 0; y < tiles.height; y++) {
                        if (y == 0 || y == tiles.height - 1) {
                            // no es necesario cambiarlas
                        }
                        else {
                            // si está yendo a la izquierda, se cambian las spikes de la izquierda
                            if (vel.x < 0) {
                                if (randomWall[y] == 1) {
                                    // spike 
                                    Tile newTile = Tile{ y * tiles.width, 1, TileType::SPIKELEFT };
                                    
                                    newTiles[y * tiles.width + 0] = newTile;
                                    
                                }
                                else if (randomWall[y] == 0) {
                                    Tile newTile = Tile{ y * tiles.width, 0, TileType::NONE };
                                    newTiles[y * tiles.width + 0] = newTile;
                                }
                            }
                            if (vel.x >= 0) {
                                if (randomWall[y] == 1) {
                                    // spike 
                                    Tile newTile = Tile{ y * tiles.width + (tiles.width - 1), 2, TileType::SPIKERIGHT };
                                    newTiles[y * tiles.width + (tiles.width - 1)] = newTile;
                                }
                                else if (randomWall[y] == 0) {
                                    Tile newTile = Tile{ y * tiles.width + (tiles.width - 1) , 0, TileType::NONE };
                                    newTiles[y * tiles.width + (tiles.width - 1)] = newTile;
                                }
                            }

                        }
                    }

                    // remplazo
                    tiles.tiles = newTiles;

                }

                auto CollidesrView = scene->r.view<BoxColliderComponent, TileColliderComponent>();
                for (auto entity : CollidesrView) {
                    // auto& tilemapCollider = view.get<TileColliderComponent>(entity);
                    
                    scene->r.destroy(entity);
                    
                }

                // update de los tiles
                auto Tilesview = scene->r.view<TilemapComponent>();

                for (auto entity : Tilesview) {
                    auto tilemap = Tilesview.get<TilemapComponent>(entity);

                    for (int y = 0; y < tilemap.height; y++) {
                        for (int x = 0; x < tilemap.width; x++) {
                            int index = y * tilemap.width + x;

                            const Tile& tile = tilemap.tiles[index];
                            // createTileEntity(x, y, tilemap.tileSize * tilemap.scale, tile);
                            Entity* tileEntity = scene->createEntity("TILE");
                            tileEntity->addComponent<TileColliderComponent>();

                            auto size = tilemap.tileSize * tilemap.scale;

                            tileEntity->addComponent<PositionComponent>(x * size, y * size);
                            tileEntity->addComponent<TileComponent>(tile);
                            if (
                                tile.type == TileType::SPIKELEFT ||
                                tile.type == TileType::SPIKELOWER ||
                                tile.type == TileType::SPIKERIGHT ||
                                tile.type == TileType::SPIKEUPPER
                                ) {
                                SDL_Rect colliderRect = { 0, 0, size, size };
                                SDL_Color color = { 0, 0, 255 };
                                tileEntity->addComponent<BoxColliderComponent>(colliderRect, color);
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

class BoostSystem : public UpdateSystem {
    void run(float dT) {

        auto boostView = scene->r.view<PowerUpComponent, BoxColliderComponent, PositionComponent>();
        auto pointsView = scene->r.view<PoinsComponent>();
        auto playerView = scene->r.view<PlayerComponent, BoxColliderComponent, PositionComponent>();

        for (auto pointsEntity : pointsView) {
            auto& points = pointsView.get<PoinsComponent>(pointsEntity);

            for (auto p : playerView) {
                auto& playerCollider = playerView.get<BoxColliderComponent>(p);

                if (playerCollider.collisionType == CollisionType::BOOST) {
                    // tocó el booster
                    

                    for (auto b : boostView) {
                        auto& boostPU = boostView.get<PowerUpComponent>(b);
                        auto& boostPos = boostView.get<PositionComponent>(b);


                        std::random_device rd;
                        std::mt19937 gen(rd());

                        std::uniform_int_distribution<> dis(0, boostPU.possiblePositions.size() - 1); // Random index range

                        // Select a random vector from possibleWalls
                        int randomIndex = dis(gen);

                        boostPos.x = boostPU.possiblePositions[randomIndex][0];
                        boostPos.y = boostPU.possiblePositions[randomIndex][1];

                        // sumamos los puntos después
                        points.poins += 1;
                        std::printf("Puntos: %d\n", points.poins);


                    }

                }
            }

            
        }
    }
};

class GAMEOVER : public UpdateSystem {
    void run(float dT) {

        auto view = scene->r.view<BoxColliderComponent>();
        auto gameStateView = scene->r.view<GameStateComponent>();

        for (auto e : view) {
            auto& collider = view.get<BoxColliderComponent>(e);

            if (collider.collisionType == CollisionType::SPIKE) {

                for (auto ee : gameStateView) {
                    auto& game = gameStateView.get<GameStateComponent>(ee);

                    game.running = false;

                }
                
            }
        }
    }
};


class GameStateSystem : public SetupSystem {
public:
    void run() override {
        Entity* gameState = scene->createEntity("");
        gameState->addComponent<GameStateComponent>();

        Entity* points = scene->createEntity("POINTS");
        points->addComponent<PoinsComponent>();
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
        addSetupSystem<TilemapColliderSetupSystem>(sampleScene);
        addSetupSystem<GameStateSystem>(sampleScene);

        /* --- UPDATE SYSTEMS --- */
        addUpdateSystem<HorizontalMovementSystem>(sampleScene);
        addUpdateSystem<VerticalMovementSystem>(sampleScene);
        // addUpdateSystem<PaddleMovementSystem>(sampleScene);
        addUpdateSystem<WallHitSystem>(sampleScene);
        // addUpdateSystem<CollisionSystem>(sampleScene);
        addUpdateSystem<SpriteAnimationSystem>(sampleScene);
        addUpdateSystem<SpriteMovementSystem>(sampleScene);
        
        addUpdateSystem<PlayerPowerUpCollisionDetectionSystem>(sampleScene);
        addUpdateSystem<PlayerPowerUpCollisionSystem>(sampleScene);

        addUpdateSystem<PlayerTileCollisionDetectionSystem>(sampleScene);

        addUpdateSystem< BoostSystem>(sampleScene);

        addUpdateSystem<GAMEOVER>(sampleScene);

        addUpdateSystem<ColliderResetSystem>(sampleScene);
        

        /* --- RENDER SYSTEMS --- */
        // addRenderSystem<SquareRenderSystem>(sampleScene);
        addRenderSystem<BackgroundRenderSystem>(sampleScene);
        addRenderSystem<TilemapRenderSystem>(sampleScene);
        addRenderSystem<SpriteRenderSystem>(sampleScene);
        // addUpdateSystem<GameStateSystem>(sampleScene);
        addRenderSystem<ColliderRenderSystem>(sampleScene);

        setScene(sampleScene);
    }
};