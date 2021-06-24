#include <collider_2d.hpp>
#include <physics_system.hpp>
#include <transform.hpp>
#include <object.hpp>

jeBegin

jeDefineComponentBuilder(Collider2D);

Collider2D::Collider2D(Object* owner) : Component(owner) {}

Collider2D::~Collider2D() {}

void Collider2D::add_to_system()
{
	transform = get_owner()->get_component<Transform>();
	init_vertices();
	PhysicsSystem::add_collider(this);
}

void Collider2D::remove_from_system()
{
	vertices_.clear();
}

void Collider2D::init_vertices()
{
	vertices_.clear();

	vec3 scale = transform->scale;
	vec3 pos = transform->position;
	float rad = transform->rotation_euler_rad().z;

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