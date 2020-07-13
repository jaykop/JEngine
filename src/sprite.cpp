#include <sprite.hpp>
#include <shader.hpp>
#include <mesh.hpp>
#include <animation_2d.hpp>
#include <graphic_system.hpp>
#include <gl_manager.hpp>
#include <object.hpp>

jeBegin

jeDefineComponentBuilder(Sprite);

const std::vector<Vertex> quadVertices =
{
	{{ -.5f, .5f, 0.0f }, {0.f, 0.f, 1.f}, { 0.0f, 0.0f }, vec4::one},
	{{ -.5f, -.5f, 0.0f }, {0.f, 0.f, 1.f}, { 0.0f, 1.0f }, vec4(1,0,0,1)},
	{{ .5f, -.5f, 0.0f }, {0.f, 0.f, 1.f}, { 1.0f, 1.0f }, vec4(0,1,0,1)},
	{{ .5f,  .5f, 0.0f }, {0.f, 0.f, 1.f}, { 1.0f, 0.0f }, vec4(0,0,1,1)}
};

const std::vector<unsigned> quadIndices = { 2, 0, 1, 2, 3, 0 };

Sprite::Sprite(Object* owner)
	: Renderer(owner), animation_ (nullptr), mesh_(nullptr)
{
	if (!owner->has_component<Animation2D>())
		owner->add_component<Animation2D>();

	animation_ = owner->get_component<Animation2D>();

	mesh_ = new Mesh;
	mesh_->vertices_ = quadVertices;
	mesh_->indices_ = quadIndices;

	// setup plane VAO
	glGenVertexArrays(1, &mesh_->vao_);
	glBindVertexArray(mesh_->vao_);

	glGenBuffers(1, &mesh_->vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_->vbo_);
	glBufferData(GL_ARRAY_BUFFER, mesh_->vertices_.size() * sizeof(Vertex), &mesh_->vertices_[0], GL_STATIC_DRAW);
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

	glGenBuffers(1, &mesh_->ebo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_->ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_->indices_.size() * sizeof(unsigned), &mesh_->indices_[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}

Sprite::~Sprite()
{
	delete mesh_;
	mesh_ = nullptr;
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

	glBindVertexArray(mesh_->vao_);
	glBindTexture(GL_TEXTURE_2D, mesh_->texture_);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite::set_texture(unsigned t) { mesh_->texture_ = t; }

unsigned Sprite::get_texture() const { return mesh_->texture_; }

void Sprite::run_animation()
{
	if (animation_) {

		glBindTexture(GL_TEXTURE_2D, mesh_->texture_);

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