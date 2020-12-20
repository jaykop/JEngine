#include "colors.hpp"
#include "player_attack.h"
#include "bullet_logic.h"
#include "emitter.hpp"
#include "debug_renderer.hpp"

jeBegin

jeDefineUserComponentBuilder(PlayerAttack);

PlayerAttack::~PlayerAttack()
{
	remove_from_system();
}

void PlayerAttack::init() { }

void PlayerAttack::update(float dt)
{
	if (InputHandler::key_triggered(KEY::MOUSE_LEFT) && ammo_)
	{
		static unsigned bulletCount = 0;
		// shoot a bullet
		Object* bullet = ObjectManager::create_object(std::string("bullet_" + std::to_string(bulletCount++)).c_str());
		bullet->add_component<Emitter>();
		bullet->add_component<BulletLogic>();
		Transform* bulletTrans = bullet->get_component<Transform>();
		bulletTrans->position = get_owner()->get_component<Transform>()->position;

		// set velocity
		vec3 vel = InputHandler::get_position() - bulletTrans->position;
		vel.normalize();
		bullet->get_component<BulletLogic>()->vel = vel;
		bullet->get_component<BulletLogic>()->speed = speed;

		// set emitter
		Emitter* emi = bullet->get_component<Emitter>();
		//emi->life = 100.f;
		//emi->set_colors(Color::yellow, Color::red);
		//emi->set_size(50);
		//emi->velocity = vel * speed;
		//emi->set_texture(AssetManager::get_texture("particle"));

		bullet->get_component<Transform>()->scale.set(1.f, 1.f, 0.f);
		emi->set_texture(AssetManager::get_texture("rect"));
		emi->set_colors(Color::yellow, Color::red);
		emi->active = true;
		emi->life = 1.f;
		emi->colorSpeed = 3.f;
		emi->velocity.set(15.f, 15.f, 0.f);
		emi->angle.set(0.f, 180.f);
		emi->set_size(50);

		// decrease the num of ammo
		--ammo_;

		// register the bullet
		SceneManager::get_current_scene()->register_object(bullet);
	}
}

void PlayerAttack::close() { }

jeEnd