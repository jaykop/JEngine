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

	// update main camera
	// mainCamera_->update();

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
	for (auto& r : renderers_)
		r->draw(dt);
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
	shader->set_matrix("m4_scale", mat4::scale(vec3::one * static_cast<float>(grid.size)));
	shader->set_matrix("m4_rotate", mat4::identity);
	shader->set_vec3("v3_color", grid.color);

	if (grid.prjType == Renderer::ProjectType::PERSPECTIVE) {

		mat4 perspective = mat4::perspective(
			mainCamera_->fovy, mainCamera_->aspect_,
			mainCamera_->near_, mainCamera_->far_);

		shader->set_matrix("m4_projection", perspective);
	}

	else {

		float right_ = GLManager::get_width() * GLManager::resScaler_.x;
		float left_ = -right_;
		float top_ = GLManager::get_height() * GLManager::resScaler_.y;
		float bottom_ = -top_;

		mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, mainCamera_->near_, mainCamera_->far_);
		shader->set_matrix("m4_projection", orthogonal);
	}

	// Send camera info to shader
	mat4 viewport = mat4::look_at(mainCamera_->position, mainCamera_->target, mainCamera_->up_);
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


//void GraphicSystem::RenderToFramebuffer() const
//{
//	// Render to framebuffer
//	glBindFramebuffer(GL_FRAMEBUFFER, GLM::fbo_);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClearColor(0.f, 0.f, 0.f, 0.f);
//	glViewport(0, 0, GLint(width_), GLint(height_));
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

//
//void GraphicSystem::LightingEffectPipeline(Material* pMaterial)
//{
//	Shader::pCurrentShader_->SetInt("int_lightSize", int(lights_.size()));
//
//	// Send material info to shader
//	Shader::pCurrentShader_->SetInt("material.m_specular", pMaterial->specular_);
//	Shader::pCurrentShader_->SetInt("material.m_diffuse", pMaterial->diffuse_);
//	Shader::pCurrentShader_->SetFloat("material.m_shininess", pMaterial->shininess_);
//
//	static int s_lightIndex;
//	static std::string s_index, s_light,
//		amb("m_ambient"), spec("m_specular"), diff("m_diffuse"),
//		type("m_type"), constant("m_constant"), linear("m_linear"), dir("m_direction"), pos("m_position"),
//		cut("m_cutOff"), outcut("m_outerCutOff"), quad("m_quadratic");
//	s_lightIndex = 0;
//
//	for (auto _light : lights_) {
//
//		s_index = std::to_string(s_lightIndex);
//
//		Shader::pCurrentShader_->SetVector4(
//			("v4_lightColor[" + s_index + "]").c_str(), _light->color_);
//
//		s_light = "light[" + s_index + "].";
//
//		Shader::pCurrentShader_->SetVector4(
//			(s_light + spec).c_str(), _light->specular_);
//
//		Shader::pCurrentShader_->SetVector4(
//			(s_light + diff).c_str(), _light->diffuse_);
//
//		Shader::pCurrentShader_->SetEnum(
//			(s_light + type).c_str(), _light->type_);
//
//		Shader::pCurrentShader_->SetVector3(
//			(s_light + dir).c_str(), _light->direction_);
//
//		Shader::pCurrentShader_->SetFloat(
//			(s_light + constant).c_str(), _light->constant_);
//
//		Shader::pCurrentShader_->SetFloat(
//			(s_light + linear).c_str(), _light->linear_);
//
//		Shader::pCurrentShader_->SetFloat(
//			(s_light + quad).c_str(), _light->quadratic_);
//
//		Shader::pCurrentShader_->SetVector3(
//			(s_light + pos).c_str(), _light->pTransform_->position_);
//
//		Shader::pCurrentShader_->SetFloat(
//			(s_light + cut).c_str(), cosf(Math::DegToRad(_light->cutOff_)));
//
//		Shader::pCurrentShader_->SetFloat(
//			(s_light + outcut).c_str(), cosf(Math::DegToRad(_light->outerCutOff_)));
//
//		s_lightIndex++;
//	}
//}

jeEnd
