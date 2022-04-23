#include "JEngine.h"
#include "asset_manager.hpp"
#include "scene_manager.hpp"
#include "component_manager.hpp"
#include "game_logics.h"
#include "game_scenes.h"

using namespace JE;

void JEngine::register_builders()
{
	// add game logic builders here

	jeRegisterComponent(SceneController);
	jeRegisterComponent(ScriptController);
	jeRegisterComponent(EventController);
}

void JEngine::register_scenes()
{
	// add scene manually here
	SceneManager::register_scene<License>("resource/scene/license.json");
	SceneManager::register_scene<Pause>("resource/scene/pause.json");
	SceneManager::register_scene<Level2>("resource/scene/level2.json");
	SceneManager::register_scene<Level3>("resource/scene/level3.json");
	SceneManager::register_scene<Level7>("resource/scene/level7.json");
	SceneManager::register_scene<Level8>("resource/scene/level8.json");

	// set the first scene
	SceneManager::set_first_scene<Level2>();
}

bool JEngine::register_assets()
{
	// Get assets ready 
	AssetManager::set_initdata_directory("resource/register/initData.json");
	AssetManager::set_scene_directory("resource/register/scene.json");
	AssetManager::set_archetype_directory("resource/register/archetype.json");
	
	register_scenes();
	register_builders();

	return true;
}