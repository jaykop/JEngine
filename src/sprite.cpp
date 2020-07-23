#include <sprite.hpp>
#include <shader.hpp>
#include <animation_2d.hpp>
#include <graphic_system.hpp>
#include <object.hpp>

#include <camera.hpp>
#include <transform.hpp>
#include <mat4.hpp>

jeBegin

jeDefineComponentBuilder(Sprite);

Sprite::Sprite(Object* owner)
	: Renderer(owner), color(vec4::one), animation_ (nullptr), texture_(0)
{
	if (!owner->has_component<Animation2D>())
		owner->add_component<Animation2D>();

	animation_ = owner->get_component<Animation2D>();
}

Sprite::~Sprite()
{
	remove_from_system();
}

void Sprite::add_to_system() 
{
	set_parent_renderer();
	GraphicSystem::add_renderer(this);
}

void Sprite::remove_from_system() 
{
	GraphicSystem::remove_renderer(this);
}

void Sprite::load(const rapidjson::Value& /*data*/) {

}


void Sprite::draw(float /*dt*/)
{
	Camera* camera = GraphicSystem::get_camera();
	Shader* shader = GraphicSystem::shader_[GraphicSystem::SPRITE];
	shader->use();

	shader->set_matrix("m4_translate", mat4::translate(transform_->position));
	shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
	shader->set_matrix("m4_rotate", transform_->orientation.to_mat4());
	shader->set_vec3("v3_cameraPosition", camera->position);
	shader->set_bool("boolean_bilboard", (status & IS_BILBOARD) == IS_BILBOARD);
	shader->set_bool("boolean_flip", (status & IS_FLIPPED) == IS_FLIPPED);
	shader->set_vec4("v4_color", color);

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
		// Send camera info to shader
		// mat4 viewport = mat4::look_at(camera->position, camera->right_, camera->up_, camera->back_);
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

	//if (pModel->pMaterial_ && isLight_)
	//	LightingEffectPipeline(pModel->pMaterial_);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(sfactor_, dfactor_);

	run_animation();

	glBindVertexArray(GraphicSystem::quadVao_);
	glBindBuffer(GL_ARRAY_BUFFER, GraphicSystem::quadVbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GraphicSystem::quadEbo_);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glDrawElements(GL_TRIANGLES, GraphicSystem::quadIndicesSize_, GL_UNSIGNED_INT, nullptr);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void Sprite::set_texture(unsigned t) { texture_ = t; }

unsigned Sprite::get_texture() const { return texture_; }

void Sprite::run_animation()
{
	if (animation_) {

		glBindTexture(GL_TEXTURE_2D, texture_);

		if (animation_->activated_) {

			float realSpeed = animation_->realSpeed_;

			if (realSpeed && realSpeed <= animation_->timer_.get_elapsed_time()) {

				float nextFrame = animation_->currentFrame_;
				float realFrame = animation_->realFrame_;

				nextFrame = (status & IS_FLIPPED) == IS_FLIPPED ? nextFrame - realFrame : nextFrame + realFrame;

				animation_->currentFrame_ = nextFrame >= 1.f ? 0.f : nextFrame;
				animation_->timer_.start();
			}

			animation_->scale_.set(animation_->realFrame_, 1.f, 0.f);
			animation_->translate_.set(animation_->currentFrame_, 0.f, 0.f);

		}

		else {
			animation_->scale_.set(1, 1, 0);
			animation_->translate_.set(0, 0, 0);
		}

		// Send color info to shader
		Shader* shader = GraphicSystem::shader_[GraphicSystem::SPRITE];
		shader->use();

		shader->set_matrix("m4_aniScale", mat4::scale(animation_->scale_));
		shader->set_matrix("m4_aniTranslate", mat4::translate(animation_->translate_));
	}
}

jeEnd