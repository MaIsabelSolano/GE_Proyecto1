#include "Game.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Systems.h"
#include <print>
#include <entt/entt.hpp>
#include <vector>
#include <string>


class EntitiesSpawnSetupSystem : public SetupSystem {
    void run() {
        Entity* square = scene->createEntity("BALL", 500, 400);
        square->addComponent<VelocityComponent>(150, 150);
        square->addComponent<SpriteComponent>(20, 20, SDL_Color{ 255, 0, 0 });
        square->addComponent<BallComponent>();

        Entity* bar = scene->createEntity("PADDLE", 500, 600);
        bar->addComponent<VelocityComponent>(0, 0);
        bar->addComponent<SpriteComponent>(100, 20, SDL_Color{ 0, 0, 255 });
        bar->addComponent<PaddleComponent>();

        // Create blocks
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 8; ++j) {
                Entity* block = scene->createEntity("BLOCK", 102 + j * 100, 100 + i * 40);
                block->addComponent<SpriteComponent>(80, 30, SDL_Color{ 0, 255, 0 });
                block->addComponent<BlockComponent>();

            }
        }

        // estado del juego 
        Entity* gameStateEntity = scene->createEntity("GameState");
        gameStateEntity->addComponent<GameStateComponent>();
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

            if (newPosX < 0 || newPosX + spr.width > 1024) {
                vel.x *= -1.05;
            }

            if (newPosY < 0 || newPosY + spr.height > 768) {
                vel.y *= -1.05;
            }

        }
    }
};

class SquareRenderSystem : public RenderSystem {
    void run(SDL_Renderer* renderer) {
        auto view = scene->r.view<PositionComponent, SpriteComponent>();
        for (auto e : view) {
            auto pos = view.get<PositionComponent>(e);
            auto spr = view.get<SpriteComponent>(e);

            SDL_SetRenderDrawColor(renderer, spr.color.r, spr.color.g, spr.color.b, spr.color.a);
            SDL_Rect r = { pos.x, pos.y, spr.width, spr.height };
            SDL_RenderFillRect(renderer, &r);
        }
    }
};

class CollisionSystem : public UpdateSystem {
    void run(float dt) {
        // 
        auto view = scene->r.view<PositionComponent, SpriteComponent>();

        auto ballView = scene->r.view<BallComponent, PositionComponent, VelocityComponent, SpriteComponent>();
        auto paddleView = scene->r.view<PaddleComponent, PositionComponent, VelocityComponent, SpriteComponent>();
        auto blockView = scene->r.view<BlockComponent, PositionComponent, SpriteComponent>();

        auto gameStateView = scene->r.view<GameStateComponent>();
        auto& gameState = gameStateView.get<GameStateComponent>(gameStateView.front());

        for (auto ball : ballView) {
            auto& ballPos = ballView.get<PositionComponent>(ball);
            auto& ballVel = ballView.get<VelocityComponent>(ball);
            auto& ballSpr = ballView.get<SpriteComponent>(ball);


            // game over al fondo de la pantalla
            if (ballPos.y + ballSpr.height > 768) {
                gameState.gameOver = true;
            }

            // colisión con el paddle
            for (auto player : paddleView) {
                auto& playerPos = paddleView.get<PositionComponent>(player);
                auto& playerSpr = paddleView.get<SpriteComponent>(player);

                bool collisionX = ballPos.x + ballSpr.width >= playerPos.x &&
                    playerPos.x + playerSpr.width >= ballPos.x;

                bool collisionY = ballPos.y + ballSpr.height >= playerPos.y &&
                    playerPos.y + playerSpr.height >= ballPos.y;

                if (collisionX && collisionY) {
                    ballVel.y = -ballVel.y;
                    ballPos.y = ballPos.y < playerPos.y ? playerPos.y - ballSpr.height : playerPos.y + playerSpr.height;
                }

            }

            // colisión con un bloque
            for (auto block : blockView) {
                auto& blockPos = paddleView.get<PositionComponent>(block);
                auto& blocksprite = paddleView.get<SpriteComponent>(block);

                if (checkCollision(ballPos, ballSpr, blockPos, blocksprite)) {
                    scene->r.destroy(block);  // Destroy block on collision
                    ballVel.y *= -1;
                }

            }

        }

        if (blockView.size_hint() == 0) {
            gameState.gameWon = true;
        }

    }

    // función que checkea las collisiones
    bool checkCollision(const PositionComponent& posA, const SpriteComponent& sprA,
        const PositionComponent& posB, const SpriteComponent& sprB) {
        return (posA.x < posB.x + sprB.width &&
            posA.x + sprA.width > posB.x &&
            posA.y < posB.y + sprB.height &&
            posA.y + sprA.height > posB.y);
    }
};

class PaddleMovementSystem : public UpdateSystem {
    void run(float dT) {

        auto view = scene->r.view<PositionComponent, SpriteComponent>();
        auto paddleView = scene->r.view<PaddleComponent, PositionComponent, VelocityComponent, SpriteComponent>();

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


class GameStateSystem : public UpdateSystem {
    void run(float dt) override {
        auto gameStateView = scene->r.view<GameStateComponent>();

        for (auto entity : gameStateView) {
            auto& gameState = gameStateView.get<GameStateComponent>(entity);

            if (gameState.gameOver) {
                std::printf("Game over, F");

            }

            if (gameState.gameWon) {
                std::printf("Game cleared, YAY");
                //scene->stop(); //
            }
        }
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
        sampleScene = new Scene("SAMPLE SCENE", r);
        addSetupSystem<EntitiesSpawnSetupSystem>(sampleScene);
        addUpdateSystem<MovementSystem>(sampleScene);
        addUpdateSystem<PaddleMovementSystem>(sampleScene);
        addUpdateSystem<WallHitSystem>(sampleScene);
        addUpdateSystem<CollisionSystem>(sampleScene);
        addRenderSystem<SquareRenderSystem>(sampleScene);
        addUpdateSystem<GameStateSystem>(sampleScene);

        setScene(sampleScene);
    }
};