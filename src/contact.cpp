#include <contact.hpp>
#include <physics_system.hpp>
#include <transform.hpp>
#include <object.hpp>
#include <rigidbody.hpp>
#include <material.hpp>

jeBegin

jeDefineComponentBuilder(Contact);

Contact::Contact(Object* owner) : Component(owner) {}

Contact::~Contact() {}

void Contact::add_to_system()
{

	for (int i = 0; i < maxContacts; ++i)
	{
		std::vector<vec3> v;
		v.emplace_back();
		v.emplace_back();
		contacts.emplace_back(v);
	}
}

void Contact::remove_from_system()
{
}


void Contact::reset()
{
	bodies[0] = bodies[1] = nullptr;
	iNumContacts = 0;
}

void Contact::solve()
{
	if (t < 0.0f)
		resolve_overlap();

	resolve_collision();
}

void Contact::resolve_collision()
{
	if (!bodies[0] || !bodies[1])
		return;

	for (int i = 0; i < iNumContacts; i++)
	{
		resolve_collision(contacts[i][0], contacts[i][1]);
	}
}

void Contact::resolve_overlap()
{
	if (!bodies[0] || !bodies[1])
		return;

	for (int i = 0; i < iNumContacts; i++)
	{
		resolve_overlap(contacts[i][0], contacts[i][1]);
	}
}

void Contact::resolve_collision(const vec3& a, const vec3& b)
{
	float m0 = bodies[0]->get_invMass();
	float m1 = bodies[1]->get_invMass();
	float i0 = bodies[0]->GetInvInertia();
	float i1 = bodies[1]->GetInvInertia();

	const vec3& P0 = bodies[0]->transform->position;
	const vec3& P1 = bodies[1]->transform->position;
	vec3& V0 = bodies[0]->GetLinVelocity();
	vec3& V1 = bodies[1]->GetLinVelocity();
	float& w0 = bodies[0]->GetAngVelocity();
	float& w1 = bodies[1]->GetAngVelocity();

	float fCoR = PhysicsSystem::contactMaterial_->restitution;
	float fCoF = PhysicsSystem::contactMaterial_->friction;

	::ResolveCollision(-normal, t, fCoF, fCoR,
		b, P1, V1, w1, m1, i1,
		a, P0, V0, w0, m0, i0);
}

void Contact::resolve_overlap(const vec3& a, const vec3& b)
{
	float m0 = bodies[0]->get_invMass();
	float m1 = bodies[1]->get_invMass();
	float m = m0 + m1;

	vec3 D = b - a;
	float fRelaxation = PhysicsSystem::contactMaterial_->separation;

	D *= fRelaxation;
	vec3 D0, D1;
	D0.set_zero();
	D1.set_zero();

	if (m0 > 0.0f)
	{
		D0 = D * (m0 / m);
		bodies[0]->transform->position += D0;
	}
	if (m1 > 0.0f)
	{
		D1 = D * -(m1 / m);
		bodies[1]->transform->position += D1;
	}
}

void Contact::add_contact_pair(const vec3& a, const vec3& b)
{
	if (iNumContacts >= maxContacts)
		return;

	contacts[iNumContacts][0] = a;
	contacts[iNumContacts][1] = b;
	++iNumContacts;
}

jeEnd