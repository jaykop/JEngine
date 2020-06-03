#pragma once
#include <vec3.hpp>
#include <macros.hpp>

jeBegin

struct HalfEdge;
struct HalfEdgeVertex
{
	HalfEdgeVertex(void) : pos(vec3::zero), edge(nullptr) {}
	HalfEdgeVertex(const vec3& _pos) : pos(_pos), edge(nullptr) {}

	vec3 pos;
	HalfEdge* edge;
};

jeEnd