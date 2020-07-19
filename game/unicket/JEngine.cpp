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
	// add scene manully here
	SceneManager::add_scene(new License("License", "resource/scene/license.json"));
	SceneManager::add_scene(new Level1("Level1", "resource/scene/level1.json"));
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
