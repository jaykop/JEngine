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

using FontMap = std::unordered_map<std::string, Font*>;
using ObjectMap = std::unordered_map<std::string, Object*>;
using TextureMap = std::unordered_map<std::string, unsigned>;
using AudioMap = std::unordered_map<std::string, FMOD::Sound*>;
using ArchetypeMap = std::unordered_map<std::string, Archetype*>;
using MeshMap = std::unordered_map<std::string, std::vector<Mesh*>>;

jeEnd