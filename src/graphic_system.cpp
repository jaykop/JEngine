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
#include <asset_manager.hpp>
#include <scene.hpp>
#include <camera.hpp>
#include <renderer.hpp>
#include <shader.hpp>

#include <vec2.hpp>
#include <mat4.hpp>
#include <colors.hpp>

jeBegin

Camera* GraphicSystem::mainCamera_ = nullptr;
GraphicSystem::Renderers GraphicSystem::renderers_;
GraphicSystem::Cameras GraphicSystem::cameras_;
vec4 GraphicSystem::backgroundColor = vec4::zero, GraphicSystem::screenColor = vec4::zero;
bool GraphicSystem::renderGrid = false;
GraphicSystem::Grid GraphicSystem::grid;

void GraphicSystem::set_camera(Camera* camera)
{
	mainCamera_ = camera;
}

Camera* GraphicSystem::get_camera()
{
	return mainCamera_;
}

void GraphicSystem::initialize() {

	if (!grid.texture_)
		grid.texture_ = AssetManager::get_texture("grid");

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

	// render grid
	if (renderGrid)
		render_grid();

	// update renderers
	for (auto& r : renderers_) {
		r->start_draw();
		r->draw(dt);
		r->end_draw();
	}
}

void GraphicSystem::close() {

	//for (auto& model : models_)
	//	models_->close();

	mainCamera_ = nullptr;
}

void GraphicSystem::render_grid()
{
	Shader* shader = GLManager::shader_[GLManager::GRID];
	shader->use();

	shader->set_matrix("m4_translate", mat4::translate(vec3::zero));
	shader->set_matrix("m4_scale", mat4::scale(vec3::one * grid.size));
	shader->set_matrix("m4_rotate", mat4::identity);
	shader->set_vec3("v3_color", grid.color);

	mat4 viewport;

	if (grid.prjType == Renderer::ProjectType::PERSPECTIVE) {

		viewport = mat4::look_at(mainCamera_->position, mainCamera_->target, mainCamera_->up_);

		mat4 perspective = mat4::perspective(
			mainCamera_->fovy, mainCamera_->aspect_,
			mainCamera_->near_, mainCamera_->far_);

		shader->set_matrix("m4_projection", perspective);
	}

	else {

		viewport = mat4::scale(GLManager::resScaler_);

		float right_ = GLManager::get_width() * .5f;
		float left_ = -right_;
		float top_ = GLManager::get_height() * .5f;
		float bottom_ = -top_;

		mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, mainCamera_->near_, mainCamera_->far_);
		shader->set_matrix("m4_projection", orthogonal);
	}

	// Send camera info to shader
	shader->set_matrix("m4_viewport", viewport);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(GLManager::quadVao_);
	glBindBuffer(GL_ARRAY_BUFFER, GLManager::quadVbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLManager::quadEbo_);
	glBindTexture(GL_TEXTURE_2D, grid.texture_);
	glDrawElements(GL_TRIANGLES, GLManager::quadIndicesSize_, GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

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
