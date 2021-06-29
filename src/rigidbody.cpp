#include <physics_system.hpp>
#include <rigidbody.hpp>
#include <transform.hpp>
#include <object.hpp>

jeBegin

jeDefineComponentBuilder(RigidBody);

RigidBody::RigidBody(Object* owner) 
	: Component(owner), friction(0.f), restitution(0.1f), glue(0.01f),
mass_(1.f), displacement_(vec3(0.f, 0.f, 0.f)) 
{}

RigidBody::~RigidBody() {}

void RigidBody::add_to_system()
{
	transform = get_owner()->get_component<Transform>();
	PhysicsSystem::add_rigidbody(this);
}

void RigidBody::remove_from_system() {}

void RigidBody::add_impulse(const vec3& force, float dt)
{
	if (isStatic) 
		return;

	displacement_ += force * (mass_ * dt * dt);
}

float RigidBody::get_invMass() const
{
	return (1.f / mass_);
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

jeEnd