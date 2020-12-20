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

	float speed = 50.f;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	vec3 currentPos_, nextPos_, dist_;
	bool moveByGrid_ = false, moving_ = false;
	PlayerAttack& operator=(const PlayerAttack& rhs);
	PlayerAttack() = delete;

};

jeEnd