#include <contact.hpp>
#include <physics_system.hpp>
#include <transform.hpp>
#include <rigidbody.hpp>
#include <material.hpp>

jeBegin

bool Contact::useFriction = false;

Contact::Contact() {

	for (int i = 0; i < maxContacts; ++i)
	{
		std::vector<vec3> v;
		v.emplace_back();
		v.emplace_back();
		contacts.emplace_back(v);
	}
}

Contact::Contact(const std::vector<vec3>& va, const std::vector<vec3>& vb, int iCnum, const vec3& N, float t, RigidBody* ba, RigidBody* bb)
{
	iNumContacts = 0;
	contacts.clear();
	bodies[0] = ba;
	bodies[1] = bb;
	normal = N;
	t_ = t;

	for (int i = 0; i < maxContacts; ++i)
	{
		std::vector<vec3> v;
		v.emplace_back();
		v.emplace_back();
		contacts.emplace_back(v);
	}

	for (int i = 0; i < iCnum; i++)
	{
		add_contact_pair(va[i], vb[i]);
	}
}

Contact::~Contact() {}

void Contact::reset()
{
	bodies[0] = bodies[1] = nullptr;
	iNumContacts = 0;
}

void Contact::solve()
{
	if (t_ < 0.0f)
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
	float i0 = bodies[0]->get_invInertia();
	float i1 = bodies[1]->get_invInertia();

	const vec3& P0 = bodies[0]->transform->position;
	const vec3& P1 = bodies[1]->transform->position;
	vec3& V0 = bodies[0]->velocity;
	vec3& V1 = bodies[1]->velocity;
	float& w0 = bodies[0]->angVelocity;
	float& w1 = bodies[1]->angVelocity;

	float fCoR = PhysicsSystem::contactMaterial_->restitution;
	float fCoF = PhysicsSystem::contactMaterial_->friction;

	Contact::resolve_collision(-normal, t_, fCoF, fCoR,
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

	if (m0 > 0.0f && !bodies[0]->isStatic)
	{
		D0 = D * (m0 / m);
		bodies[0]->transform->position += D0;
	}
	if (m1 > 0.0f && !bodies[1]->isStatic)
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

void Contact::resolve_collision(const vec3& Ncoll, float t, float fCoF, float fCoR,
	const vec3& C0, const vec3& P0, vec3& V0, float& w0, float m0, float i0,
	const vec3& C1, const vec3& P1, vec3& V1, float& w1, float m1, float i1)
{
	//------------------------------------------------------------------------------------------------------
	// pre-computations
	//------------------------------------------------------------------------------------------------------
	float tcoll = (t > 0.0f) ? t : 0.0f;

	vec3 Q0 = P0 + V0 * tcoll;
	vec3 Q1 = P1 + V1 * tcoll;
	vec3 R0 = C0 - Q0;
	vec3 R1 = C1 - Q1;
	vec3 T0(-R0.y, R0.x, 0.f);
	vec3 T1(-R1.y, R1.x, 0.f);
	vec3 VP0 = V0 - T0 * w0; // point velocity (SIGN IS WRONG)
	vec3 VP1 = V1 - T1 * w1; // point velocity (SIGN IS WRONG)

	//------------------------------------------------------------------------------------------------------
	// impact velocity
	//------------------------------------------------------------------------------------------------------
	vec3 Vcoll = VP0 - VP1;
	float  vn = Vcoll.dot(Ncoll);
	vec3 Vn = Ncoll * vn;
	vec3 Vt = Vcoll - Vn;

	// separation
	if (vn > 0.0f)
		return;

	float vt = Vt.length();
	Vt.normalize();

	//------------------------------------------------------------------------------------------------------
	// compute impulse (frction and restitution).
	// ------------------------------------------
	//
	//									-(1+Cor)(Vel.norm)
	//			j =  ------------------------------------------------------------
	//			     [1/Ma + 1/Mb] + [Ia' * (ra x norm)? + [Ib' * (rb x norm)?
	//------------------------------------------------------------------------------------------------------
	vec3 J, Jt, Jn;

	float r0_mul = R0.cross(Ncoll).z;
	float r1_mul = R1.cross(Ncoll).z;
	float t0 = r0_mul * r0_mul * i0;
	float t1 = r1_mul * r1_mul * i1;
	float m = m0 + m1;

	float denom = m + t0 + t1;

	float jn = vn / denom;

	Jn = Ncoll * (-(1.0f + fCoR) * jn);

	if (Contact::useFriction)
	{
		Jt = Vt.normalize() * (fCoF * jn);
	}

	J = Jn + Jt;

	//------------------------------------------------------------------------------------------------------
	// changes in momentum
	//------------------------------------------------------------------------------------------------------
	vec3 dV0 = J * m0;
	vec3 dV1 = -J * m1;

	float dw0 = -(R0.cross(J).z) * i0; // (SIGN IS WRONG)
	float dw1 = (R1.cross(J).z) * i1; // (SIGN IS WRONG)

	//------------------------------------------------------------------------------------------------------
	// apply changes in momentum
	//------------------------------------------------------------------------------------------------------
	if (m0 > 0.0f) V0 += dV0;
	if (m1 > 0.0f) V1 += dV1;
	if (m0 > 0.0f) w0 += dw0;
	if (m1 > 0.0f) w1 += dw1;

	//------------------------------------------------------------------------------------------------------
	// Check for static frcition
	//------------------------------------------------------------------------------------------------------
	if (vn < 0.0f && fCoF > 0.0f)
	{
		float cone = -vt / vn;

		if (cone < fCoF)
		{
			// treat static friciton as a collision at the contact point
			vec3 Nfriction = -Vt.normalize();
			float fCoS = PhysicsSystem::contactMaterial_->staticFriction;

			resolve_collision(Nfriction, 0.0f, 0.0f, fCoS,
				C0, P0, V0, w0, m0, i0,
				C1, P1, V1, w1, m1, i1);
		}
	}
}

jeEnd