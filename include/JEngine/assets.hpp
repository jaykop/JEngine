#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include <macros.hpp>

jeBegin

struct Image {
	std::vector<unsigned char> pixels;
	unsigned handle = 0, width = 0, height = 0;
};

struct Texture {
	unsigned id = 0;
	std::string type, path;
};

struct Font;

class Mesh;
class Audio;
class Scene;
class Object;
class Archetype;

using ObjectMap = std::unordered_map<std::string_view, Object*>;
using MeshMap = std::unordered_map<std::string_view, Mesh*>;
using FontMap = std::unordered_map<std::string_view, Font*>;
using AudioMap = std::unordered_map<std::string_view, Audio*>;
using SceneMap = std::unordered_map<std::string_view, Scene*>;
using TextureMap = std::unordered_map<std::string_view, unsigned>;
using ArchetypeMap = std::unordered_map<std::string_view, Archetype*>;
using Images = std::unordered_map<const char*, Image>;

jeEnd