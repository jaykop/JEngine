#include <sprite.hpp>
#include <shader.hpp>
#include <animation_2d.hpp>
#include <graphic_system.hpp>
#include <gl_manager.hpp>
#include <object.hpp>

#include <camera.hpp>
#include <transform.hpp>
#include <mat4.hpp>

jeBegin

jeDefineComponentBuilder(Sprite);

Sprite::Sprite(Object* owner)
	: Renderer(owner), color(vec4::one), animation_ (nullptr)
{
	if (!owner->has_component<Animation2D>())
		owner->add_component<Animation2D>();

	animation_ = owner->get_component<Animation2D>();
}

Sprite::~Sprite()
{
	remove_from_system();
}

void Sprite::add_to_system() {
	GraphicSystem::add_renderer(this);
}

void Sprite::remove_from_system() {
	GraphicSystem::remove_renderer(this);
}

void Sprite::load(const rapidjson::Value& /*data*/) {

}

void Sprite::start_draw()
{
	Camera* camera = GraphicSystem::get_camera();
	Shader* shader = GLManager::shader_[GLManager::NORMAL];
	shader->use();

	shader->set_matrix("m4_translate", mat4::translate(transform_->position));
	shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
	shader->set_matrix("m4_rotate", transform_->orientation.to_mat4());
	shader->set_vec3("v3_cameraPosition", camera->position);
	shader->set_bool("boolean_bilboard", (status & IS_BILBOARD) == IS_BILBOARD);
	shader->set_vec4("v4_color", color);

	mat4 viewport;

	if (prjType == ProjectType::PERSPECTIVE) {

		viewport = mat4::look_at(camera->position, camera->target, camera->up_);

		mat4 perspective = mat4::perspective(
			camera->fovy, camera->aspect_,
			camera->near_, camera->far_);

		shader->set_matrix("m4_projection", perspective);
	}

	else {

		viewport = mat4::scale(GLManager::resScaler_);

		float right_ = GLManager::get_width() * .5f;
		float left_ = -right_;
		float top_ = GLManager::get_height() * .5f;
		float bottom_ = -top_;

		mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, camera->near_, camera->far_);
		shader->set_matrix("m4_projection", orthogonal);
	}

	// Send camera info to shader
	shader->set_matrix("m4_viewport", viewport);

	//bool hasParent = (pModel->status_ & Model::IS_INHERITED) == Model::IS_INHERITED;
	//glUniform1i(glGetUniformLocation(Shader::pCurrentShader_->programId_, "hasParent"), hasParent);
	//if (hasParent)
	//	ParentPipeline(pModel->pInherited_);

	//if (pModel->pMaterial_ && isLight_)
	//	LightingEffectPipeline(pModel->pMaterial_);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(sfactor_, dfactor_);
}

void Sprite::draw(float /*dt*/)
{
	run_animation();

	glBindVertexArray(GLManager::quadVao_);
	glBindBuffer(GL_ARRAY_BUFFER, GLManager::quadVbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLManager::quadEbo_);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glDrawElements(GL_TRIANGLES, GLManager::quadIndicesSize_, GL_UNSIGNED_INT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sprite::end_draw()
{
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
		Shader* shader = GLManager::shader_[GLManager::NORMAL];
		shader->use();

		shader->set_bool("boolean_flip", (status & IS_FLIPPED) == IS_FLIPPED);
		shader->set_matrix("m4_aniScale", mat4::scale(animation_->scale_));
		shader->set_matrix("m4_aniTranslate", mat4::translate(animation_->translate_));
	}
}

jeEnd