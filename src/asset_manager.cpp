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

#include <scene_manager.hpp>
#include <asset_manager.hpp>
#include <json_parser.hpp>
#include <debug_tools.hpp>
#include <gl_manager.hpp>
#include <shader.hpp>
#include <text.hpp>
#include <thread>
#include <lodepng.h>
#include <mesh.hpp>

// component list
#include <component_manager.hpp>
#include <transform.hpp>
#include <Renderer.hpp>
#include <Camera.hpp>

jeBegin

std::string	AssetManager::initDirectory_, AssetManager::assetDirectory_, 
AssetManager::stateDirectory_, AssetManager::archeDirectory_;

unsigned char* AssetManager::pixel_chunk = nullptr;
AssetManager::Images AssetManager::images_;

AssetManager::FontMap		AssetManager::fontMap_;
AssetManager::AudioMap		AssetManager::audioMap_;
AssetManager::SceneMap		AssetManager::sceneMap_;
AssetManager::TextureMap	AssetManager::textureMap_;
AssetManager::ArchetypeMap	AssetManager::archetypeMap_;

bool AssetManager::set_bulit_in_components()
{
	jeRegisterComponent(Transform);

	// Graphic components
	//jeCheckComponentRegistration(jeRegisterComponent(Text));
	jeRegisterComponent(Renderer);
	jeRegisterComponent(Camera);
	//jeCheckComponentRegistration(jeRegisterComponent(Emitter));
	//jeCheckComponentRegistration(jeRegisterComponent(Light));
	//jeCheckComponentRegistration(jeRegisterComponent(Material));
	//jeCheckComponentRegistration(jeRegisterComponent(Animation));

	return false;
}

void AssetManager::load_shaders() {

	// raed shader directory
	JsonParser::read_file("../../shader/shaders.json");

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
	const rapidjson::Value& scenes = JsonParser::get_document()["Scene"];

	//// Read asset info
	//JsonParser::read_file(assetDirectory_.c_str());
	//const rapidjson::Value& textures = JsonParser::get_document()["Texture"];

	//// Read font info
	//const rapidjson::Value& fonts = JsonParser::get_document()["Font"];

	// Get sizes of them
	unsigned sceneSize = scenes.Size();
		//textureSize = textures.Size(),
		//fontSize = fonts.Size();

	// Load scenes using thread
	for (rapidjson::SizeType i = 0; i < sceneSize; ++i) {
		std::thread stateLoader(&SceneManager::push_scene, scenes[i]["Directory"].GetString(), scenes[i]["Key"].GetString());
		stateLoader.join();
		jeDebugPrint("Loaded %s\n", scenes[i]["Key"].GetString());
	}

	// Set first state
	const rapidjson::Value& fristStates = JsonParser::get_document()["FirstScene"];
	std::string firstStateName = SceneManager::firstScene_.empty() ? fristStates.GetString() : SceneManager::firstScene_;
	SceneManager::set_first_scene(firstStateName.c_str());
	jeDebugPrint("The first scene is %s.\n", firstStateName.c_str());

	//// Load images using thread
	//for (rapidjson::SizeType i = 0; i < textureSize; ++i) {
	//	std::thread imageLoader(&LoadImage, textures[i]["Directory"].GetString(), textures[i]["Key"].GetString());
	//	imageLoader.join();
	//	jeDebugPrint("*AssetManager - Loaded image: %s.\n", textures[i]["Directory"].GetString());
	//}

	//// Register images to gpu
	//for (auto it : images_)
	//	RegisterImage(it.second, it.first.c_str());

	// Load font
	//for (rapidjson::SizeType i = 0; i < fontSize; ++i) {

	//	// Load default ascii characters (0 - 128)
	//	LoadFont(fonts[i]["Directory"].GetString(), fonts[i]["Key"].GetString(), fonts[i]["Size"].GetUint(),
	//		0, 128);

	//	// Load additional unicode set
	//	for (unsigned j = 0; j < fonts[i]["Additional"].Size(); ++j) {
	//		LoadFont(fonts[i]["Directory"].GetString(), fonts[i]["Key"].GetString(), fonts[i]["Size"].GetUint(),
	//			static_cast<unsigned long>(fonts[i]["Additional"][j][0].GetUint64()),
	//			static_cast<unsigned long>(fonts[i]["Additional"][j][1].GetUint64()));
	//	}
	//}
}

void AssetManager::unload_assets()
{
	ComponentManager::clear_builders();
}

void AssetManager::load_font(const char* path, const char* key, unsigned size,
	unsigned long start, unsigned long end)
{
	// Set pointer to new font
	Font* newFont = nullptr;
	static bool s_existing = false;
	static float s_newLineLevel = 0;
	auto found = fontMap_.find(key);

	if (found != fontMap_.end()) {
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
		if (bool a = !FT_New_Face(newFont->lib, path, 0, &newFont->face))
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
		fontMap_.insert(FontMap::value_type(key, newFont));
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

void AssetManager::load_audio(const char* /*path*/, const char* /*_audioKey*/)
{
	// TODO
	// load audio assets
}

void AssetManager::load_image(const char* path, const char* textureKey)
{
	Image image;
	unsigned error = lodepng::decode(image.pixels, image.width, image.height, path);

	if (error)
		jeDebugPrint("!AssetManager - Decoder error %d / %s.\n", error, lodepng_error_text(error));

	else
		images_.insert(Images::value_type(textureKey, image));
}

void AssetManager::register_image(Image& image, const char* textureKey)
{
	// Enable the texture for OpenGL.
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &image.handle);
	glBindTexture(GL_TEXTURE_2D, image.handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, &image.pixels[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	textureMap_.insert(TextureMap::value_type(
		textureKey, image.handle));
}

void AssetManager::load_archetype(const char* /*path*/, const char* /*_archetypeKey*/)
{
	// TODO
	// load archetpye assets
}

Mesh* AssetManager::load_object(const char* /*path*/)
{
	// todo!
	return nullptr;
}

void AssetManager::generate_screenshot(const char* directory)
{
	// Get the total size of image
	unsigned width = unsigned(GLManager::get_width()), 
		height = unsigned(GLManager::get_height()), 
		size = width * height * 4;

	// Send the pixel info to the image vector
	std::vector<unsigned char> image;
	pixel_chunk = new unsigned char[size];

	// Read pixel from window screen
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &pixel_chunk[0]);

	// Invert the image vertucally
	for (unsigned y = 0; y < height / 2; y++)
		for (unsigned x = 0; x < width; x++)
		{
			unsigned index = 4 * (width * y + x);
			unsigned invertedInder = 4 * (width * (height - y - 1) + x);

			std::swap(pixel_chunk[index + 0], pixel_chunk[invertedInder + 0]);
			std::swap(pixel_chunk[index + 1], pixel_chunk[invertedInder + 1]);
			std::swap(pixel_chunk[index + 2], pixel_chunk[invertedInder + 2]);
			std::swap(pixel_chunk[index + 3], pixel_chunk[invertedInder + 3]);
		}

	// Check error
	unsigned error = lodepng::encode(image, pixel_chunk, width, height);
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


	delete[] pixel_chunk;
	pixel_chunk = nullptr;
}

Font* AssetManager::get_font(const char* key)
{
	auto found = fontMap_.find(key);
	if (found != fontMap_.end())
		return found->second;

	jeDebugPrint("!AssetManager - Cannot find such name of font resource: %s.\n", key);
	return nullptr;
}

Scene* AssetManager::get_scene(const char* key)
{
	auto found = sceneMap_.find(key);
	if (found != sceneMap_.end())
		return found->second;

	jeDebugPrint("!AssetManager - Cannot find such name of state resource: %s.\n", key);
	return nullptr;
}

Audio* AssetManager::get_audio(const char* key)
{
	auto found = audioMap_.find(key);
	if (found != audioMap_.end())
		return found->second;

	jeDebugPrint("!AssetManager - Cannot find such name of audio resource: %s.\n", key);
	return nullptr;
}

unsigned AssetManager::get_texture(const char* key)
{
	auto found = textureMap_.find(key);
	if (found != textureMap_.end())
		return found->second;

	jeDebugPrint("!AssetManager - Cannot find such name of texture resource: %s.\n", key);
	return 0;
}

Archetype* AssetManager::get_archetype(const char* key)
{
	auto found = archetypeMap_.find(key);
	if (found != archetypeMap_.end())
		return found->second;

	jeDebugPrint("!AssetManager: Cannot find such name of archetype resource: %s.\n", key);
	return nullptr;
}

void AssetManager::set_initdata_directory(const char* dir) { initDirectory_.assign(dir); }

void AssetManager::set_asset_directory(const char* dir) { assetDirectory_.assign(dir); }

void AssetManager::set_scene_directory(const char* dir) { stateDirectory_.assign(dir); }

void AssetManager::set_archetype_directory(const char* dir) { archeDirectory_.assign(dir); }

jeEnd

