#include "application.hpp"
#include "particle_conrtoller.h"
#include "emitter.hpp"

jeBegin

jeDefineUserComponentBuilder(ExplodingParticleController);

ExplodingParticleController::~ExplodingParticleController()
{
	remove_from_system();
}

void ExplodingParticleController::init() { }

void ExplodingParticleController::update(float /*dt*/)
{
	if (InputHandler::key_triggered(KEY::ENTER))
	{
		Emitter* explosion = get_owner()->get_component<Emitter>();
		explosion->refresh_particles();
		explosion->active = true;
	}
}

void ExplodingParticleController::close() { }

jeEnd