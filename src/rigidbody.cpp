#include <physics_system.hpp>
#include <rigidbody.hpp>
#include <transform.hpp>
#include <object.hpp>

jeBegin

jeDefineComponentBuilder(RigidBody);

RigidBody::RigidBody(Object* owner) : Component(owner) {}

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

jeEnd