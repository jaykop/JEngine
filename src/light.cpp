#include <light.hpp>
#include <graphic_system.hpp>
#include <shader.hpp>
#include <mat4.hpp>
#include <transform.hpp>
#include <camera.hpp>
#include <asset_manager.hpp>

jeBegin

jeDefineComponentBuilder(Light);

float Light::constant = 1.f, Light::linear = 0.7f, Light::quadratic = 1.8f;
vec3 Light::kAmbientColor = vec3::zero, Light::fogColor = vec3::zero;

Light::Light(Object* owner)
	: Renderer(owner)
{
	texture = AssetManager::get_texture("rect");
}

Light::~Light()
{
}

void Light::draw(float /*dt*/)
{
	Camera* camera = GraphicSystem::get_camera();
	Shader* shader = GraphicSystem::shader_[GraphicSystem::LIGHT];
	shader->use();

	shader->set_matrix("m4_translate", mat4::translate(transform_->position));
	shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
	shader->set_matrix("m4_rotate", transform_->orientation.to_mat4());
	shader->set_vec3("v3_color", diffuse);

	switch (prjType)
	{
	case ProjectType::PERSPECTIVE:
	{

		mat4 perspective = mat4::perspective(
			camera->fovy_ + camera->zoom, camera->aspect_,
			camera->near_, camera->far_);

		shader->set_matrix("m4_projection", perspective);
		break;
	}

	case ProjectType::ORTHOGONAL:
	default:
	{
		float right_ = GraphicSystem::width_ * GraphicSystem::resScaler_.x;
		float left_ = -right_;
		float top_ = GraphicSystem::height_ * GraphicSystem::resScaler_.y;
		float bottom_ = -top_;

		mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, camera->near_, camera->far_);
		shader->set_matrix("m4_projection", orthogonal);
		break;
	}
	}

	bool fixed = (status & IS_FIXED) == IS_FIXED;
	shader->set_bool("boolean_fix", fixed);

	if (!fixed)
	{
		mat4 viewport = mat4::look_at(camera->position, camera->position + camera->back_, camera->up_);
		shader->set_matrix("m4_viewport", viewport);
	}

	bool isHerited = parent_ != nullptr;
	shader->set_bool("boolean_herited", isHerited);
	if (isHerited)
	{
		Transform* pTransform = parent_->get_transform();
		shader->set_matrix("m4_parentTranslate", mat4::translate(pTransform->position));
		shader->set_matrix("m4_parentScale", mat4::scale(pTransform->scale));
		shader->set_matrix("m4_parentRotate", pTransform->orientation.to_mat4());
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(GraphicSystem::quadVao_);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, GraphicSystem::quadIndicesSize_, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glDisable(GL_DEPTH_TEST);
}

void Light::add_to_system()
{
	set_parent_renderer();
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
