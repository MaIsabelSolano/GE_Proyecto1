#pragma once
#include <string>
#include "Entity.h"
#include "Systems.h"
#include "Components.h"
#include <iostream>

enum class TileType {
    NONE,
    SPIKEUPPER,
    SPIKELOWER,
    SPIKERIGHT,
    SPIKELEFT,
};

struct Tile {
    int index;
    int tilemapIndex;
    TileType type;
};

struct TileComponent {
    Tile tile;
};

struct TilemapComponent {
    std::string filename;
    std::vector<Tile> tiles;
    std::vector<std::vector<int>> possibleWalls;
    int tileSize;
    int scale;
    int width;
    int height;
};

class TilemapSetupSystem : public SetupSystem {
public:
    void run() override {
        std::printf("TILEMAP SET UP SYSTEM\n");
        std::vector<int> initialMap = {
          3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
          4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
        };

        std::vector<std::vector<int>> possibleWalls = {
          {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
          {1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
          {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
          {1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1},
          {1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
          {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
          {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1},
          {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
          {1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
          {1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
          {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
          {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1},
          {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1},
          {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
          {1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
          {1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1},
          {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
          {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
          {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1}
        };

        std::string filename = "Assets/Sprites/spikes.png";
        
        int tileSize = 8;
        int tileScale = 8;

        std::vector<Tile> tiles;

        for (int i = 0; i < initialMap.size(); i++) {
            TileType type = TileType::NONE;
            switch (initialMap[i]) {
                case 1:
                    type = TileType::SPIKELEFT;
                case 2:
                    type = TileType::SPIKERIGHT;
                case 3:
                    type = TileType::SPIKEUPPER;
                case 4:
                    type = TileType::SPIKELOWER;
            }
            Tile tile = Tile{ i, initialMap[i], type};
            tiles.push_back(tile);

            // std::printf("TileIndex: %d\n", i);

        }

        Entity* tilemapEntity = scene->createEntity("TILEMAP");
        tilemapEntity->addComponent<TilemapComponent>(
            filename,
            tiles,
            possibleWalls,
            tileSize,
            tileScale,
            16,
            12

        );
        tilemapEntity->addComponent<TextureComponent>(filename);

        /*for (Tile t : tiles) {
            std::printf("initial %d\n", t.index);
        }*/
    }
};


class TilemapRenderSystem : public RenderSystem {
    void run(SDL_Renderer* renderer) {
        auto view = scene->r.view<TilemapComponent, TextureComponent>();
        for (auto e : view) {
            auto tmap = view.get<TilemapComponent>(e);
            auto tex = view.get<TextureComponent>(e);

            Texture* texture = TextureManager::GetTexture(tex.filename);

            // std::printf("Texture: %s\n", tex.filename.c_str());

            int tileSize = tmap.tileSize * tmap.scale;
            int tilemapHeight = tmap.height;
            int tilemapWidth = tmap.width;

            /*for (Tile t : tmap.tiles) {
                std::printf("initial %s\n", t.tilemapIndex);
            }*/

            for (int y = 0; y < tilemapHeight; y++) {
                for (int x = 0; x < tilemapWidth; x++) {

                    int tileIndex = tmap.tiles[y * tilemapWidth + x].tilemapIndex;

                    // std::printf("x: %d, y:%d, TileIndex: %d\n", x, y, tileIndex);
                    TileType tt = tmap.tiles[y * tilemapWidth + x].type;


                    if (tileIndex > 0) {

                        int tileIndexX = tileIndex % 8;
                        int tileIndexY = tileIndex / 8;

                        if (tileIndex == 1) {
                            tileIndexX = 0;
                            tileIndexY = 0;
                        }

                        if (tileIndex == 2) {
                            tileIndexX = 1;
                            tileIndexY = 0;
                        }

                        if (tileIndex == 3) {
                            tileIndexX = 0;
                            tileIndexY = 1;
                        }

                        if (tileIndex == 4) {
                            tileIndexX = 1;
                            tileIndexY = 1;
                        }

                        SDL_Rect clip = {
                          tileIndexX * tmap.tileSize,
                          tileIndexY * tmap.tileSize,
                          tmap.tileSize,
                          tmap.tileSize,
                        };

                        /*std::printf("tileIndexX: %d\n", tileIndexX);
                        std::printf("tileIndexY: %d\n", tileIndexY);
                        std::printf("Tile size: %d\n", tmap.tileSize);*/

                        texture->render(
                            scene->renderer,
                            x * tileSize,
                            y * tileSize,
                            tileSize,
                            tileSize,
                            &clip
                        );
                    }
                }
            }
        }
    }
};



