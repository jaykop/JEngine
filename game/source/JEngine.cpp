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
	jeRegisterComponent(EnvironmentSetter);

	// player game logics
	jeRegisterComponent(PlayerController);
	jeRegisterComponent(PlayerAttack);
	jeRegisterComponent(BulletLogic);
	jeRegisterComponent(CameraController);
	jeRegisterComponent(TopDownController);
}

void JEngine::register_scenes()
{
	// add scene manually here
	SceneManager::register_scene<License>("resource/scene/license.json");
	SceneManager::register_scene<Pause>("resource/scene/pause.json");
	SceneManager::register_scene<Level1>("resource/scene/level1.json");
	SceneManager::register_scene<Level2>("resource/scene/level2.json");
	SceneManager::register_scene<Level3>("resource/scene/level3.json");
	SceneManager::register_scene<Level4>("resource/scene/level4.json");
	SceneManager::register_scene<Level5>("resource/scene/level5.json");
	SceneManager::register_scene<Level6>("resource/scene/level6.json");
	SceneManager::register_scene<Level7>("resource/scene/level7.json");
	SceneManager::register_scene<Level8>("resource/scene/level8.json");
	SceneManager::register_scene<Level9>("resource/scene/level9.json");

	// set the first scene
	SceneManager::set_first_scene<Level6>();
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