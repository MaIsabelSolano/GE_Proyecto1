#pragma once
#include <string>
#include "Systems.h"
#include "Components.h"
#include "Entity.h"
#include "Sprites.h"

struct BackgroundComponent {
    std::string filename;
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
        background->addComponent<SpriteComponent>(bgfile, 1024, 768, 1, 7, 1500);
        background->addComponent<BackgroundComponent>(bgfile);
        std::printf("BACKGROUND SETUP SYSTEM\n");
    }
};
