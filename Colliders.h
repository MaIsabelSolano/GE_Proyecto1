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

