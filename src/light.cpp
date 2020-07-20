#include <light.hpp>
#include <graphic_system.hpp>
#include <gl_manager.hpp>
#include <shader.hpp>
#include <mat4.hpp>
#include <transform.hpp>
#include <camera.hpp>

jeBegin

jeDefineComponentBuilder(Light);

float Light::constant = 1.f, Light::linear = 0.7f, Light::quadratic = 1.8f;
vec3 Light::kAmbientColor = vec3::zero, Light::fogColor = vec3::zero;

Light::Light(Object* owner)
	: Renderer(owner)
{
}

Light::~Light()
{
}

void Light::draw(float /*dt*/)
{
	Camera* camera = GraphicSystem::get_camera();
	Shader* shader = GLManager::shader_[GLManager::NORMAL];
	shader->use();

	shader->set_matrix("m4_translate", mat4::translate(transform_->position));
	shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
	shader->set_matrix("m4_rotate", transform_->orientation.to_mat4());
	shader->set_vec3("v3_color", diffuse);

	if (prjType == ProjectType::PERSPECTIVE) {

		mat4 perspective = mat4::perspective(
			camera->fovy, camera->aspect_,
			camera->near_, camera->far_);

		shader->set_matrix("m4_projection", perspective);
	}

	else {
		float right_ = GLManager::get_width() * GLManager::resScaler_.x;
		float left_ = -right_;
		float top_ = GLManager::get_height() * GLManager::resScaler_.y;
		float bottom_ = -top_;

		mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, camera->near_, camera->far_);
		shader->set_matrix("m4_projection", orthogonal);
	}

	// Send camera info to shader
	mat4 viewport = mat4::look_at(camera->position, camera->target, camera->up_);
	shader->set_matrix("m4_viewport", viewport);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(sfactor_, dfactor_);

	glBindVertexArray(GLManager::quadVao_);
	glBindBuffer(GL_ARRAY_BUFFER, GLManager::quadVbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLManager::quadEbo_);
	// glBindTexture(GL_TEXTURE_2D, texture_);
	glDrawElements(GL_TRIANGLES, GLManager::quadIndicesSize_, GL_UNSIGNED_INT, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void Light::add_to_system()
{
	GraphicSystem::add_light(this);
}

void Light::remove_from_system()
{
	GraphicSystem::remove_light(this);
}

void Light::load(const rapidjson::Value& /*data*/)
{
}

jeEnd
