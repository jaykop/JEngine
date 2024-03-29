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
	netForce.z = 0.f;
}

void RigidBody::add_force(const vec3& f, const vec3& p)
{
	if (isStatic) return;

	vec3 force = f;
	force.z = 0.f;
	netForce += f;
	netTorque += ((p - transform->position).cross(force)).z;
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
	transform->orientation = mat4::rotate_z(Math::deg_to_rad(angOrientation));
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

	else if (gravity != 0.f)
	{
		vec3 g;
		g.y = -(gravity * mass);

		add_force(g);
	}

	//-------------------------------------------------------
	// Integrate position (verlet integration)
	//-------------------------------------------------------
	transform->position += velocity * dt;
	angOrientation = Math::rad_to_deg(transform->orientation.get_euler().z);
	angOrientation += angVelocity * dt;
	angOrientation = Math::wrap_angle(angOrientation);
	transform->orientation = mat4::rotate_z(Math::deg_to_rad(angOrientation));

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
	set_orientation(Math::rad_to_deg(transform->orientation.get_euler().z));
}

void RigidBody::init_vertices()
{
	vertices_.clear();

	vec3 scale = transform->scale * 0.5f;

	vec3 lb(-scale.x, -scale.y, 0.f);
	vec3 rb(scale.x, - scale.y, 0.f);
	vec3 rt(scale.x, scale.y, 0.f);
	vec3 lt(-scale.x, scale.y, 0.f);

	vertices_.emplace_back(lt);
	vertices_.emplace_back(rt);
	vertices_.emplace_back(rb);
	vertices_.emplace_back(lb);
}

void RigidBody::add_collided(RigidBody* body)
{
	collided_.push_back(body);
}

void RigidBody::remove_force()
{
	velocity.set_zero();
	angVelocity = 0.f;
}

bool RigidBody::is_collided()
{
	return !collided_.empty();
}

bool RigidBody::is_collided_with(RigidBody* body)
{
	for (auto& b : collided_)
	{
		if (b == body)
			return true;
	}

	return false;
}

std::vector<RigidBody*> RigidBody::get_collided() const
{
	return collided_;
}

void RigidBody::clear_collided()
{
	collided_.clear();
}

jeEnd