#pragma once
#include "pch.h"

jeBegin

class Object;
class RigidBody;

jeDeclareUserComponentBuilder(PlayerController);

class PlayerController : public Behavior {

public:

	PlayerController(Object* owner) : Behavior(owner) {};
	virtual ~PlayerController();

	void init() override;
	void update(float dt) override;
	void close() override;

	float speed = 100.f;
	float slidingTime = 3.f;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	RigidBody* body_ = nullptr;
	vec3 currentPos_, nextPos_, dist_;
	bool moveByGrid_ = false, moving_ = false;
	bool sliding_ = false;

	PlayerController& operator=(const PlayerController& rhs);
	PlayerController() = delete;

};

jeEnd