/******************************************************************************/
/*!
\file   asset_manager.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/03(yy/mm/dd)

\description
Contains the definition of asset_manager class
*/
/******************************************************************************/

#pragma once
#include <vec3.hpp>
#include <assets.hpp>
#include <scene.h> // assimp

jeBegin

// generic asset manager class
class AssetManager {

	// Prevent to clone this class
	AssetManager() = delete;
	~AssetManager() = delete;

	jePreventClone(AssetManager);

	friend class Scene;
	friend class GraphicSystem;
	friend class Application;

public:

	static void generate_screenshot(const char* directory);

	static void	set_initdata_directory(const char* dir);
	static void	set_asset_directory(const char* dir);
	static void	set_scene_directory(const char* dir);
	static void	set_archetype_directory(const char* dir);

	static Font* get_font(const char* key);
	static FMOD::Sound* get_audio(const char* key);
	static unsigned	get_texture(const char* key);
	static Archetype* get_archetype(const char* key);
	static const std::vector<Mesh*>& get_meshes(const char* key);

private:

	static bool set_bulit_in_components();
	static void load_assets();
	static void unload_assets();

	// asset loaders
	static void load_shaders();
	static void load_audio(const char* path, const char* audioKey, 
		AudioMap* aMap = &audioMap_);
	static void load_archetype(const char* path, const char* archetypeKey,
		ArchetypeMap* atMap = &archetypeMap_);
	static void load_texture(const char* path, const char* textureKey, 
		TextureMap* tMap = &textureMap_);

	// font loader
	static void load_font(const char* path, const char* audioKey, unsigned size,
		unsigned long start, unsigned long end,
		FontMap* fMap = &fontMap_);
	static void load_characters(Font* font, float& newLineLevel, unsigned long start, unsigned long end);

	// obj loader
	static bool load_obj(const std::string& path, const char* meshKey,
		MeshMap* mMap = &meshMap_);
	static void load_model(const std::string& path);
	static void process_node(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes, const std::string& directory, std::vector<Texture>& texturesLoaded);
	static Mesh* process_mesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, std::vector<Texture>& texturesLoaded);
	static std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const std::string directory, std::vector<Texture>& texturesLoaded);
	
	static MeshMap meshMap_;
	static FontMap fontMap_;
	static AudioMap	audioMap_;
	static TextureMap textureMap_;
	static ArchetypeMap	archetypeMap_;
	static std::string initDirectory_, assetDirectory_,
		stateDirectory_, archeDirectory_;
};

jeEnd