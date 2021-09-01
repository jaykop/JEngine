#pragma once
#include <macros.hpp>

jeBegin

struct Material {

public:

	float friction = .2f;
	float restitution = .3f;
	float staticFriction = .4f;
	float separation = .5f;
};

jeEnd