#pragma once

#include <SDL.h>
#include <iostream>
#include "Systems.h"
#include "Components.h"


enum class CollisionType {
	NONE,
	SPIKE,
	BOOST
};


struct BoxColliderComponent {
	SDL_Rect rect;
	SDL_Color color;
	CollisionType collisionType = CollisionType::NONE;
	bool isTriggered = false;
};


class ColliderResetSystem : public UpdateSystem {
public:
	void run(float dT) override {
		auto view = scene->r.view<BoxColliderComponent>();

		for (auto entity : view) {
			auto& collider = view.get<BoxColliderComponent>(entity);

			collider.collisionType = CollisionType::NONE;
		}
	}
};

class ColliderRenderSystem : public RenderSystem {
public:
	void run(SDL_Renderer* renderer) override {
		auto view = scene->r.view<PositionComponent, BoxColliderComponent>();

		for (auto entity : view) {
			auto [position, collider] = view.get<PositionComponent, BoxColliderComponent>(entity);

			SDL_Rect renderRect = {
			  position.x + collider.rect.x ,
			  position.y + collider.rect.y,
			  collider.rect.w,
			  collider.rect.h,
			};

			SDL_SetRenderDrawColor(renderer, collider.color.r, collider.color.g, collider.color.b, collider.color.a);
			SDL_RenderDrawRect(renderer, &renderRect);
		}

	}
};

class TilemapColliderSetupSystem : public SetupSystem {
public:
	void run() override {
		auto view = scene->r.view<TilemapComponent>();

		for (auto entity : view) {
			auto tilemap = view.get<TilemapComponent>(entity);

			for (int y = 0; y < tilemap.height; y++) {
				for (int x = 0; x < tilemap.width; x++) {
					int index = y * tilemap.width + x;

					const Tile& tile = tilemap.tiles[index];
					createTileEntity(x, y, tilemap.tileSize * tilemap.scale, tile);
				}
			}
		}
	}

public:
	void createTileEntity(int x, int y, int size, Tile tile) {
		Entity* tileEntity = scene->createEntity("TILE");
		tileEntity->addComponent<TileColliderComponent>();

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
		/*else if (tile.type == TileType::NONE) {
			SDL_Rect colliderRect = { 0, 0, size, size };
			SDL_Color color = { 255, 255, 255 };
			tileEntity->addComponent<BoxColliderComponent>(colliderRect, color);
		}*/
	}
};

class PlayerPowerUpCollisionSystem : public UpdateSystem {
public:
	void run(float dT) override {
		auto playerView = scene->r.view<PlayerComponent, BoxColliderComponent, PositionComponent>();

		for (auto player : playerView) {
			auto& collider = playerView.get<BoxColliderComponent>(player);

			if (collider.collisionType == CollisionType::BOOST && !collider.isTriggered) {
				std::printf("Boost!");
				collider.isTriggered = true;
			}
		}
	}
};

class PlayerPowerUpCollisionDetectionSystem : public UpdateSystem {
public:
	void run(float dT) override {
		auto playerView = scene->r.view<PlayerComponent, BoxColliderComponent, PositionComponent>();
		auto powerUpView = scene->r.view<PowerUpComponent, BoxColliderComponent, PositionComponent>();

		for (auto player : playerView) {
			auto position = playerView.get<PositionComponent>(player);
			auto& collider = playerView.get<BoxColliderComponent>(player);

			SDL_Rect playerRect = {
			position.x + collider.rect.x,
			position.y + collider.rect.y,
			collider.rect.w,
			collider.rect.h,
			};

			for (auto powerUp : powerUpView) {
				auto [pposition, pcollider] = powerUpView.get<PositionComponent, BoxColliderComponent>(powerUp);

				SDL_Rect powerUpRect = {
				  pposition.x + pcollider.rect.x,
				  pposition.y + pcollider.rect.y,
				  pcollider.rect.w,
				  pcollider.rect.h,
				};

				if (SDL_HasIntersection(&playerRect, &powerUpRect)) {
					collider.collisionType = CollisionType::BOOST;
				}
			}
		}
	}
};


class PlayerTileCollisionDetectionSystem : public UpdateSystem {
public:
	void run(float dT) override {
		auto playerView = scene->r.view<PlayerComponent, BoxColliderComponent, PositionComponent, VelocityComponent>();
		auto tileView = scene->r.view<TileComponent, BoxColliderComponent, PositionComponent>();

		for (auto player : playerView) {
			auto position = playerView.get<PositionComponent>(player);
			auto velocity = playerView.get<VelocityComponent>(player);
			auto& collider = playerView.get<BoxColliderComponent>(player);

			int newPlayerX = position.x + velocity.x * dT;
			int newPlayerY = position.y + velocity.y * dT;

			SDL_Rect playerRect = {
			  newPlayerX + collider.rect.x,
			  newPlayerY + collider.rect.y,
			  collider.rect.w,
			  collider.rect.h,
			};

			for (auto tile : tileView) {
				auto [tposition, tcollider] = tileView.get<PositionComponent, BoxColliderComponent>(tile);

				SDL_Rect tileRect = {
				  tposition.x + tcollider.rect.x,
				  tposition.y + tcollider.rect.y,
				  tcollider.rect.w,
				  tcollider.rect.h,
				};

				if (SDL_HasIntersection(&playerRect, &tileRect)) {
					collider.collisionType = CollisionType::SPIKE;
					std::printf("collision!\n");
				}
			}
		}
	}
};

