#pragma once

#include "Game.h"
#include "Scene.h"
#include "Entity.h"
#include "Systems.h"

#include <SDL.h>
#include <SDL_events.h>


// physics constants
const float PIXELS_TO_METERS = 0.01f;
const float GRAVITY = 9.8f;
const float JUMP_FORCE = 3.0f;
const float FORWARD_FORCE = 2.0f;