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

	// basic game logics
	jeRegisterComponent(SceneController);
	jeRegisterComponent(ExplodingParticleController);
	jeRegisterComponent(ScriptController);

	// player game logics
	jeRegisterComponent(PlayerController);
	jeRegisterComponent(PlayerAttack);
	jeRegisterComponent(BulletLogic);
}

void JEngine::register_scenes()
{
	// add scene manually here
	SceneManager::register_scene<License>("resource/scene/license.json");
	SceneManager::register_scene<Pause>("resource/scene/pause.json");
	SceneManager::register_scene<Level1>("resource/scene/level1.json");
	SceneManager::register_scene<Level2>("resource/scene/level2.json");
	SceneManager::register_scene<Level3>("resource/scene/level3.json");

	// set the first scene
	SceneManager::set_first_scene<Level1>();
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