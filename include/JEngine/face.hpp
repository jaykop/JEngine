#pragma once
#include <macros.hpp>

jeBegin

struct HalfEdge;
struct Face
{
	Face(void) : edge(nullptr) {}
	HalfEdge* edge;
};

jeEnd