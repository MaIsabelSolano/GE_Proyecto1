#pragma once
#include "Scene.h"
#include <print>
#include <entt/entt.hpp>
#include <utility>
#include "Components.h"

class Entity {
public:
	Entity(entt::entity h, Scene* s)
		: handle(h), scene(s)
	{
		std::printf("Entity Created\n");
	}

	~Entity() {
		// scene es un puntero
		scene->r.destroy(handle);
		std::printf("Entity Destroyed\n");
	}

	template<typename T>
	void addComponent(auto&&... args) {
		scene->r.emplace<T>(handle, std::forward<decltype(args)>(args)...);
	}

private:
	entt::entity handle;
	Scene* scene;
};