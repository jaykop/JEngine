#pragma once
#include <vector>
#include <random.hpp>
#include <renderer.hpp>

#include <vec3.hpp>
#include <vec2.hpp>

jeBegin

struct Particle {

	vec3 color = vec3::zero, position = vec3::zero, direction = vec3::zero, velocity = vec3::zero;
	float life = 0.f, rotationSpeed = 0.f, rotation = 0.f;
	bool hidden = false, dead = false, done = false;
};

class Emitter : public Renderer
{
	jeBaseFriends(Emitter);
	friend class GraphicSystem;

	using Particles = std::vector<Particle*>;

public:

	enum class ParticleType { NORMAL, EXPLOSION, WIDE, SMOKE };

	Emitter(Object* owner);
	virtual ~Emitter();

	void refresh_particles();

	void set_size(unsigned size);
	void set_colors(const vec3& start, const vec3& end);

	void set_texture(unsigned texture);
	unsigned get_texture() const;

	void set_pointSize(float size);
	float get_pointSize() const;

	vec2 angle;
	vec3 velocity, range;
	float life, rotationSpeed, colorSpeed;
	bool active;
	ParticleType type;

	//Emitter(const Emitter& rhs) = delete;
	//Emitter& operator=(const Emitter& rhs) = delete;
	//Emitter(Emitter&& rhs) noexcept = delete;
	//Emitter& operator=(Emitter&& rhs) noexcept = delete;

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& data);

	void draw(float dt) override;

private:

	void update_particle(Particles& particles,
		float dt, unsigned start, unsigned end);

	Particle* generate_particle();
	void refresh_particle(Particle* pParticle);

	Particles particles_;
	vec3 startColor_, endColor_, colorDiff_;
	unsigned deadCount_, size_, drawMode_;
	float pointSize_;
	unsigned texture_;

	std::vector<float> particlePosData_, particleColorData_;
};

jeDeclareComponentBuilder(Emitter);

jeEnd