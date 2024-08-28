#include "Scene.h"
#include <print>
#include <entt/entt.hpp>
#include "Entity.h"
#include "Components.h"
#include "Systems.h"

Scene::Scene(const std::string& n, entt::registry& r)
	: name(n), r(r)
{
	std::printf("Init scene: {}\n", n);
}

Scene:: ~Scene() {
	setupSystems.clear();
	std::printf("Scene Destroyed: {}\n");
}

Entity* Scene::createEntity(const std::string& n) {
	Entity* entity = new Entity(r.create(), this);
	entity->addComponent<NameComponent>(n);
	return entity;
}

Entity* Scene::createEntity(
	const std::string& n, int x, int y
) {
	Entity* entity = new Entity(r.create(), this);

	entity->addComponent<NameComponent>(n);
	entity->addComponent<PositionComponent>(x, y);

	std::printf("Entity {%s} created\n", n.c_str());

	return entity;
}

void Scene::setup() {
	std::printf("Scene setting up!\n");

	for (auto sys : setupSystems) {
		sys->run();
	}
}

void Scene::update(float dT) {
	for (auto sys : updateSystems) {
		sys->run(dT);
	}
}

void Scene::processEvents(SDL_Event e) {
	for (auto sys : eventSystems) {
		sys->run(e);
	}
}

void Scene::render(SDL_Renderer* render) {
	for (auto sys : renderSystems) {
		sys->run(render);
	}
}