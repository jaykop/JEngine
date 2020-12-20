#include "player_attack.h"
#include "emitter.hpp"

jeBegin

jeDefineUserComponentBuilder(PlayerAttack);

PlayerAttack::~PlayerAttack()
{
	remove_from_system();
}

void PlayerAttack::init() { }

void PlayerAttack::update(float /*dt*/)
{}

void PlayerAttack::close() { }

jeEnd