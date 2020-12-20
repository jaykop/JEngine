#pragma once
#include "pch.h"

jeBegin

class Object;

jeDeclareUserComponentBuilder(PlayerController);

class PlayerController : public Behavior {

public:

	PlayerController(Object* owner) : Behavior(owner) {};
	virtual ~PlayerController();

	void init() override;
	void update(float dt) override;
	void close() override;

	float speed = 50.f;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	vec3 currentPos_, nextPos_, dist_;
	bool moveByGrid_ = false, moving_ = false;
	PlayerController& operator=(const PlayerController& rhs);
	PlayerController() = delete;

};

jeEnd