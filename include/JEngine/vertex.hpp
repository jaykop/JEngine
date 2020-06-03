#pragma once
#include <macros.hpp>
#include <vec3.hpp>
#include <vec2.hpp>

jeBegin

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texCoords;
};

jeEnd