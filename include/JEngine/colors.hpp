#pragma once
#include <vec3.hpp>

namespace Color {

	static const vec3 red = vec3(1.f, 0.f, 0.f);
	static const vec3 blue = vec3(0.f, 0.f, 1.f);
	static const vec3 white = vec3(1.f, 1.f, 1.f);
	static const vec3 green = vec3(0, 1.f, 0);
	static const vec3 yellow = vec3(1.f, 1.f, 0);

	static vec3 colors_list[] = {
		vec3(1, 0, 0),
		vec3(1, 0.5f, 0.5f),
		vec3(0, 1, 1),
		vec3(0, 0, 1),
		vec3(1, 0.25f, 0.25f),
		vec3(1, 0, 1),
		vec3(1, 0.5f, 1),
		vec3(0, 1, 0),
		vec3(0, 1, 1),
		vec3(1, 1, 1),
		vec3(0.5f, 1, .3f),
		vec3(1, 1, 0),
		vec3(0.3f, 0.7f, 0.3f),
		vec3(0.7f, 0.2f, 0.7f),
		vec3(0.25f, 1, 0.25f),
		vec3(0.69f, 0, 0.69f),
		vec3(0.1f, 0.4f, 0.9f),
		vec3(0.25f, 0.25f, 1.f),
	};
};