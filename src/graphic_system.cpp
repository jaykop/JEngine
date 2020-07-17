/******************************************************************************/
/*!
\file   graphic_system.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the methods of GraphicSystem class
*/
/******************************************************************************/

#include <glew.h>
#include <graphic_system.hpp>
#include <scene_manager.hpp>
#include <gl_manager.hpp>
#include <scene.hpp>
#include <camera.hpp>
#include <renderer.hpp>

#include <mat4.hpp>

jeBegin

const vec3 stdResolution(1.f / 800.f, 1.f / 600.f, 1.f);
vec3 resolutionScaler_;

Camera* GraphicSystem::mainCamera_ = nullptr;
GraphicSystem::Renderers GraphicSystem::renderers_;
GraphicSystem::Cameras GraphicSystem::cameras_;
vec4 GraphicSystem::backgroundColor = vec4::zero, GraphicSystem::screenColor = vec4::zero;
bool GraphicSystem::renderGrid = false;

void GraphicSystem::set_camera(Camera* camera)
{
	mainCamera_ = camera;
}

Camera* GraphicSystem::get_camera()
{
	return mainCamera_;
}

void GraphicSystem::initialize() {

	if (!mainCamera_ && !(cameras_.empty()))
		mainCamera_ = *cameras_.begin();

	//for (auto& model : models_)
	//	model->initialize();
}

void GraphicSystem::update(float dt) {

	// get current scene color
	backgroundColor = SceneManager::get_current_scene()->background;

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(
		backgroundColor.r,
		backgroundColor.g,
		backgroundColor.b,
		backgroundColor.a);

	// Update the projection size by window screen size
	vec3 windowSize(GLManager::get_width(), GLManager::get_height(), 1.f);
	resolutionScaler_ = windowSize * stdResolution;

	// update renderers
	for (auto& r : renderers_) {
		r->start_draw(resolutionScaler_);
		r->draw(dt);
		r->end_draw();
	}

	if (renderGrid)
		render_grid();

}

void GraphicSystem::close() {

	//for (auto& model : models_)
	//	models_->close();

	mainCamera_ = nullptr;
}

void GraphicSystem::render_grid()
{
	//glEnable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	//glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, GLsizei(gridVertices.size()));

	//glDisable(GL_BLEND);
}

void GraphicSystem::add_renderer(Renderer* model) {
	renderers_.emplace_back(model);
}

void GraphicSystem::add_camera(Camera* camera) { 
	cameras_.emplace_back(camera);
}

void GraphicSystem::remove_renderer(Renderer* model) {
	renderers_.erase(std::remove(renderers_.begin(), renderers_.end(), model), renderers_.end());
}

void GraphicSystem::remove_camera(Camera* camera) {
	cameras_.erase(std::remove(cameras_.begin(), cameras_.end(), camera), cameras_.end());
}

jeEnd
