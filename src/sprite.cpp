#include <sprite.hpp>
#include <shader.hpp>
#include <mesh.hpp>
#include <animation_2d.hpp>
#include <graphic_system.hpp>
#include <gl_manager.hpp>
#include <object.hpp>

jeBegin

jeDefineComponentBuilder(Sprite);

Sprite::Sprite(Object* owner)
	: Renderer(owner), animation_ (nullptr),
	color(vec4::one), texture_(0)
{
	if (!owner->has_component<Animation2D>())
		owner->add_component<Animation2D>();

	animation_ = owner->get_component<Animation2D>();
}

void Sprite::add_to_system() {
	GraphicSystem::add_renderer(this);
}

void Sprite::remove_from_system() {
	GraphicSystem::remove_renderer(this);
}

void Sprite::load(const rapidjson::Value& /*data*/) {

}

void Sprite::draw()
{
	run_animation();

	Shader* shader = GLManager::shader_[GLManager::Pipeline::NORMAL];
	shader->use();
	shader->set_vec4("v4_color", color);

	glBindVertexArray(Mesh::quadVAO);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
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

				nextFrame = (status_ & IS_FLIPPED) == IS_FLIPPED ? nextFrame - realFrame : nextFrame + realFrame;

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
		Shader* shader = GLManager::shader_[GLManager::Pipeline::NORMAL];
		shader->use();

		shader->set_bool("boolean_flip", (status_ & IS_FLIPPED) == IS_FLIPPED);
		shader->set_matrix("m4_aniScale", mat4::scale(animation_->scale_));
		shader->set_matrix("m4_aniTranslate", mat4::translate(animation_->translate_));
	}
}

jeEnd