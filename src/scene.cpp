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
#include <asset_manager.hpp>

#include <sound_system.hpp>
#include <physics_system.hpp>
#include <graphic_system.hpp>
#include <behavior_system.hpp>

#include <text.hpp>
#include <mesh.hpp>

#include <glew.h>

jeBegin

void Scene::load()
{
	// Read asset info
	JsonParser::read_file(directory_);
	
	const rapidjson::Value& backgroundColor = JsonParser::get_document()["Background"];
	if (!backgroundColor.Empty())
	{
		background.set(backgroundColor[0].GetFloat(),
			backgroundColor[1].GetFloat(),
			backgroundColor[2].GetFloat(),
			backgroundColor[3].GetFloat());
	}

	// Load textures 
	const rapidjson::Value& textures = JsonParser::get_document()["Texture"];
	for (rapidjson::SizeType i = 0; i < textures.Size(); ++i) 
	{
		AssetManager::load_texture(textures[i]["Directory"].GetString(), textures[i]["Key"].GetString(),
			&textures_);
			jeDebugPrint("*AssetManager - Loaded image: %s.\n", textures[i]["Directory"].GetString());
	}
	
	const rapidjson::Value& skybox = JsonParser::get_document()["Skybox"];
	for (rapidjson::SizeType i = 0; i < skybox.Size(); ++i) {
		AssetManager::load_skybox(skybox[i]["Directory"].GetString(), skybox[i]["Key"].GetString(),
			&textures_);
		jeDebugPrint("*AssetManager - Loaded skybox: %s.\n", skybox[i]["Directory"].GetString());
	}

	// Load obj 
	const rapidjson::Value& models = JsonParser::get_document()["Model"];
	for (rapidjson::SizeType i = 0; i < models.Size(); ++i) {
		bool loaded = AssetManager::load_obj(models[i]["Directory"].GetString(), models[i]["Key"].GetString(), 
			&meshes_);
		if (loaded) jeDebugPrint("*AssetManager - Loaded obj: %s.\n", models[i]["Directory"].GetString());
	}

	// Load font
	// Read font info
	const rapidjson::Value& fonts = JsonParser::get_document()["Font"];
	for (rapidjson::SizeType i = 0; i < fonts.Size(); ++i) {

		// Load default ascii characters (0 - 128)
		AssetManager::load_font(fonts[i]["Directory"].GetString(),
			fonts[i]["Key"].GetString(),
			fonts[i]["Size"].GetUint(),
			0, 128, &fonts_);

		if (fonts[i].HasMember("Additional"))
		{
			// Load additional korean set
			for (unsigned j = 0; j < fonts[i]["Additional"].Size(); ++j) {
				AssetManager::load_font(fonts[i]["Directory"].GetString(),
					fonts[i]["Key"].GetString(),
					fonts[i]["Size"].GetUint(),
					static_cast<unsigned long>(fonts[i]["Additional"][j][0].GetUint64()),
					static_cast<unsigned long>(fonts[i]["Additional"][j][1].GetUint64()),
					&fonts_);
			}
		}
	}

	JsonParser::clear_document();

	// bind the objects to the manager
	ObjectManager::objects_ = &objects_;
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
	// unload all assets for current scene
	// clear font memory
	for (auto& font : fonts_)
	{
		if (font.second)
		{
			for (auto& c : font.second->data)
				glDeleteTextures(1, &c.second.texture);
		}

		delete font.second;
		font.second = nullptr;
	}

	// clear texture memory
	for (auto& tex : textures_)
	{
		if (tex.second)
			glDeleteTextures(1, &tex.second);
	}

	// clear model memory
	for (auto& ms : meshes_)
	{
		for (auto& m : ms.second)
		{
			delete m;
			m = nullptr;
		}
	}

	meshes_.clear();
	fonts_.clear();
	textures_.clear();
	audios_.clear();
	archetypes_.clear();

	// make sure current object map belongs to the current scene
	ObjectManager::objects_ = &objects_;
	ObjectManager::clear_objects();
	ObjectManager::objects_ = nullptr;
}

void Scene::resume()
{
	BehaviorSystem::resume();
	//SoundSystem::resume();
	//PhysicsSystem::resume();
	GraphicSystem::resume();
}

void Scene::pause()
{
	BehaviorSystem::pause();
	//SoundSystem::pause();
	//PhysicsSystem::pause();
	GraphicSystem::pause();
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