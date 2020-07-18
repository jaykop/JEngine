#pragma once
#include <vec3.hpp>
#include <renderer.hpp>
#include <graphic_system.hpp>

struct Grid
{
	vec3 scale, color;
	unsigned size;
	Renderer::ProjectType prjType;

private:

	friend class GraphicSystem;

	unsigned texture;

	Grid();
	~Grid();
};