/******************************************************************************/
/*!
\file   application.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the methods of scene class
*/
/******************************************************************************/

#include <scene.hpp>
#include <object.hpp>
#include <object_manager.hpp>
#include <sound_system.hpp>
#include <physics_system.hpp>
#include <graphic_system.hpp>
#include <behavior_system.hpp>

jeBegin

void Scene::load()
{
	// bind the objects to the manager
	ObjectManager::objects_ = &objects_;

	// Here load from json file...
}

void Scene::initialize()
{
	BehaviorSystem::initialize();
	GraphicSystem::initialize();
	PhysicsSystem::initialize();
	SoundSystem::initialize();
}

void Scene::update(float dt)
{
	// update all systems
	BehaviorSystem::update(dt);
	SoundSystem::update(dt);
	PhysicsSystem::update(dt);
	GraphicSystem::update(dt);
}

void Scene::close()
{
	BehaviorSystem::close();
	SoundSystem::close();
	PhysicsSystem::close();
	GraphicSystem::close();
}

void Scene::unload()
{	
	// make sure current object map belongs to the current scene
	ObjectManager::objects_ = &objects_;

	// clear current object map
	ObjectManager::clear_objects();

	// initialize
	ObjectManager::objects_ = nullptr;
}

const char* Scene::get_name() const
{
	return name_;
}

void Scene::register_object(Object* obj) {

	// check if the object is pointing null
	if (ObjectManager::objects_) {
		ObjectManager::objects_->insert({ obj->get_name(), obj });
		obj->register_components();
	}
}

jeEnd