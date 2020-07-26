/******************************************************************************/
/*!
\file   asset_manager.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/03(yy/mm/dd)

\description
Contains the methods of asset_manager class
*/
/******************************************************************************/
#include <thread>
#include <lodepng.h>
#include <stb_image.h>
#include <Importer.hpp>
#include <postprocess.h>

#include <application.hpp>
#include <debug_tools.hpp>
#include <scene_manager.hpp>
#include <asset_manager.hpp>
#include <shader.hpp>
#include <scene.hpp>
#include <component_manager.hpp>
#include <components.hpp>
#include <mesh.hpp>
#include <math_util.hpp>

jeBegin

std::string meshDir;
std::vector<Texture> texturesLoaded;

unsigned int TextureFromFile(const char* path, const std::string& directory);

std::string	AssetManager::initDirectory_, 
AssetManager::stateDirectory_, AssetManager::archeDirectory_;

MeshMap AssetManager::meshMap_; 
FontMap	AssetManager::fontMap_;
AudioMap AssetManager::audioMap_;
TextureMap AssetManager::textureMap_;
ArchetypeMap AssetManager::archetypeMap_;

bool AssetManager::set_bulit_in_components()
{
	jeRegisterComponent(Transform);

	// Graphic components
	// jeRegisterComponent(Renderer);
	jeRegisterComponent(Model);
	jeRegisterComponent(Sprite);
	jeRegisterComponent(Camera);
	jeRegisterComponent(Animation2D);
	jeRegisterComponent(DebugRenderer);
	jeRegisterComponent(Text);
	jeRegisterComponent(Emitter);
	jeRegisterComponent(Light);
	//jeRegisterComponent(Material);

	return false;
}

void AssetManager::load_shaders() {

	// raed shader directory
	JsonParser::read_file("../shader/shaders.json");

	const rapidjson::Value& vs = JsonParser::get_document()["vertex"];
	const rapidjson::Value& fs = JsonParser::get_document()["fragment"];
	const unsigned shader_size = vs.Size();
	for (rapidjson::SizeType i = 0; i < shader_size; ++i) {
		Shader::vsDirectory_.push_back(vs[i]["Directory"].GetString());
		Shader::fsDirectory_.push_back(fs[i]["Directory"].GetString());
	}
}

void AssetManager::load_assets()
{
	// Read scene info
	JsonParser::read_file(stateDirectory_.c_str());

	// Get sizes of them
	const rapidjson::Value& scenes = JsonParser::get_document()["Scene"];

	// Load scenes 
	for (rapidjson::SizeType i = 0; i < scenes.Size(); ++i) {
		SceneManager::register_scene(scenes[i]["Directory"].GetString(), scenes[i]["Key"].GetString());
		jeDebugPrint("Loaded %s\n", scenes[i]["Key"].GetString());
	}

	// Set first state
	const rapidjson::Value& fristStates = JsonParser::get_document()["FirstScene"];
	if (fristStates.GetString())
	{
		SceneManager::set_first_scene(fristStates.GetString());
		jeDebugPrint("The first scene is %s.\n", fristStates.GetString());
	}

	// Read asset info
	JsonParser::read_file("../default/default.json");

	// Load textures 
	const rapidjson::Value& textures = JsonParser::get_document()["Texture"];
	for (rapidjson::SizeType i = 0; i < textures.Size(); ++i) {
		load_texture(textures[i]["Directory"].GetString(), textures[i]["Key"].GetString());
		jeDebugPrint("*AssetManager - Loaded image: %s.\n", textures[i]["Directory"].GetString());
	}

	// Load obj 
	const rapidjson::Value& models = JsonParser::get_document()["Model"];
	for (rapidjson::SizeType i = 0; i < models.Size(); ++i) {
		load_obj(models[i]["Directory"].GetString(), models[i]["Key"].GetString());
		jeDebugPrint("*AssetManager - Loaded obj: %s.\n", models[i]["Directory"].GetString());
	}

	// Load font
	const rapidjson::Value& fonts = JsonParser::get_document()["Font"];
	for (rapidjson::SizeType i = 0; i < fonts.Size(); ++i) {

		// Load default ascii characters (0 - 128)
		load_font(fonts[i]["Directory"].GetString(), 
			fonts[i]["Key"].GetString(), 
			fonts[i]["Size"].GetUint(),
			0, 128);

		// Load additional unicode set
		for (unsigned j = 0; j < fonts[i]["Additional"].Size(); ++j) {
			load_font(fonts[i]["Directory"].GetString(), 
				fonts[i]["Key"].GetString(), 
				fonts[i]["Size"].GetUint(),
				static_cast<unsigned long>(fonts[i]["Additional"][j][0].GetUint64()),
				static_cast<unsigned long>(fonts[i]["Additional"][j][1].GetUint64()));
		}
	}	

	JsonParser::clear_document();
}

void AssetManager::unload_assets()
{
	ComponentManager::clear_builders();

	// clear font memory
	for (auto& font : fontMap_)
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
	for (auto& tex : textureMap_)
	{
		if (tex.second)
			glDeleteTextures(1, &tex.second);
	}

	// clear model memory
	for (auto& ms : meshMap_)
	{
		for (auto& m : ms.second)
		{
			delete m;
			m = nullptr;
		}
	}

	meshMap_.clear();
	fontMap_.clear();
	textureMap_.clear();
	audioMap_.clear();
	archetypeMap_.clear();

}

void AssetManager::load_texture(const char* path, const char* textureKey, TextureMap* tMap)
{
	Image image;
	unsigned error = lodepng::decode(image.pixels, image.width, image.height, path);

	if (error)
		jeDebugPrint("!AssetManager - Decoder error %d / %s.\n", error, lodepng_error_text(error));

	else
	{
		// Enable the texture for OpenGL.
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &image.handle);
		glBindTexture(GL_TEXTURE_2D, image.handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, &image.pixels[0]);

		//// texture only for engine
		//if (!strcmp(textureKey, "grid") && tMap == &textureMap_)
		//{
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
		//	glGenerateMipmap(GL_TEXTURE_2D);
		//}

		//else
		//{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//}

		tMap->insert(TextureMap::value_type(
			textureKey, image.handle));
	}
}

void AssetManager::load_font(const char* path, const char* key, unsigned size,
	unsigned long start, unsigned long end, FontMap* fMap)
{
	// Set pointer to new font
	Font* newFont = nullptr;
	static bool s_existing = false;
	static float s_newLineLevel = 0;
	auto found = fMap->find(key);

	if (found != fMap->end()) {
		// There is existing font map
		s_existing = true;
		// Then get that one
		newFont = found->second;
		// Load the size of that font
		s_newLineLevel = newFont->newline;
	}

	else {

		// No existing font
		s_existing = false;
		// Then get a new font 
		newFont = new Font;

		// Init freetype
		if (FT_Init_FreeType(&newFont->lib))
			jeDebugPrint("!AssetManager - Could not init freetype library: %s\n", path);

		// Check freetype face init
		if (!FT_New_Face(newFont->lib, path, 0, &newFont->face))
			jeDebugPrint("*AssetManager - Loaded font: %s\n", path);
		else {
			jeDebugPrint("!AssetManager - Failed to load font: %s\n", path);
			return;
		}

		// Select unicode range
		FT_Select_Charmap(newFont->face, FT_ENCODING_UNICODE);
		// Set pixel size
		FT_Set_Pixel_Sizes(newFont->face, 0, size);
		// Set size of the font
		newFont->size = size;
		// Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	load_characters(newFont, s_newLineLevel, start, end);

	// If there is not existing font in the list,
	// add new one
	if (!s_existing) {
		newFont->newline = s_newLineLevel;
		fMap->insert(FontMap::value_type(key, newFont));
	}
}

void AssetManager::load_characters(Font* font, float& newLineLevel,
	unsigned long start, unsigned long end)
{
	// Load first 128 characters of ASCII set
	for (unsigned long c = start; c < end; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(font->face, c, FT_LOAD_RENDER))
		{
			jeDebugPrint("!AssetManager - Failed to load Glyph.\n");
			break;
		}

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			font->face->glyph->bitmap.width,
			font->face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			font->face->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		Character character = {
				texture, GLuint(font->face->glyph->advance.x),
				vec2(float(font->face->glyph->bitmap.width), float(font->face->glyph->bitmap.rows)),
				vec2(float(font->face->glyph->bitmap_left), float(font->face->glyph->bitmap_top))
		};
		if (newLineLevel < character.size.y)
			newLineLevel = character.size.y;
		font->data.insert(Font::FontData::value_type(c, character));
	}
}

bool AssetManager::load_obj(const std::string& path, const char* meshKey, MeshMap* mMap)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		jeDebugPrint("ERROR::ASSIMP:: %s\n", importer.GetErrorString());
		return false;
	}
	// retrieve the directory path of the filepath
	meshDir = path.substr(0, path.find_last_of('/'));
	std::vector<Mesh*> newMeshes;

	// process ASSIMP's root node recursively
	process_node(scene->mRootNode, scene, newMeshes);
	mMap->insert(MeshMap::value_type(meshKey, newMeshes));

	meshDir.clear();
	texturesLoaded.clear();
	return true;
}

void AssetManager::load_audio(const char* /*path*/, const char* /*_audioKey*/, AudioMap* /*aMap*/)
{
	// TODO
	// load audio assets
}

void AssetManager::load_archetype(const char* /*path*/, const char* /*_archetypeKey*/, ArchetypeMap* /*atMap*/)
{
	// TODO
	// load archetpye assets
}

void AssetManager::generate_screenshot(const char* directory)
{
	bool isFullscreen = Application::get_appdata().isFullscreen;

	// Get the total size of image
	unsigned width = 0, height = 0;
	width = isFullscreen ? unsigned(Application::get_appdata().displayWidth) : unsigned(Application::get_appdata().width);
	height = isFullscreen ? unsigned(Application::get_appdata().displayHeight) : unsigned(Application::get_appdata().height);
	unsigned size = width * height * 4;

	// Send the pixel info to the image vector
	std::vector<unsigned char> image;
	unsigned char* pixelChunk = new unsigned char[size];

	// Read pixel from window screen
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &pixelChunk[0]);

	// Invert the image vertucally
	for (unsigned y = 0; y < height / 2; y++)
		for (unsigned x = 0; x < width; x++)
		{
			unsigned index = 4 * (width * y + x);
			unsigned invertedInder = 4 * (width * (height - y - 1) + x);

			std::swap(pixelChunk[index + 0], pixelChunk[invertedInder + 0]);
			std::swap(pixelChunk[index + 1], pixelChunk[invertedInder + 1]);
			std::swap(pixelChunk[index + 2], pixelChunk[invertedInder + 2]);
			std::swap(pixelChunk[index + 3], pixelChunk[invertedInder + 3]);
		}

	// Check error
	unsigned error = lodepng::encode(image, pixelChunk, width, height);
	if (!error) {

		std::string fileName;
		if (directory)
			fileName.assign(directory);

		Time currentTimeInfo = Timer::get_current_time_info();

		fileName += std::to_string(currentTimeInfo.year);

		if (currentTimeInfo.month < 10)
			fileName += "0" + std::to_string(currentTimeInfo.month);
		else
			fileName += std::to_string(currentTimeInfo.month);

		if (currentTimeInfo.day < 10)
			fileName += "0" + std::to_string(currentTimeInfo.day);
		else
			fileName += std::to_string(currentTimeInfo.day);

		if (currentTimeInfo.hour < 10)
			fileName += "0" + std::to_string(currentTimeInfo.hour);
		else
			fileName += std::to_string(currentTimeInfo.hour);

		if (currentTimeInfo.minute < 10)
			fileName += "0" + std::to_string(currentTimeInfo.minute);
		else
			fileName += std::to_string(currentTimeInfo.minute);

		if (currentTimeInfo.second < 10)
			fileName += "0" + std::to_string(currentTimeInfo.second);
		else
			fileName += std::to_string(currentTimeInfo.second);

		fileName += ".png";

		lodepng::save_file(image, fileName);
		jeDebugPrint("*AssetManager - Generated screenshot image file : %s\n", fileName.c_str());
	}

	else
		jeDebugPrint("!AssetManager - Cannot export screenshot image : %i\n", error);


	delete[] pixelChunk;
	pixelChunk = nullptr;
}

Font* AssetManager::get_font(const char* key)
{
	auto found = fontMap_.find(key);
	if (found != fontMap_.end())
		return found->second;

	found = SceneManager::currentScene_->fonts_.find(key);
	if (found != SceneManager::currentScene_->fonts_.end())
		return found->second;

	jeDebugPrint("!AssetManager - Cannot find such name of font resource: %s.\n", key);
	return nullptr;
}

FMOD::Sound* AssetManager::get_audio(const char* key)
{
	auto found = audioMap_.find(key);
	if (found != audioMap_.end())
		return found->second;

	found = SceneManager::currentScene_->audios_.find(key);
	if (found != SceneManager::currentScene_->audios_.end())
		return found->second;

	jeDebugPrint("!AssetManager - Cannot find such name of audio resource: %s.\n", key);
	return nullptr;
}

unsigned AssetManager::get_texture(const char* key)
{
	auto found = textureMap_.find(key);
	if (found != textureMap_.end())
		return found->second;

	found = SceneManager::currentScene_->textures_.find(key);
	if (found != SceneManager::currentScene_->textures_.end())
		return found->second;

	jeDebugPrint("!AssetManager - Cannot find such name of texture resource: %s.\n", key);
	return 0;
}

Archetype* AssetManager::get_archetype(const char* key)
{
	auto found = archetypeMap_.find(key);
	if (found != archetypeMap_.end())
		return found->second;

	found = SceneManager::currentScene_->archetypes_.find(key);
	if (found != SceneManager::currentScene_->archetypes_.end())
		return found->second;

	jeDebugPrint("!AssetManager: Cannot find such name of archetype resource: %s.\n", key);
	return nullptr;
}

const std::vector<Mesh*>& AssetManager::get_meshes(const char* key)
{
	auto found = meshMap_.find(key);
	if (found != meshMap_.end())
		return found->second;

	found = SceneManager::currentScene_->meshes_.find(key);
	if (found != SceneManager::currentScene_->meshes_.end())
		return found->second;

	jeDebugPrint("!AssetManager: Cannot find such name of model resource: %s.\n", key);

	// return invalid meshes
	return found->second;
}

void AssetManager::set_initdata_directory(const char* dir) { initDirectory_.assign(dir); }

void AssetManager::set_scene_directory(const char* dir) { stateDirectory_.assign(dir); }

void AssetManager::set_archetype_directory(const char* dir) { archeDirectory_.assign(dir); }

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void AssetManager::process_node(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(process_mesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		process_node(node->mChildren[i], scene, meshes);
	}
}

Mesh* AssetManager::process_mesh(aiMesh* mesh, const aiScene* scene)
{
	vec3 minPoint(Math::max_float, Math::max_float, Math::max_float),
		maxPoint(Math::min_float, Math::min_float, Math::min_float),
		centerOffset;
	float absMax = 0.f;

	// data to fill
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		// check absolute min/max value from vertex
		if (vector.x < minPoint.x)
			minPoint.x = vector.x;
		else if (vector.x > maxPoint.x)
			maxPoint.x = vector.x;

		if (vector.y < minPoint.y)
			minPoint.y = vector.y;
		else if (vector.y > maxPoint.y)
			maxPoint.y = vector.y;

		if (vector.z < minPoint.z)
			minPoint.z = vector.z;
		else if (vector.z > maxPoint.z)
			maxPoint.z = vector.z;

		vertex.position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = vec2(0.0f, 0.0f);

		//// tangent
		//vector.x = mesh->mTangents[i].x;
		//vector.y = mesh->mTangents[i].y;
		//vector.z = mesh->mTangents[i].z;
		//vertex.Tangent = vector;
		//// bitangent
		//vector.x = mesh->mBitangents[i].x;
		//vector.y = mesh->mBitangents[i].y;
		//vector.z = mesh->mBitangents[i].z;
		//vertex.Bitangent = vector;

		vertices.emplace_back(vertex);
	}

	// normalize the scale and position
	vec3 sum = { maxPoint.x - minPoint.x,
		maxPoint.y - minPoint.y,
		maxPoint.z - minPoint.z };

	sum *= .5f;
	absMax = std::fmaxf(std::fmaxf(sum.x, sum.y), sum.z);

	maxPoint /= absMax;
	minPoint /= absMax;
	centerOffset = ((maxPoint + minPoint) * .5f);
	minPoint -= centerOffset;
	maxPoint -= centerOffset;

	// move to center and set unit scale
	for (auto& v : vertices)
	{
		v.position = v.position / absMax - centerOffset;
	}

	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.emplace_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = load_material_textures(material, aiTextureType_DIFFUSE, "gDiffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = load_material_textures(material, aiTextureType_SPECULAR, "gSpecular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. height maps
	std::vector<Texture> heightMaps = load_material_textures(material, aiTextureType_AMBIENT, "gAmbient");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	//// 4. normal maps
	//std::vector<Texture> normalMaps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
	//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	
	// return a mesh object created from the extracted mesh data
	return new Mesh(vertices, indices, textures);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<Texture> AssetManager::load_material_textures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < texturesLoaded.size(); j++)
		{
			if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.emplace_back(texturesLoaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), meshDir);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.emplace_back(texture);
			texturesLoaded.emplace_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}

	return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		switch (nrComponents)
		{
		case 1: format = GL_RED; break;
		case 3: format = GL_RGB; break;
		case 4:
		default:
			format = GL_RGBA;
			break;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		jeDebugPrint("Texture failed to load at path: %s\n", path);
		stbi_image_free(data);
	}

	return textureID;
}

jeEnd

