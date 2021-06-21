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

const unsigned NUM_THREADS = 12;

Emitter::Emitter(Object* owner)
	: Renderer(owner), angle(vec2::zero), velocity(vec3::zero), range(vec3::zero),
	life(1.f), rotationSpeed(0.f), colorSpeed(1.f), pointSize_(0.f), active(true),
	type(ParticleType::NORMAL), startColor_(vec3::zero), endColor_(vec3::zero), colorDiff_(vec3::zero),
	deadCount_(0), size_(0), drawMode_(GL_TRIANGLES), texture_(0)
{
	status |= IS_BILBOARD;
	sfactor_ = GL_SRC_ALPHA;
	dfactor_ = GL_ONE_MINUS_SRC_ALPHA;
}

Emitter::~Emitter()
{
	remove_from_system();

	g_particule_position_size_data.clear();
	g_particule_color_data.clear();

	for (auto& p : particles_)
	{
		delete p;
		p = nullptr;
	}

	particles_.clear();
}

void Emitter::add_to_system() {

	if (particles_.empty()) {

		for (unsigned i = 0; i < size_; ++i)
			particles_.emplace_back(generate_particle());

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

		shader->set_matrix("m4_translate", mat4::translate(transform_->position));
		shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
		//shader->set_matrix("m4_rotate", transform_->orientation.to_mat4());
		//shader->set_vec3("v3_cameraPosition", camera->position);
		shader->set_bool("boolean_bilboard", (status & IS_BILBOARD) == IS_BILBOARD);
		shader->set_bool("boolean_flip", (status & IS_FLIPPED) == IS_FLIPPED);
		//shader->set_vec4("v4_color", color);

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
			// mat4 viewport = mat4::look_at(camera->position, camera->right_, camera->up_, camera->back_);
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

		glBindVertexArray(GraphicSystem::quadVao_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		int particleSize = static_cast<int>(particles_.size());

		for (int i = 0; i < particleSize; ++i) {

			// if (particles_[i]->done) 
			// continue;

			if (particles_[i]->life < 0.f)
				refresh_particle(particles_[i]);

			else
			{
				// particles_[i]->done = false;

				particles_[i]->life -= dt;
				particles_[i]->position += particles_[i]->direction * dt * velocity;

				if (rotationSpeed)
					particles_[i]->rotation += particles_[i]->rotationSpeed * dt;

				if (colorDiff_ != vec3::zero)
					particles_[i]->color += colorDiff_ * dt * colorSpeed;

				vec3 viewDirection = (camera->position - particles_[i]->position).normalized();

				// Send transform info to shader
				// particle->position.z = transform_->position.z;
				// shader->set_matrix("m4_translate", mat4::translate(particles_[i]->position));
				shader->set_matrix("m4_rotate", mat4::rotate(Math::deg_to_rad(particles_[i]->rotation), viewDirection));

				// Send color info to shader
				shader->set_vec4("v4_color", vec4(particles_[i]->color, particles_[i]->life));
				shader->set_bool("boolean_hide", particles_[i]->hidden);

				// Fill the GPU buffer
				g_particule_position_size_data[3 * i + 0] = particles_[i]->position.x;
				g_particule_position_size_data[3 * i + 1] = particles_[i]->position.y;
				g_particule_position_size_data[3 * i + 2] = particles_[i]->position.z;

				g_particule_color_data[4 * i + 0] = particles_[i]->color.r;
				g_particule_color_data[4 * i + 1] = particles_[i]->color.g;
				g_particule_color_data[4 * i + 2] = particles_[i]->color.b;
				g_particule_color_data[4 * i + 3] = particles_[i]->life;

				// glDrawElements(drawMode_, GraphicSystem::quadIndicesSize_, GL_UNSIGNED_INT, nullptr);
			}

			// particles_[i]->done = !particles_[i]->done ? true : false;
		}

		glBindBuffer(GL_ARRAY_BUFFER, GraphicSystem::particlesPosBuf_);
		glBufferData(GL_ARRAY_BUFFER, GraphicSystem::ParticleMaxSize * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, particleSize * sizeof(GLfloat) * 3, &g_particule_position_size_data[0]);

		glBindBuffer(GL_ARRAY_BUFFER, GraphicSystem::particlesColorBuf_);
		glBufferData(GL_ARRAY_BUFFER, GraphicSystem::ParticleMaxSize* 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, particleSize * sizeof(GLfloat) * 4, &g_particule_color_data[0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, GraphicSystem::billboardVerticeBuf_);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : positions of particles' centers
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, GraphicSystem::particlesPosBuf_);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size : x + y + z + size => 4
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, GraphicSystem::particlesColorBuf_);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : r + g + b + a => 4
			GL_FLOAT,						  // type
			GL_FALSE,                         // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1); // positions : one per quad (its center)                 -> 1

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleSize);

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
	// int totalSize = 4 * size_;
	g_particule_position_size_data.resize(GraphicSystem::ParticleMaxSize, 0.f);
	g_particule_color_data.resize(GraphicSystem::ParticleMaxSize, 0.f);

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