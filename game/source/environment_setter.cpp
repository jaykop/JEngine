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
	Sprite* sprite = left_->add_component<Sprite>();
	sprite->set_texture(AssetManager::get_texture("rect"));
	sprite->color = vec4(Color::blue, 1.f);
	Transform* transform = left_->get_component<Transform>();
	transform->position.set(-110, 0, 0);
	transform->scale.set(THICK, LENGTH, 0);
	RigidBody* rigidbody = left_->add_component<RigidBody>();
	rigidbody->isStatic = true;
	SceneManager::get_current_scene()->register_object(left_);

	right_ = ObjectManager::create_object("rightWall");
	sprite = right_->add_component<Sprite>();
	sprite->set_texture(AssetManager::get_texture("rect"));
	sprite->color = vec4(Color::red, 1.f);
	transform = right_->get_component<Transform>();
	transform->position.set(110, 0, 0);
	transform->scale.set(THICK, LENGTH, 0);
	rigidbody = right_->add_component<RigidBody>();
	rigidbody->isStatic = true;
	SceneManager::get_current_scene()->register_object(right_);

	top_ = ObjectManager::create_object("topWall");
	sprite = top_->add_component<Sprite>();
	sprite->set_texture(AssetManager::get_texture("rect"));
	sprite->color = vec4(Color::white, 1.f);
	transform = top_->get_component<Transform>();
	transform->position.set(0, 80, 0);
	transform->scale.set(LENGTH, THICK, 0);
	rigidbody = top_->add_component<RigidBody>();
	rigidbody->isStatic = true;
	SceneManager::get_current_scene()->register_object(top_);

	bottom_ = ObjectManager::create_object("bottomWall");
	sprite = bottom_->add_component<Sprite>();
	sprite->set_texture(AssetManager::get_texture("rect"));
	sprite->color = vec4(Color::yellow, 1.f);
	transform = bottom_->get_component<Transform>();
	transform->position.set(0, -80, 0);
	transform->scale.set(LENGTH, THICK, 0);
	rigidbody = bottom_->add_component<RigidBody>();
	rigidbody->isStatic = true;
	SceneManager::get_current_scene()->register_object(bottom_);

	owner_->add_child(left_);
	owner_->add_child(right_);
	owner_->add_child(top_);
	owner_->add_child(bottom_);
}

void EnvironmentSetter::update(float dt)
{

}

void EnvironmentSetter::close() { }

jeEnd