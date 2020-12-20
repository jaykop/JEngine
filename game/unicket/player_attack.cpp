#include "colors.hpp"
#include "player_attack.h"
#include "bullet_logic.h"
#include "emitter.hpp"
#include "debug_renderer.hpp"

#include <iostream>

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
		bullet->get_component<BulletLogic>()->startPos 
			= bulletTrans->position 
			= get_owner()->get_component<Transform>()->position;

		// set velocity
		bullet->get_component<BulletLogic>()->vel = InputHandler::get_position().normalized();
		bullet->get_component<BulletLogic>()->speed = speed;

		// set emitter
		Emitter* emi = bullet->get_component<Emitter>();
		bulletTrans->scale.set(3.f, 3.f, 0.f);
		emi->set_texture(AssetManager::get_texture("rect"));
		emi->set_colors(Color::red, Color::red);
		emi->active = true;
		emi->life = .5f;
		emi->colorSpeed = 3.f;
		//emi->velocity.set(15.f, 15.f, 0.f);
		//emi->angle.set(0.f, 180.f);
		emi->set_size(50);

		// decrease the num of ammo
		--ammo_;

		// register the bullet
		SceneManager::get_current_scene()->register_object(bullet);
	}

	if (ammo_ <= 0)
		std::cout << "Out of ammo\n";
}

void PlayerAttack::close() { }

jeEnd