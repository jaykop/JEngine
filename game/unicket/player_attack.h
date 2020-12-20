#pragma once
#include "pch.h"

jeBegin

class Object;

jeDeclareUserComponentBuilder(PlayerAttack);

class PlayerAttack : public Behavior {

public:

	PlayerAttack(Object* owner) : Behavior(owner) {};
	virtual ~PlayerAttack();

	void init() override;
	void update(float dt) override;
	void close() override;

	float speed = 300.f;
	int ammo_ = 1000;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	PlayerAttack& operator=(const PlayerAttack& rhs);
	PlayerAttack() = delete;

};

jeEnd