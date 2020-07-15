#include <iostream>
#include "application.hpp"
#include "particle_conrtoller.h"

#include "emitter.hpp"

jeBegin

jeDefineUserComponentBuilder(ExplodingParticleController);

void ExplodingParticleController::init() { }

void ExplodingParticleController::update(float /*dt*/)
{
	if (InputHandler::key_triggered(KEY::SPACE))
	{
		Emitter* explosion = get_owner()->get_component<Emitter>();
		explosion->refresh_particles();
	}
}

void ExplodingParticleController::close() { }

jeEnd