#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <vector>
#include <vec3.hpp>

jeBegin

class Transform;
class Collider2D : public Component {

	jeBaseFriends(Collider2D);
	friend class PhysicsSystem;

public:

	Collider2D(Object* owner);
	virtual ~Collider2D();

	enum class ColliderType {
		NONE,
		CIRCLE,
		RECT,
	};

	ColliderType  coliisionType_ = ColliderType::NONE;
	Transform* transform;

	void init_vertices();

protected:

	virtual void add_to_system();
	virtual void remove_from_system() {};
	virtual void load(const rapidjson::Value& /*data*/) {};

private:

	std::vector<vec3> vertices_;

};

jeDeclareComponentBuilder(Collider2D);

jeEnd