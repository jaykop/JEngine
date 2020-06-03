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
mat4 perspective_;

Camera* GraphicSystem::mainCamera_ = nullptr;
GraphicSystem::Renderers GraphicSystem::renderers_;
GraphicSystem::Cameras GraphicSystem::cameras_;
vec4 GraphicSystem::backgroundColor_ = vec4::zero, GraphicSystem::screenColor_ = vec4::zero;

void GraphicSystem::initialize() {

	if (!mainCamera_ && !(cameras_.empty()))
		mainCamera_ = *cameras_.begin();

	//for (auto& model : models_)
	//	model->initialize();
}

void GraphicSystem::update(float /*dt*/) {

	// get current scene color
	backgroundColor_ = SceneManager::get_current_scene()->background;

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(
		backgroundColor_.r, 
		backgroundColor_.g, 
		backgroundColor_.b, 
		backgroundColor_.a);

	// Update the projection size by window screen size
	vec3 windowSize(GLManager::get_width(), GLManager::get_height(), 1.f);
	resolutionScaler_ = windowSize * stdResolution;

	// Update the perpsective matrix by camera's zoom
	perspective_ = mat4::perspective(
		mainCamera_->fovy_, mainCamera_->aspect_,
		mainCamera_->near_, mainCamera_->far_);

	for (auto& r : renderers_) {
		
		r->draw(mainCamera_, perspective_, resolutionScaler_);
	}

}

void GraphicSystem::close() {

	//for (auto& model : models_)
	//	models_->close();

	mainCamera_ = nullptr;
}

void GraphicSystem::render()
{
}

void GraphicSystem::add_model(Renderer* model) {
	renderers_.emplace_back(model);
}

void GraphicSystem::add_camera(Camera* camera) { 
	cameras_.emplace_back(camera);
}

void GraphicSystem::remove_model(Renderer* model) {
	renderers_.erase(std::remove(renderers_.begin(), renderers_.end(), model), renderers_.end());
}

void GraphicSystem::remove_camera(Camera* camera) {
	cameras_.erase(std::remove(cameras_.begin(), cameras_.end(), camera), cameras_.end());
}

jeEnd
