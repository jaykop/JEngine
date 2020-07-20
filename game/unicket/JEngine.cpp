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
	jeRegisterComponent(TopDownController);
	jeRegisterComponent(ExplodingParticleController);
	jeRegisterComponent(CameraController);
}

void JEngine::register_scenes()
{
	// add scene manually here
	SceneManager::push_scene(new License("license", "resource/scene/license.json"));
	SceneManager::push_scene(new Pause("pause", "resource/scene/pause.json"));
	SceneManager::push_scene(new Level1("level1", "resource/scene/level1.json"));
	SceneManager::push_scene(new Level2("level2", "resource/scene/level2.json"));
	SceneManager::push_scene(new Level3("level3", "resource/scene/level3.json"));

	//
	SceneManager::set_first_scene("level1");
}

bool JEngine::register_assets()
{
	// Get assets ready 
	AssetManager::set_initdata_directory("resource/register/initData.json");
	AssetManager::set_scene_directory("resource/register/scene.json");
	AssetManager::set_asset_directory("resource/register/asset.json");
	AssetManager::set_archetype_directory("resource/register/archetype.json");
	
	register_scenes();
	register_builders();

	return true;
}

void JEngine::run()
{
	Application::run();
}
