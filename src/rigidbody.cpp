#include <physics_system.hpp>
#include <rigidbody.hpp>
#include <transform.hpp>
#include <object.hpp>
#include <math_util.hpp>

jeBegin

jeDefineComponentBuilder(RigidBody);

RigidBody::RigidBody(Object* owner) 
	: Component(owner), friction(0.f), restitution(0.1f), glue(0.01f),
mass(1.f), gravity(0.f), angOrientation(0.f), density_(1.f)
{}

RigidBody::~RigidBody() {}

void RigidBody::add_to_system()
{
	transform = get_owner()->get_component<Transform>();
	initialize();
	PhysicsSystem::add_rigidbody(this);
}

void RigidBody::remove_from_system() 
{
	vertices_.clear();
}

float RigidBody::get_invMass() const
{
	return (mass > 0.0f) ? (1.0f / mass) : 0.0f;
}

void RigidBody::add_force(const vec3& f)
{
	if (isStatic)
		return;

	netForce += f;
}

void RigidBody::add_force(const vec3& f, const vec3& p)
{
	if (isStatic) return;

	netForce += f;
	netTorque += ((p - transform->position).cross(f)).length();
}

void RigidBody::set_density(float density)
{
	density_ = density;	
	mass = 0.0f;
	inertia = 0.0f;

	if (density_ > 0.0f && !vertices_.empty())
	{
		mass = PhysicsSystem::calculate_mass(vertices_, density);
		inertia = PhysicsSystem::calculate_inertia(vertices_, mass);
	}
}

void RigidBody::set_orientation(float angle)
{
	angOrientation = angle;
	orientation = mat4::rotate_z(angOrientation).to_mat3();
}

float RigidBody::get_invInertia()
{
	return (inertia > 0.0f) ? 1.0f / inertia : 0.0f;
}

void RigidBody::update(float dt)
{
	if (isStatic)
	{
		velocity.set_zero();
		angVelocity = 0.f;
		return;
	}

	//-------------------------------------------------------
	// Integrate position (verlet integration)
	//-------------------------------------------------------
	transform->position += velocity * dt;
	angOrientation += angVelocity * dt;
	if (angOrientation > Math::round) angOrientation -= Math::round;
	else if (angOrientation < Math::zero) angOrientation += Math::round;
	orientation = mat4::rotate_z(Math::deg_to_rad(angOrientation)).to_mat3();

	//-------------------------------------------------------
	// Integrate velocity (implicit linear velocity)
	//-------------------------------------------------------
	velocity += netForce * (get_invMass() * dt);
	angVelocity += netTorque * (get_invInertia() * dt);

	//-------------------------------------------------------
	// clear forces
	//-------------------------------------------------------
	netForce.set_zero();
	netTorque = 0.0f;
}

void RigidBody::initialize()
{
	vertices_.clear();
	init_vertices();

	netForce.set_zero();
	netTorque = 0.f;
	velocity.set_zero();
	
	set_density(density_);

	angVelocity = 0.f; //frand(3.0f) + 1.0f;
	angOrientation = 0.f;

	set_orientation(0.0f);
}

void RigidBody::init_vertices()
{
	vertices_.clear();

	vec3 scale = transform->scale;
	vec3 pos = transform->position;
	float rad = transform->get_euler_rad().z;

	vec3 lt(pos.x - scale.x, pos.y + scale.y, 0.f);
	vec3 rt(pos.x + scale.x, pos.y + scale.y, 0.f);
	vec3 rb(pos.x + scale.x, pos.y - scale.y, 0.f);
	vec3 lb(pos.x - scale.x, pos.y - scale.y, 0.f);

	vertices_.emplace_back(vec3::rotate(lt, pos, rad));
	vertices_.emplace_back(vec3::rotate(rt, pos, rad));
	vertices_.emplace_back(vec3::rotate(rb, pos, rad));
	vertices_.emplace_back(vec3::rotate(lb, pos, rad));
}

jeEnd