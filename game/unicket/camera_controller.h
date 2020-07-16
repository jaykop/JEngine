#pragma once
#include "pch.h"

jeBegin

class Object;

jeDeclareUserComponentBuilder(CameraController);

class CameraController : public Behavior {

public:

	CameraController(Object* owner) : Behavior(owner) {};
	virtual ~CameraController();

	void init() override;
	void update(float dt) override;
	void close() override;

	float speed = 50.f;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

};

jeEnd