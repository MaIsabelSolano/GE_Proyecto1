#pragma once
#include <string>
#include "Entity.h"
#include "Systems.h"
#include "Components.h"
#include <iostream>

struct TilemapComponent {
    std::string filename;
    std::vector<std::vector<int>> map;
    std::vector<std::vector<int>> possibleWalls;
    int tileSize;
    int scale;
};

class TilemapSetupSystem : public SetupSystem {
public:
    void run() override {
        std::printf("TILEMAP SET UP SYSTEM\n");
        std::vector<std::vector<int>> map = {
          {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
          {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}
        };

        std::vector<std::vector<int>> possibleWalls = {
          {1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
          {1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
          {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
          {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1},
          {1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1},
          {1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
          {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1},
          {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1},
          {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
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
        Entity* tilemapEntity = scene->createEntity("TILEMAP");
        tilemapEntity->addComponent<TilemapComponent>(
            filename,
            map,
            possibleWalls,
            8,
            8
        );
        tilemapEntity->addComponent<TextureComponent>(filename);
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
            int tilemapHeight = tmap.map.size();
            int tilemapWidth = tmap.map[0].size();

            for (int y = 0; y < tilemapHeight; y++) {
                for (int x = 0; x < tilemapWidth; x++) {

                    int tileIndex = tmap.map[y][x];

                    if (tileIndex > 0) {


                        int tileIndexX = 0;
                        int tileIndexY = 0;

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



