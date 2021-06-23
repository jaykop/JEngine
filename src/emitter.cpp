#include <emitter.hpp>
#include <transform.hpp>
#include <graphic_system.hpp>
#include <shader.hpp>
#include <camera.hpp>
#include <asset_manager.hpp>

#include <random.hpp>
#include <math_util.hpp>
#include <mat4.hpp>

jeBegin

jeDefineComponentBuilder(Emitter);

const int COLOR_COEFFICIENT = 4, POSITION_COEFFICIENT = 3;

Emitter::Emitter(Object* owner)
	: Renderer(owner), angle(vec2::zero), velocity(vec3::zero), range(vec3::zero),
	life(1.f), rotationSpeed(0.f), colorSpeed(1.f), pointSize_(0.f), active(true),
	type(ParticleType::NORMAL), startColor_(vec3::zero), endColor_(vec3::zero), colorDiff_(vec3::zero),
	deadCount_(0), size_(0), drawMode_(GL_TRIANGLE_STRIP), texture_(0)
{
	status |= IS_BILBOARD;
	sfactor_ = GL_SRC_ALPHA;
	dfactor_ = GL_ONE_MINUS_SRC_ALPHA;
}

Emitter::~Emitter()
{
	remove_from_system();

	for (auto& p : particles_)
	{
		delete p;
		p = nullptr;
	}

	particlePosData_.clear();
	particleColorData_.clear();
	particles_.clear();
}

void Emitter::add_to_system() {

	if (particles_.empty()) {

		for (unsigned i = 0; i < size_; ++i)
			particles_.emplace_back(generate_particle());

		particlePosData_.resize(size_ * POSITION_COEFFICIENT, 0.f);
		particleColorData_.resize(size_ * COLOR_COEFFICIENT, 0.f);

		colorDiff_ = (endColor_ - startColor_) / life;
	}

	else
		jeDebugPrint("!Emitter - Already allocated.\n");

	set_parent_renderer();
	GraphicSystem::add_renderer(this);
}

void Emitter::remove_from_system() {
	GraphicSystem::remove_renderer(this);
}

void Emitter::load(const rapidjson::Value& /*data*/) {

}

void Emitter::draw(float dt)
{
	if (active) {

		Camera* camera = GraphicSystem::get_camera();

		Shader* shader = GraphicSystem::shader_[GraphicSystem::PARTICLE];
		shader->use();

		shader->set_matrix("m4_translate", mat4::identity);
		shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
		shader->set_bool("boolean_bilboard", (status & IS_BILBOARD) == IS_BILBOARD);
		shader->set_bool("boolean_flip", (status & IS_FLIPPED) == IS_FLIPPED);

		switch (prjType)
		{
		case ProjectType::PERSPECTIVE:
		{

			mat4 perspective = mat4::perspective(
				Math::deg_to_rad(camera->fovy_ + camera->zoom), camera->aspect_,
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
			mat4 viewport = mat4::look_at(camera->position, camera->position + camera->front_, camera->up_);
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

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(sfactor_, dfactor_);
		glPointSize(pointSize_);

		glBindVertexArray(GraphicSystem::particleVao_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		int particleSize = static_cast<int>(size_);

		for (int i = 0; i < particleSize; ++i) {

			if (particles_[i]->done) 
				continue;

			else if (particles_[i]->life < 0.f)
				refresh_particle(particles_[i]);

			else
			{
				particles_[i]->life -= dt;
				particles_[i]->position += particles_[i]->direction * dt * velocity;

				if (rotationSpeed)
					particles_[i]->rotation += particles_[i]->rotationSpeed * dt;

				if (colorDiff_ != vec3::zero)
					particles_[i]->color += colorDiff_ * dt * colorSpeed;

				vec3 viewDirection = (camera->position - particles_[i]->position).normalized();

				// Send transform info to shader
				shader->set_matrix("m4_rotate", mat4::rotate(Math::deg_to_rad(particles_[i]->rotation), viewDirection));

				// Send color info to shader
				shader->set_bool("boolean_hide", particles_[i]->hidden);

				// Fill the GPU buffer
				int posStride = POSITION_COEFFICIENT * i;
				particlePosData_[posStride] = particles_[i]->position.x;
				particlePosData_[posStride + 1] = particles_[i]->position.y;
				particlePosData_[posStride + 2] = particles_[i]->position.z;// particles_[i]->position.z;

				int colorStride = COLOR_COEFFICIENT * i;
				particleColorData_[colorStride] = particles_[i]->color.r;
				particleColorData_[colorStride + 1] = particles_[i]->color.g;
				particleColorData_[colorStride + 2] = particles_[i]->color.b;
				particleColorData_[colorStride + 3] = particles_[i]->life;

			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, GraphicSystem::particlesPosBuf_);
		glBufferData(GL_ARRAY_BUFFER, particleSize* POSITION_COEFFICIENT * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, particleSize * sizeof(GLfloat) * POSITION_COEFFICIENT, &particlePosData_[0]);

		glBindBuffer(GL_ARRAY_BUFFER, GraphicSystem::particlesColorBuf_);
		glBufferData(GL_ARRAY_BUFFER, particleSize * COLOR_COEFFICIENT * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, particleSize * sizeof(GLfloat) * COLOR_COEFFICIENT, &particleColorData_[0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, GraphicSystem::billboardVerticeBuf_);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, GraphicSystem::particlesPosBuf_);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, GraphicSystem::particlesColorBuf_);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glVertexAttribDivisor(0, 0); 
		glVertexAttribDivisor(1, 1); 
		glVertexAttribDivisor(2, 1); 

		glDrawArraysInstanced(drawMode_, 0, 4, particleSize);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}

Particle* Emitter::generate_particle()
{
	Particle* newParticle = new Particle;

	newParticle->dead = false;
	newParticle->life = Random::get_rand_float(0.f, life);
	newParticle->velocity = Random::get_rand_vec3(-velocity, velocity);
	newParticle->position = transform_->position;
	newParticle->rotation = Random::get_rand_float(0.f, 360.f);
	newParticle->color.set(startColor_);
	newParticle->hidden = type != ParticleType::EXPLOSION;

	if (rotationSpeed)
		newParticle->rotationSpeed = Random::get_rand_float(0.f, rotationSpeed);

	if (angle == vec2::zero)
		newParticle->direction = Random::get_rand_vec3(-vec3::one, vec3::one);

	else
	{
		float rad = Math::deg_to_rad(Random::get_rand_float(angle.x, angle.y));
		newParticle->direction.set(cosf(rad), sinf(rad), 0.f);
	}

	newParticle->direction.normalize();

	return newParticle;
}

void Emitter::refresh_particle(Particle* particle)
{
	particle->life = Random::get_rand_float(0.f, life);
	particle->color.set(startColor_);
	particle->rotation = Random::get_rand_float(0.f, 360.f);
	if (rotationSpeed)
		particle->rotationSpeed = Random::get_rand_float(0.f, rotationSpeed);

	if (type == ParticleType::NORMAL) {

		particle->position = transform_->position;
		particle->hidden = false;
		particle->velocity = Random::get_rand_vec3(-velocity, velocity);
	}

	else if (type == ParticleType::EXPLOSION) {

		// No more particle to update,
		// turn off the active toggle
		if (size_ == deadCount_)
			active = false;

		else if (!particle->dead) {

			// Set dead and add number
			particle->dead = true;
			particle->hidden = true;
			++deadCount_;
		}

		else if (!deadCount_)
		{
			particle->hidden = false;
			particle->dead = false;
			particle->velocity = Random::get_rand_vec3(-velocity, velocity);
			particle->position = transform_->position;
		}

	}

	else if (type == ParticleType::WIDE) {

		particle->hidden = false;

		vec3 position;
		position = transform_->position;
		
		particle->position.x = Random::get_rand_float(position.x - range.x, position.x + range.x);
		particle->position.y = Random::get_rand_float(position.y - range.y, position.y + range.y);
		particle->position.z = Random::get_rand_float(position.z - range.z, position.z + range.z);

		particle->velocity = Random::get_rand_vec3(-velocity, velocity);
		particle->life = Random::get_rand_float(0.f, life);
		particle->color.set(startColor_);
	}

	if (angle == vec2::zero)
		particle->direction = Random::get_rand_vec3(-vec3::one, vec3::one);

	else
	{
		float rad = Math::deg_to_rad(Random::get_rand_float(angle.x, angle.y));
		particle->direction.set(cosf(rad), sinf(rad), 0.f);
	}

	particle->direction.normalize();
	particle->position.z = transform_->position.z;
}

void Emitter::refresh_particles()
{
	deadCount_ = 0;
	for (auto particle : particles_)
		refresh_particle(particle);
}

void Emitter::set_size(unsigned size)
{
	size_ = size;
}

void Emitter::set_colors(const vec3& start, const vec3& end)
{
	startColor_ = start, endColor_ = end;
}

void Emitter::set_texture(unsigned t) { texture_ = t; }

unsigned Emitter::get_texture() const { return texture_; }

void Emitter::set_pointSize(float size)
{
	if (size)
		drawMode_ = GL_POINTS;
	else
		drawMode_ = GL_TRIANGLES;

	pointSize_ = size;
}

float Emitter::get_pointSize() const
{
	return pointSize_;
}

jeEnd