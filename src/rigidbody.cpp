#include <physics_system.hpp>
#include <rigidbody.hpp>
#include <transform.hpp>
#include <object.hpp>
#include <math_util.hpp>

jeBegin

jeDefineComponentBuilder(RigidBody);

RigidBody::RigidBody(Object* owner) 
	: Component(owner), friction(0.f), restitution(0.1f), glue(0.01f),
mass(1.f), gravity(0.f), displacement_(vec3::zero) , angOrientation(0.f)
{}

RigidBody::~RigidBody() {}

bool RigidBody::is_unmovable()
{
	return (mass < Math::epsilon);
}

void RigidBody::add_to_system()
{
	transform = get_owner()->get_component<Transform>();
	init_vertices();
	PhysicsSystem::add_rigidbody(this);
}

void RigidBody::remove_from_system() 
{
	vertices_.clear();
}

void RigidBody::add_impulse(const vec3& force, float dt)
{
	if (isStatic) 
		return;

	displacement_ += force * (get_invMass() * dt * dt);
}

float RigidBody::get_invMass() const
{
	return (mass > 0.0f) ? (1.0f / mass) : 0.0f;
}

void RigidBody::add_force(const vec3& f)
{
	if (is_unmovable())
		return;

	netForce += f;
}

void RigidBody::add_force(const vec3& f, const vec3& p)
{
	if (is_unmovable()) return;

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
	orientation = mat4::rotate_z(angOrientation);
}

float RigidBody::get_invInertia()
{
	return (inertia > 0.0f) ? 1.0f / inertia : 0.0f;
}

void RigidBody::update(float dt)
{
	if (is_unmovable())
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
	orientation = mat4::rotate_z(Math::deg_to_rad(angOrientation));

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

// two objects collided at time t. stop them at that time
void RigidBody::process_collision(RigidBody* other, const vec3& N, float t)
{
	vec3 D = displacement_ - other->displacement_;

	float n = D.dot(N);

	vec3 Dn = N * n;
	vec3 Dt = D - Dn;

	if (n > 0.0f)
		Dn.set_zero();

	float dt = Dt.dot(Dt);
	float CoF = friction;

	if (dt < glue * glue) CoF = 1.01f;

	D = (Dn * -(1.0f + restitution)) - (Dt * CoF);

	float m0 = get_invMass();
	float m1 = other->get_invMass();
	float m = m0 + m1;
	float r0 = m0 / m;
	float r1 = m1 / m;

	displacement_ += D * r0;
	other->displacement_ -= D * r1;
}

// two objects overlapped. push them away from each other
void RigidBody::process_overlap(RigidBody* other, const vec3& xMTD)
{
	if (isStatic && !other->isStatic)
	{
		other->transform->position -= xMTD;
	}
	else if (!isStatic && other->isStatic)
	{
		transform->position += xMTD;
	}
	else if (!isStatic && !other->isStatic)
	{
		transform->position += xMTD * 0.5f;
		other->transform->position -= xMTD * 0.5f;
	}

	process_collision(other, xMTD.normalized(), 0.0f);
}

void RigidBody::initialize()
{
	vertices_.clear();

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