#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <vector>
#include <vec3.hpp>

jeBegin

class RigidBody;
class Contact : public Component {

	jeBaseFriends(Contact);
	friend class PhysicsSystem;

public:

	Contact(Object* owner);
	Contact(const vec3& va, const vec3& vb, int iCnum, const vec3& N, float t, RigidBody* ba, RigidBody* bb);

	virtual ~Contact();

	void reset();
	void solve();

	int maxContacts = 2;

	RigidBody* bodies[2] = {nullptr, nullptr};
	std::vector<std::vector<vec3>> contacts;
	vec3 normal;
	float t;
	int iNumContacts = 0;

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& /*data*/) {};

private:

	RigidBody* rigidBody_;

	void resolve_collision();
	void resolve_overlap();

	void resolve_collision(const vec3& a, const vec3& b);
	void resolve_overlap(const vec3& a, const vec3& b);
	void add_contact_pair(const vec3& a, const vec3& b);

};

jeDeclareComponentBuilder(Contact);

jeEnd