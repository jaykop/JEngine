#pragma once
#include <macros.hpp>
#include <vector>
#include <vec3.hpp>

jeBegin

class RigidBody;
class Contact {

	friend class PhysicsSystem;

public:

	Contact();
	virtual ~Contact();

	void reset();
	void solve();

	int maxContacts = 2;

	RigidBody* bodies[2] = {nullptr, nullptr};
	std::vector<std::vector<vec3>> contacts;
	vec3 normal;
	int iNumContacts = 0;

	static bool useFriction;

protected:

private:

	Contact(const std::vector<vec3>& va, const std::vector<vec3>& vb, int iCnum, const vec3& N, float t, RigidBody* ba, RigidBody* bb);

	RigidBody* rigidBody_;
	float t_;

	void resolve_collision();
	void resolve_overlap();

	void resolve_collision(const vec3& a, const vec3& b);
	void resolve_overlap(const vec3& a, const vec3& b);
	void add_contact_pair(const vec3& a, const vec3& b);

	static void resolve_collision(const vec3& Ncoll, float t, float fCoF, float fCoR, const vec3& C0, const vec3& P0, vec3& V0, float& w0, float m0, float i0, const vec3& C1, const vec3& P1, vec3& V1, float& w1, float m1, float i1);

};

jeEnd