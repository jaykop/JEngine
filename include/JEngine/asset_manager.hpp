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

jeBegin

// class HalfEdgeMesh;

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
	static Scene* get_scene(const char* key);
	static Audio* get_audio(const char* key);
	static unsigned	get_texture(const char* key);
	static Archetype* get_archetype(const char* key);

private:

	static void load_font(const char* path, const char* audioKey, unsigned size,
		unsigned long start, unsigned long end, 
		FontMap* fMap = &fontMap_);
	static void load_audio(const char* path, const char* audioKey, 
		AudioMap* aMap = &audioMap_);
	static void load_archetype(const char* path, const char* archetypeKey,
		ArchetypeMap* atMap = &archetypeMap_);
	static void load_image(const char* path, const char* textureKey,
		Images* img = &images_);
	static void register_image(Image& image, const char* textureKey, TextureMap* tMap = &textureMap_);

	static Mesh* load_object(const char* path);

	static void load_shaders();
	static void load_characters(Font* font, float& newLineLevel, unsigned long start, unsigned long end);
	
	static bool set_bulit_in_components();
	static void load_assets();
	static void unload_assets();

	static unsigned char* pixel_chunk;
	static Images images_;

	static FontMap fontMap_;
	static AudioMap	audioMap_;
	static SceneMap	sceneMap_;
	static TextureMap textureMap_;
	static ArchetypeMap	archetypeMap_;

	static std::string initDirectory_, assetDirectory_,
		stateDirectory_, archeDirectory_;

public:

	//static bool load_obj(const char* path);
	//static Mesh* get_mesh(const char* name);
	static std::string parse_name(const char* name);
	static void clear_meshes();

	static MeshMap meshMap_;
	static vec3 maxPoint, minPoint;

private:

	static void update_max_min(const vec3& v);
	static void convert_mesh(Mesh** mesh);

	static void parse_vertex(const std::string& data, Mesh** pMesh);
	static void read_vertex(const std::string& file_data, unsigned pos, std::vector<vec3>& vertices);
	static void read_face(const std::string& file_data, unsigned pos, std::vector<unsigned>& indice, unsigned vertice_size);
	static unsigned read_index(const char* data, unsigned vertice_size);
	static unsigned get_next_elements(const std::string& file_data, unsigned pos);
	static void calculate_normals(Mesh** pMesh);

	static vec3 get_converted_position(const vec3& position, const vec3& centerOffset, float absMax);
};

jeEnd