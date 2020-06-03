#pragma once
#include <macros.hpp>

jeBegin

struct Face;
struct HalfEdgeVertex;
struct HalfEdge
{
	HalfEdge(void)
		: vertex(nullptr)
		, face(nullptr)
		, next(nullptr)
		, twin(nullptr) {}

	HalfEdge* Prev(void) const
	{
		if (next)
			return next->next;

		return nullptr;
	}

	HalfEdgeVertex* vertex;
	Face* face;
	HalfEdge* next;
	HalfEdge* twin;

};

jeEnd