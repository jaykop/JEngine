#pragma once
#include "pch.h"

jeBegin

class Object;

jeDeclareUserComponentBuilder(TopDownController);

class TopDownController : public Behavior {

public:

	TopDownController(Object* owner) : Behavior(owner) {};
	virtual ~TopDownController();

	void init() override;
	void update(float dt) override;
	void close() override;

	float speed = 50.f;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	TopDownController& operator=(const TopDownController& rhs);
	TopDownController() = delete;

};

jeEnd