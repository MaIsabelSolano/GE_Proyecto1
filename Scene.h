#pragma once
#pragma once

#include <string>
#include <entt/entt.hpp>
#include <vector>
#include <SDL.h>

class Entity;
class SetupSystem;
class EventSystem;
class RenderSystem;
class UpdateSystem;

class Scene {
public:
	std::vector<SetupSystem*> setupSystems;
	std::vector<EventSystem*> eventSystems;
	std::vector<RenderSystem*> renderSystems;
	std::vector<UpdateSystem*> updateSystems;

	Scene(const std::string&, entt::registry&);
	~Scene();

	Entity* createEntity(const std::string&);
	Entity* createEntity(const std::string&, int, int);

	void setup();
	void update(float dT);
	void render(SDL_Renderer* renderer);
	void processEvents(SDL_Event e);

	entt::registry& r;
	std::string name;
};