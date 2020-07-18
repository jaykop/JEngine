#include <graphic_system.hpp>
#include <gl_manager.hpp>
#include <math_util.hpp>
#include <mat4.hpp>

#include <shader.hpp>
#include <camera.hpp>
#include <renderer.hpp>

jeBegin

using namespace Math;

void GraphicSystem::update_pipelines(float /*dt*/)
{
	// LightSourcePipeline();

	//GLManager::shader_[GLManager::JE_SHADER_MODEL]->use();

	// Inform that there are lights
	// GLManager::shader_[GLManager::JE_SHADER_MODEL]->set_bool("boolean_light", !lights_.empty());

	// Sort orthogonal objects and perspective objects
	// sort_renderers();

	//for (auto r : renderers_) {

	//	// Emitter
	//	if ((r->is_ & Model::IS_EMITTER) == Model::IS_EMITTER)
	//		ParticlePipeline(static_cast<Emitter*>(r), dt);

	//	// Text
	//	else if ((r->is_ & Model::IS_TEXT) == Model::IS_TEXT)
	//		TextPipeline(static_cast<Text*>(r));

	//	// Normal models
	//	else
			// r->draw(r);
	//}
}


jeEnd