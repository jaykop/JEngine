#pragma once
#include <renderer.hpp>

jeBegin

class Camera;

class Emitter : public Renderer
{
	jeBaseFriends(Emitter);
	friend class GraphicSystem;

	struct Particle {

		vec3 color, position, direction, velocity;
		float life, rotationSpeed, rotation;
		bool hidden, dead;
		Emitter* pEmitter;
	};

	using Particles = std::vector<Particle*>;

	enum ParticleType { PARTICLE_NORMAL, PARTICLE_EXPLODE, PARTICLE_WIDE, PARTICLE_SMOG };

public:

	Emitter(Object* owner);
	virtual ~Emitter();

	void set_texture(unsigned texture);
	unsigned get_texture() const;

	void start_draw(Camera* camera, const mat4& perspective, const vec3& resScalar) override;
	void draw(float dt) override;
	void end_draw() override;

	Particle* generate_particle();
	void refresh_particle(Particle* pParticle);

	void refresh_particles();

	void set_size(unsigned size);
	void set_colors(const vec3& start, const vec3& end);

	vec3 direction, velocity, range;
	float life, rotationSpeed, pointSize;
	bool active;
	ParticleType type;

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& data);

private:

	Particles particles_;
	vec3 startColor_, endColor_, colorDiff_;
	bool changeColor_;
	unsigned deadCount_, size_;
	unsigned vao_, vbo_, ebo_, texture_;
	Camera* camera_ = nullptr;
};

jeDeclareComponentBuilder(Emitter);

jeEnd