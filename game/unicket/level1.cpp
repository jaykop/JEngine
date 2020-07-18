#include "level1.h"
#include "game_logics.h"
#include "pch.h"

// components
#include "emitter.hpp"
#include "text.hpp"
#include "camera.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include "animation_2d.hpp"
#include "debug_renderer.hpp"

jeBegin

void Level1::load()
{
	// base load
	Scene::load();
}

void Level1::initialize()
{
	// base init
	Scene::initialize();
}

void Level1::update(float dt)
{
	// base update
	Scene::update(dt);
}

void Level1::close()
{
	// base close
	Scene::close();
}

void Level1::unload()
{
	// base unload
	Scene::unload();
}

jeEnd