#pragma once
#include <string>
#include "Systems.h"
#include "Components.h"
#include "Entity.h"
#include "Sprites.h"

struct BackgroundComponent {
    std::string filename;
    int width;
    int height;
    int scale = 1;
    int animationFrames = 0;
    int xIndex = 0;
    int yIndex = 0;
};

class BackgroundSetupSystem : public SetupSystem {
public:
    void run() override {
        Entity* background = scene->createEntity("BACKGROUND");
        const std::string& bgfile = "Assets/Backgrounds/bg_sheet.png";
        // const std::string& bgfile = "purple.png";
        std::printf("bgfile %s \n", bgfile);
        background->addComponent<PositionComponent>(0, 0);
        background->addComponent<TextureComponent>(bgfile);
        // background->addComponent<SpriteComponent>(bgfile, 1024, 768, 1, 7, 1500);
        background->addComponent<BackgroundComponent>(bgfile, 1024, 768, 1, 7);
        std::printf("BACKGROUND SETUP SYSTEM\n");
    }

};

class BackgroundRenderSystem : public RenderSystem {
    void run(SDL_Renderer* renderer) {
        auto view = scene->r.view<PositionComponent, BackgroundComponent>();
        for (auto e : view) {
            auto pos = view.get<PositionComponent>(e);
            auto bgspr = view.get<BackgroundComponent>(e);

            Texture* texture = TextureManager::GetTexture(bgspr.filename);
            SDL_Rect clip = {
              bgspr.xIndex * bgspr.width,
              bgspr.yIndex * bgspr.height,
              bgspr.width,
              bgspr.height,
            };
            texture->render(
                scene->renderer,
                pos.x, pos.y,
                bgspr.width * bgspr.scale,
                bgspr.height * bgspr.scale,
                &clip
            );
        }
    }
};
