#pragma once
#include <vector>
#include <random.hpp>
#include <renderer.hpp>

jeBegin

struct Particle {

	vec3 color, position, direction, velocity;
	float life, rotationSpeed, rotation;
	bool hidden, dead;

	Particle() : color(vec3::zero), position(vec3::zero), direction(vec3::zero), velocity(vec3::zero),
		life(0.f), rotationSpeed(0.f), rotation(0.f), hidden(false), dead(false) {}

	Particle(Particle&& rhs) noexcept
	: color(rhs.color), position(rhs.position), direction(rhs.direction), velocity(rhs.velocity),
	life(rhs.life), rotationSpeed(rhs.rotationSpeed), rotation(rhs.rotation),
	hidden(rhs.hidden), dead(rhs.dead)  {} 
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

	void set_texture(unsigned texture);
	unsigned get_texture() const;

	void start_draw(const vec3& resScalar) override;
	void draw(float dt) override;
	void end_draw() override;

	void update_particle(Particle* particle, float dt);

	Particle* generate_particle();
	void refresh_particle(Particle* pParticle);
	void refresh_particles();

	void set_size(unsigned size);
	void set_colors(const vec3& start, const vec3& end);

	vec3 direction, velocity, range;
	float life, rotationSpeed, colorSpeed, pointSize;
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

private:

	Particles particles_;
	vec3 startColor_, endColor_, colorDiff_;
	unsigned deadCount_, size_;
	unsigned texture_;
};

jeDeclareComponentBuilder(Emitter);

jeEnd