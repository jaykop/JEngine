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

	float speed = 25.f;
	bool camera_2d = true;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	void move(float dt);
	void rotate(float dt);

	bool firstMouse = true;
	float lastX, lastY, dx, dy, last_dx, last_dy;
	float sensitivity = 5.f;
	// const float maxOffset = 10.f;

};

jeEnd