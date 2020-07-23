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
#include <asset_manager.hpp>
#include <scene.hpp>
#include <shader.hpp>
#include <math_util.hpp>
#include <mat4.hpp>
#include <colors.hpp>

#include <transform.hpp>
#include <camera.hpp>
#include <renderer.hpp>
#include <light.hpp>
#include <vertex.hpp>

jeBegin


const std::vector<float> quadVertices =
{
	-.5f, .5f, 0.0f , 0.f, 0.f, 1.f,  0.0f, 0.0f ,
	-.5f, -.5f, 0.0f , 0.f, 0.f, 1.f,  0.0f, 1.0f,
	.5f, -.5f, 0.0f , 0.f, 0.f, 1.f,  1.0f, 1.0f,
	.5f,  .5f, 0.0f , 0.f, 0.f, 1.f,  1.0f, 0.0f
};

const std::vector<unsigned> quadIndices = { 2, 0, 1, 2, 3, 0 };

const std::string type("mode"), position("position"), direction("direction"),
innerAngle("innerAngle"), outerAngle("outerAngle"), fallOff("fallOff"),
aColor("aColor"), sColor("sColor"), dColor("dColor"), activate("activate"), radius("radius");

vec3 GraphicSystem::resScaler_;
GraphicSystem::Shaders GraphicSystem::shader_;
int GraphicSystem::widthStart_ = 0, GraphicSystem::heightStart_ = 0;
float GraphicSystem::width_ = 0.f, GraphicSystem::height_ = 0.f;
unsigned GraphicSystem::quadVao_ = 0, GraphicSystem::quadVbo_ = 0, GraphicSystem::quadEbo_ = 0,
GraphicSystem::drVao_ = 0, GraphicSystem::drVbo_ = 0,
GraphicSystem::quadIndicesSize_ = 6, GraphicSystem::gridVerticeSize_ = 48;

std::stack<GraphicSystem::Graphic> GraphicSystem::graphicStack_;
Camera* GraphicSystem::mainCamera_ = nullptr;
GraphicSystem::Renderers GraphicSystem::renderers_;
GraphicSystem::Cameras GraphicSystem::cameras_;
GraphicSystem::Lights GraphicSystem::lights_;
vec4 GraphicSystem::backgroundColor = vec4::zero, GraphicSystem::screenColor = vec4::zero;
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

	// update main camera
	mainCamera_->update(dt);

	// get current scene color
	backgroundColor = SceneManager::get_current_scene()->background;

	// scissor out the letterbox area
	glEnable(GL_SCISSOR_TEST);
	glScissor(widthStart_, heightStart_,
		static_cast<GLsizei>(width_), static_cast<GLsizei>(height_));

	// cull out invisible face
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(
		backgroundColor.r,
		backgroundColor.g,
		backgroundColor.b,
		backgroundColor.a);

	// update the viewport
	glViewport(widthStart_, heightStart_,
		static_cast<GLsizei>(width_), static_cast<GLsizei>(height_));

	// render grid
	if (grid.render)
		render_grid();

	// update lights
	update_lights(dt);

	// update renderers
	for (auto& r : renderers_)
		r->draw(dt);

	glDisable(GL_SCISSOR_TEST);
}

void GraphicSystem::close() {

	renderers_.clear();
	mainCamera_ = nullptr;
}

void GraphicSystem::render_grid()
{
	Shader* shader = shader_[GRID];
	shader->use();

	shader->set_matrix("m4_translate", mat4::translate(vec3::zero));
	shader->set_matrix("m4_scale", mat4::scale(vec3::one * static_cast<float>(grid.size)));
	shader->set_matrix("m4_rotate", mat4::identity);
	shader->set_vec3("v3_color", grid.color);

	if (grid.prjType == Renderer::ProjectType::PERSPECTIVE) {

		mat4 perspective = mat4::perspective(
			mainCamera_->fovy_ + mainCamera_->zoom, mainCamera_->aspect_,
			mainCamera_->near_, mainCamera_->far_);

		shader->set_matrix("m4_projection", perspective);
	}

	else {

		float right_ = width_ * resScaler_.x;
		float left_ = -right_;
		float top_ = height_ * resScaler_.y;
		float bottom_ = -top_;

		mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, mainCamera_->near_, mainCamera_->far_);
		shader->set_matrix("m4_projection", orthogonal);
	}

	// Send camera info to shader
	mat4 viewport = mat4::look_at(mainCamera_->position, mainCamera_->position + mainCamera_->back_, mainCamera_->up_);
	shader->set_matrix("m4_viewport", viewport);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(quadVao_);
	glBindBuffer(GL_ARRAY_BUFFER, quadVbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEbo_);
	glBindTexture(GL_TEXTURE_2D, grid.texture_);
	glDrawElements(GL_TRIANGLES, quadIndicesSize_, GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void GraphicSystem::add_renderer(Renderer* model) 
{
	renderers_.emplace_back(model);
}

void GraphicSystem::add_camera(Camera* camera) 
{ 
	cameras_.emplace_back(camera);
}

void GraphicSystem::add_light(Light* light)
{
	lights_.emplace_back(light);
}

void GraphicSystem::remove_renderer(Renderer* model)
{
	renderers_.erase(std::remove(renderers_.begin(), renderers_.end(), model), renderers_.end());
}

void GraphicSystem::remove_camera(Camera* camera) 
{
	cameras_.erase(std::remove(cameras_.begin(), cameras_.end(), camera), cameras_.end());
}

void GraphicSystem::remove_light(Light* light)
{
	lights_.erase(std::remove(lights_.begin(), lights_.end(), light), lights_.end());
}

void GraphicSystem::pause()
{
	Graphic grp;
	grp.backgroundColor = backgroundColor;
	grp.screenColor = screenColor;
	grp.mainCamera = mainCamera_;
	grp.grid = grid;
	grp.renderers = renderers_;
	grp.cameras = cameras_;
	grp.lights = lights_;

	graphicStack_.emplace(grp);

	// 
	renderers_.clear();
	cameras_.clear();
	lights_.clear();
}

void GraphicSystem::resume()
{
	if (!graphicStack_.empty())
	{
		auto grp = graphicStack_.top();
		graphicStack_.pop();

		backgroundColor = grp.backgroundColor;
		screenColor = grp.screenColor;
		mainCamera_ = grp.mainCamera;
		grid = grp.grid;
		renderers_ = grp.renderers;
		cameras_ = grp.cameras;
		lights_ = grp.lights;
	}
}

void GraphicSystem::update_lights(float dt)
{
	Shader* shader = shader_[SPRITE];
	shader->use();
	shader->set_uint("uint_lightSize", static_cast<unsigned>(lights_.size()));
	
	for (unsigned i = 0; i < lights_.size() ; ++i)
	{
		shader = shader_[SPRITE];
		shader->use();

		// Update shader uniform info
		std::string str("light[" + std::to_string(i) + "].");
		shader->set_bool((str + activate).c_str(), lights_[i]->activate);

		// Set strings as a static
		if (lights_[i]->activate) {

			/*Calculate the light max and set the radius for light volume optimization*/
			// Calculate the light max
			float ambientMax = std::fmaxf(std::fmaxf(lights_[i]->ambient.x, lights_[i]->ambient.y),lights_[i]->ambient.z);
			float diffuseMax = std::fmaxf(std::fmaxf(lights_[i]->diffuse.x, lights_[i]->diffuse.y),lights_[i]->diffuse.z);
			float speculaMax = std::fmaxf(std::fmaxf(lights_[i]->specular.x, lights_[i]->specular.y), lights_[i]->specular.z);
			float lightMax = std::fmaxf(std::fmaxf(ambientMax, diffuseMax), speculaMax);

			// Get radius
			lights_[i]->radius = (-Light::linear 
				+ std::sqrtf(Light::linear * Light::linear - 4 * Light::quadratic 
					* (Light::constant - (256.f / 5.f) * lightMax))) * 0.5f * Light::quadratic;

			// Update light direction
			const vec3& pos = lights_[i]->transform_->position;
			lights_[i]->direction = (vec3::zero - pos);
			shader->set_vec3((str + type).c_str(), pos);

			shader->set_int((str + type).c_str(), static_cast<int>(lights_[i]->type));
			shader->set_vec3((str + type).c_str(), lights_[i]->direction);
			shader->set_vec3((str + aColor).c_str(), lights_[i]->ambient);
			shader->set_vec3((str + sColor).c_str(), lights_[i]->specular);
			shader->set_vec3((str + dColor).c_str(), lights_[i]->diffuse);
			shader->set_float((str + fallOff).c_str(), lights_[i]->fallOff);
			shader->set_float((str + radius).c_str(), lights_[i]->radius);
			shader->set_float((str + innerAngle).c_str(), Math::deg_to_rad(lights_[i]->innerAngle));
			shader->set_float((str + outerAngle).c_str(), Math::deg_to_rad(lights_[i]->outerAngle));

			lights_[i]->draw(dt);
		}
	}
}

void GraphicSystem::initialize_graphics()
{
	// init shaders
	// do shader stuff
	for (unsigned int i = 0; i < Pipeline::END; ++i) {

		Shader* newShader = new Shader;
		newShader->create_shader(Shader::vsDirectory_[i].c_str(), Shader::Type::VERTEX);

		// TODO
		// Work on geometry shader
		//shader_[i]->CreateShader(Shader::m_geometryShader[i], Shader::JE_GEOMETRY);

		newShader->create_shader(Shader::fsDirectory_[i].c_str(), Shader::Type::PIXEL);
		newShader->combine_shaders(i);

		shader_.push_back(newShader);
	}

	// remove shader directories
	Shader::vsDirectory_.clear();
	Shader::fsDirectory_.clear();

	jeDebugPrint("*Compiled and linked shaders.\n");

	// init buffers
	/**************************** QUAD BUFFER ******************************/
	// generate vertex array
	glGenVertexArrays(1, &quadVao_);
	glBindVertexArray(quadVao_);

	// generate vertex buffer
	glGenBuffers(1, &quadVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, quadVbo_);

	// set vertex data
	glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), &quadVertices[0], GL_STATIC_DRAW);

	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normals of vertices
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coordinate position
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// generate index buffer
	glGenBuffers(1, &quadEbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEbo_);

	// set index data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadIndices.size() * sizeof(unsigned), &quadIndices[0], GL_STATIC_DRAW);

	// unbind buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/**************************** DEBUG RENDERER BUFFER ******************************/

	glGenVertexArrays(1, &drVao_);
	glBindVertexArray(drVao_);

	glGenBuffers(1, &drVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, drVbo_);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::texCoords)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::color)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GraphicSystem::close_graphics()
{
	glDeleteVertexArrays(1, &quadVao_);
	glDeleteBuffers(1, &quadVbo_);
	glDeleteBuffers(1, &quadEbo_);

	glDeleteVertexArrays(1, &drVao_);
	glDeleteBuffers(1, &drVbo_);

	for (unsigned i = 0; i < Pipeline::END; ++i)
		delete shader_[i];

	shader_.clear();
}

float GraphicSystem::get_width() { return width_; }

float GraphicSystem::get_height() { return height_; }

//void GraphicSystem::RenderToFramebuffer() const
//{
//	// Render to framebuffer
//	glBindFramebuffer(GL_FRAMEBUFFER, GLM::fbo_);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClearColor(0.f, 0.f, 0.f, 0.f);
//	//glViewport(0, 0, GLint(width_), GLint(height_));
//
//	// Backface culling
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);
//}
//
//void GraphicSystem::RenderToScreen() const
//{
//	const static GLsizei sizeOfPlaneIndices
//		= static_cast<GLsizei>(GLM::targetMesh_[GLM::JE_TARGET_SCREEN]->GetIndiceCount());
//
//	// Bind default framebuffer and render to screen
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClearColor(backgroundColor_.x, backgroundColor_.y, backgroundColor_.z, backgroundColor_.w);
//
//	glDisable(GL_CULL_FACE);	//Disable face culling
//	glDisable(GL_DEPTH_TEST);	//Disable depth test
//
//	// Render to plane 2d
//	glBindVertexArray(GLM::targetMesh_[GLM::JE_TARGET_SCREEN]->vao_);
//	Shader::Use(GLM::JE_SHADER_SCREEN);
//	Shader::pCurrentShader_->SetVector4("v4_screenColor", screenColor_);
//
//	// Impose screen effect 
//	Shader::pCurrentShader_->SetEnum("enum_effectType", screenEffect_);
//
//	if (screenEffect_ == JE_EFFECT_BLUR) {
//		Shader::pCurrentShader_->SetFloat("float_blurSize", blurSize_);
//		Shader::pCurrentShader_->SetFloat("float_blurAmount", blurAmount_);
//	}
//	else if (screenEffect_ == JE_EFFECT_SOBEL)
//		Shader::pCurrentShader_->SetFloat("float_sobelAmount", sobelAmount_);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, GLM::texColorBuf_);
//	glDrawElements(GL_TRIANGLES, sizeOfPlaneIndices, GL_UNSIGNED_INT, nullptr);
//	glEnable(GL_DEPTH_TEST);
//
//	glBindVertexArray(0);
//}
//
////////////////////////////////////////////////////////////////////////////
//// Light box pipeline
////////////////////////////////////////////////////////////////////////////
//void GraphicSystem::LightSourcePipeline()
//{
//	if (isLight_) {
//
//		glEnable(GL_BLEND);
//		glEnable(GL_DEPTH_TEST);
//
//		Shader::Use(GLM::JE_SHADER_LIGHTING);
//
//		for (auto light : lights_) {
//
//			Transform* transform = light->pTransform_;
//
//			Shader::pCurrentShader_->SetMatrix(
//				"m4_translate", Translate(transform->position_));
//
//			Shader::pCurrentShader_->SetMatrix(
//				"m4_rotate", Rotate(Math::DegToRad(transform->rotation_), transform->rotationAxis_));
//
//			Shader::pCurrentShader_->SetMatrix("m4_scale", Scale(transform->scale_));
//
//			Shader::pCurrentShader_->SetMatrix(
//				"m4_rotateZ", RotateZ(atan2(light->direction_.y, light->direction_.x)));
//
//			Shader::pCurrentShader_->SetMatrix(
//				"m4_rotateY", RotateY(-atan2(light->direction_.z, light->direction_.x)));
//
//			if (light->projection_ == PROJECTION_PERSPECTIVE) {
//				Shader::pCurrentShader_->SetMatrix("m4_projection", perspective_);
//
//				viewport_ = LookAt(mainCamera_->position_, mainCamera_->target_, mainCamera_->up_);
//			}
//
//			else {
//				right_ = width_ * .5f;
//				left_ = -right_;
//				top_ = height_ * .5f;
//				bottom_ = -top_;
//
//				orthogonal_ = Orthogonal(left_, right_, bottom_, top_, mainCamera_->near_, mainCamera_->far_);
//
//				Shader::pCurrentShader_->SetMatrix("m4_projection", orthogonal_);
//
//				SetIdentity(viewport_);
//				viewport_ = Scale(resolutionScaler_);
//			}
//
//			Shader::pCurrentShader_->SetMatrix("m4_viewport", viewport_);
//			Shader::pCurrentShader_->SetVector4("v4_color", light->color_);
//
//			glBlendFunc(light->sfactor_, light->dfactor_);
//
//			// Update every mesh
//			for (auto mesh : light->meshes_)
//				Render(mesh, mesh->drawMode_);
//
//		} // for (auto light : lights_) {
//	} // if (isLight_) {
//
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_BLEND);
//}
//
//void GraphicSystem::ParentPipeline(Transform* pTransform) const
//{
//	glUniformMatrix4fv(glGetUniformLocation(Shader::pCurrentShader_->programId_, "m4_parentTranslate"),
//		1, GL_FALSE, &Translate(pTransform->position_).m[0][0]);
//
//	glUniformMatrix4fv(glGetUniformLocation(Shader::pCurrentShader_->programId_, "m4_parentScale"),
//		1, GL_FALSE, &Scale(pTransform->scale_).m[0][0]);
//
//	glUniformMatrix4fv(glGetUniformLocation(Shader::pCurrentShader_->programId_, "m4_parentRotate"),
//		1, GL_FALSE, &Rotate(DegToRad(pTransform->rotation_), pTransform->rotationAxis_).m[0][0]);
//}
//

jeEnd
