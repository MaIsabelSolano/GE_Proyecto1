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

//class TilemapColliderSetupSystem : public SetupSystem {
//public: 
//	void run() override {
//		auto view = scene->r.view<TilemapComponent>();
//
//		for (auto entity : view) {
//			auto tilemap = view.get<TilemapComponent>(entity);
//
//			for (int y = 0; y < tilemap.map.size(); y++) {
//				for (int x = 0; x < tilemap.map[y].size(); x++) {
//					
//				}
//			}
//		}
//	}
//};

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
				}
			}
		}
	}
};

