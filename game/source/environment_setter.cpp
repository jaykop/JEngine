#include "environment_setter.h"
#include "object.hpp"
#include "sprite.hpp"
#include "colors.hpp"
#include "rigidbody.hpp"

jeBegin

jeDefineUserComponentBuilder(EnvironmentSetter);

const float LENGTH = 300, THICK = 10;

EnvironmentSetter::~EnvironmentSetter()
{
	remove_from_system();
}

void EnvironmentSetter::init()
{
	left_ = ObjectManager::create_object("leftWall");
	ls_ = left_->add_component<Sprite>();
	ls_->set_texture(AssetManager::get_texture("rect"));
	ls_->color = vec4(Color::blue, 1.f);
	Transform* transform = left_->get_component<Transform>();
	transform->position.set(-110, 0, 0);
	transform->scale.set(THICK, LENGTH, 0);
	lb_ = left_->add_component<RigidBody>();
	lb_->isStatic = true;
	lb_->collisionType_ = RigidBody::ColliderType::RECT;
	SceneManager::get_current_scene()->register_object(left_);

	right_ = ObjectManager::create_object("rightWall");
	rs_ = right_->add_component<Sprite>();
	rs_->set_texture(AssetManager::get_texture("rect"));
	rs_->color = vec4(Color::red, 1.f);
	transform = right_->get_component<Transform>();
	transform->position.set(110, 0, 0);
	transform->scale.set(THICK, LENGTH, 0);
	rb_ = right_->add_component<RigidBody>();
	rb_->isStatic = true;
	rb_->collisionType_ = RigidBody::ColliderType::RECT;
	SceneManager::get_current_scene()->register_object(right_);

	top_ = ObjectManager::create_object("topWall");
	ts_ = top_->add_component<Sprite>();
	ts_->set_texture(AssetManager::get_texture("rect"));
	ts_->color = vec4(Color::black, 1.f);
	transform = top_->get_component<Transform>();
	transform->position.set(0, 80, 0);
	transform->scale.set(LENGTH, THICK, 0);
	tb_ = top_->add_component<RigidBody>();
	tb_->isStatic = true;
	tb_->collisionType_ = RigidBody::ColliderType::RECT;
	SceneManager::get_current_scene()->register_object(top_);

	bottom_ = ObjectManager::create_object("bottomWall");
	bs_ = bottom_->add_component<Sprite>();
	bs_->set_texture(AssetManager::get_texture("rect"));
	bs_->color = vec4(Color::yellow, 1.f);
	transform = bottom_->get_component<Transform>();
	transform->position.set(0, -80, 0);
	transform->scale.set(LENGTH, THICK, 0);
	bb_ = bottom_->add_component<RigidBody>();
	bb_->isStatic = true;
	bb_->collisionType_ = RigidBody::ColliderType::RECT;
	SceneManager::get_current_scene()->register_object(bottom_);

	owner_->add_child(left_);
	owner_->add_child(right_);
	owner_->add_child(top_);
	owner_->add_child(bottom_);
}

void EnvironmentSetter::update(float dt)
{
	if (lb_->is_collided())
	{
		auto collided = lb_->get_collided();
		for (auto& b : collided)
		{
			Sprite* sp = b->get_owner()->get_component<Sprite>();
			sp->color = ls_->color;
		}
	}

	if (rb_->is_collided())
	{
		auto collided = rb_->get_collided();
		for (auto& b : collided)
		{
			Sprite* sp = b->get_owner()->get_component<Sprite>();
			sp->color = rs_->color;
		}
	}

	if (tb_->is_collided())
	{
		auto collided = tb_->get_collided();
		for (auto& b : collided)
		{
			Sprite* sp = b->get_owner()->get_component<Sprite>();
			sp->color = ts_->color;
		}
	}

	if (bb_->is_collided())
	{
		auto collided = bb_->get_collided();
		for (auto& b : collided)
		{
			Sprite* sp = b->get_owner()->get_component<Sprite>();
			sp->color = bs_->color;
		}
	}
}

void EnvironmentSetter::close() { }

jeEnd