#include <emitter.hpp>
#include <transform.hpp>
#include <graphic_system.hpp>
#include <shader.hpp>
#include <gl_manager.hpp>
#include <camera.hpp>
#include <asset_manager.hpp>

#include <random.hpp>
#include <math_util.hpp>

jeBegin

jeDefineComponentBuilder(Emitter);

const std::vector<unsigned> quadIndices = { 2, 0, 1, 2, 3, 0 };

Emitter::Emitter(Object* owner)
	: Renderer(owner)
{
	sfactor_ = GL_SRC_ALPHA;
	dfactor_ = GL_ONE;

	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normals of vertices
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coordinate position
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * quadIndices.size(),
		static_cast<const void*>(&quadIndices[0]), GL_DYNAMIC_DRAW);
	glBindVertexArray(0);
}

Emitter::~Emitter()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &ebo_);
}
void Emitter::add_to_system() {
	GraphicSystem::add_renderer(this);
}

void Emitter::remove_from_system() {
	GraphicSystem::remove_renderer(this);
}

void Emitter::load(const rapidjson::Value& /*data*/) {

}

void Emitter::start_draw(Camera* camera, const mat4& perspective, const vec3& resScalar)
{
	Shader* shader = GLManager::shader_[GLManager::Pipeline::PARTICLE];
	shader->use();

	shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
	shader->set_bool("boolean_bilboard", (status & IS_BILBOARD) == IS_BILBOARD);

	mat4 viewport;

	if (prjType == ProjectType::PERSPECTIVE) {

		viewport = mat4::look_at(camera->position_, camera->target_, camera->up_);
		shader->set_matrix("m4_projection", perspective);
	}

	else {

		viewport = mat4::scale(resScalar);

		float right_ = GLManager::get_width() * .5f;
		float left_ = -right_;
		float top_ = GLManager::get_height() * .5f;
		float bottom_ = -top_;

		mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, camera->near_, camera->far_);
		shader->set_matrix("m4_projection", orthogonal);
	}

	// Send camera info to shader
	shader->set_matrix("m4_viewport", viewport);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(sfactor_, dfactor_);
	glPointSize(pointSize);
}

void Emitter::draw(float dt)
{
	if (active) {

		Shader* shader = GLManager::shader_[GLManager::Pipeline::PARTICLE];

		for (auto particle : particles_) {

			if (particle->life < 0.f)
				refresh_particle(particle);

			else {

				particle->life -= dt;
				particle->position += particle->direction * dt * velocity;

				if (rotationSpeed)
					particle->rotation += particle->rotationSpeed * dt;

				if (changeColor_)
					particle->color += colorDiff_ * dt;

				vec3 viewDirection = (camera_->position_ - particle->position).normalized();

				// Send transform info to shader
				shader->set_matrix("m4_translate", mat4::translate(particle->position));
				shader->set_matrix("m4_rotate", mat4::rotate(Math::deg_to_rad(particle->rotation), viewDirection));

				// Send color info to shader
				shader->set_vec4("v4_color", vec4(particle->color, particle->life));
				shader->set_bool("boolean_hide", particle->hidden);

				glBindTexture(GL_TEXTURE_2D, texture_);
				glBindVertexArray(vao_);
				glDrawElements(GL_TRIANGLES, quadIndices.size(), GL_UNSIGNED_INT, nullptr);
				glBindVertexArray(0);
				
			}
		}
	}

	glBindVertexArray(0);
}

void Emitter::end_draw()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

Emitter::Particle* Emitter::generate_particle()
{
	Particle* newParticle = new Particle;

	newParticle->dead = false;
	newParticle->life = Random::get_rand_float(0.f, life);
	newParticle->velocity = Random::get_rand_vec3(vec3::zero, velocity);
	newParticle->position = transform_->position;
	newParticle->rotation = Random::get_rand_float(0.f, 360.f);
	newParticle->direction = direction == vec3::zero ? Random::get_rand_vec3(-vec3::one, vec3::one) : direction;
	newParticle->direction.normalize();
	newParticle->color.set(startColor_);
	newParticle->hidden = type != PARTICLE_EXPLODE;

	if (rotationSpeed)
		newParticle->rotationSpeed = Random::get_rand_float(0.f, rotationSpeed);

	return newParticle;
}

void Emitter::refresh_particle(Particle* particle)
{
	particle->rotation = Random::get_rand_float(0.f, 360.f);
	particle->rotationSpeed = Random::get_rand_float(0.f, rotationSpeed);

	particle->life = Random::get_rand_float(0.f, life);
	particle->color.set(startColor_);

	if (type == PARTICLE_NORMAL) {

		particle->position = transform_->position;
		particle->hidden = false;
		particle->direction = direction == vec3::zero ? Random::get_rand_vec3(-vec3::one, vec3::one) : direction;
	}

	else if (type == PARTICLE_EXPLODE) {

		// No more particle to update,
		// turn off the active toggle
		if (size_ == deadCount_)
			active = false;

		else if (!particle->dead) {

			// Ready for next update
			particle->position = transform_->position;
			particle->direction = direction == vec3::zero ? Random::get_rand_vec3(-vec3::one, vec3::one) : direction;

			// Set dead and add number
			particle->dead = true;
			particle->hidden = true;
			++deadCount_;
		}
	}

	else if (type == PARTICLE_WIDE) {

		particle->hidden = false;

		vec3 position;
		position = transform_->position;
		particle->direction = direction == vec3::zero ? Random::get_rand_vec3(-vec3::one, vec3::one) : direction;
		particle->position.x = Random::get_rand_float(position.x - range.x, position.x + range.x);
		particle->position.y = Random::get_rand_float(position.y - range.y, position.y + range.y);
		particle->position.z = Random::get_rand_float(position.z - range.z, position.z + range.z);

		particle->life = Random::get_rand_float(0.f, life);
		particle->color.set(startColor_);

	}
}

void Emitter::refresh_particles()
{
	deadCount_ = 0;
	for (auto particle : particles_)
		refresh_particle(particle);
}

void Emitter::set_size(unsigned size)
{
	if (particles_.empty()) {

		for (unsigned i = 0; i < size; ++i)
			particles_.emplace_back(generate_particle());

		size_ = size;
	}

	else
		jeDebugPrint("!Emitter - Already allocated.\n");
}

void Emitter::set_colors(const vec3& start, const vec3& end)
{
	startColor_ = start, endColor_ = end;
	colorDiff_ = (endColor_ - startColor_) / life;

	// If dff is zero, no need to add diff
	if (colorDiff_ == vec3::zero)
		changeColor_ = false;

	else
		changeColor_ = true;
}

jeEnd