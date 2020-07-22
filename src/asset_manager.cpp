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

#include <application.hpp>
#include <debug_tools.hpp>
#include <scene_manager.hpp>
#include <asset_manager.hpp>
#include <shader.hpp>
#include <scene.hpp>
#include <component_manager.hpp>
#include <components.hpp>

#include <mesh.hpp>

jeBegin

std::string	AssetManager::initDirectory_, AssetManager::assetDirectory_, 
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

	//jeCheckComponentRegistration(jeRegisterComponent(Light));
	//jeCheckComponentRegistration(jeRegisterComponent(Material));

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
	AssetManager::set_asset_directory("../default/default.json");

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
	JsonParser::read_file(assetDirectory_.c_str());

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

	// clear texture memory
	for (auto& m : meshMap_)
	{
		if (m.second)
			glDeleteTextures(1, &m.second->texture_);

		delete m.second;
		m.second = nullptr;
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

		// texture only for engine
		if (!strcmp(textureKey, "grid") && tMap == &textureMap_)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

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

	glBindTexture(GL_TEXTURE_2D, 0);
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

void AssetManager::set_initdata_directory(const char* dir) { initDirectory_.assign(dir); }

void AssetManager::set_asset_directory(const char* dir) { assetDirectory_.assign(dir); }

void AssetManager::set_scene_directory(const char* dir) { stateDirectory_.assign(dir); }

void AssetManager::set_archetype_directory(const char* dir) { archeDirectory_.assign(dir); }

jeEnd

