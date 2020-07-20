#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include <macros.hpp>

namespace FMOD
{
	class Sound;
}

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
class Scene;
class Object;
class Archetype;

using ObjectMap = std::unordered_map<std::string, Object*>;
using MeshMap = std::unordered_map<std::string, Mesh*>;
using FontMap = std::unordered_map<std::string, Font*>;
using AudioMap = std::unordered_map<std::string, FMOD::Sound*>;
using SceneMap = std::unordered_map<std::string, Scene*>;
using TextureMap = std::unordered_map<std::string, unsigned>;
using ArchetypeMap = std::unordered_map<std::string, Archetype*>;

jeEnd