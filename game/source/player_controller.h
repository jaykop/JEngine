#pragma once
#include "pch.h"

jeBegin

class Object;
class RigidBody;
class Transform;

jeDeclareUserComponentBuilder(PlayerController);

class PlayerController : public Behavior {

public:

	enum class ROTATION_MODE {MOUSE, KEYBOARD};
	enum class MOVEMENT_MODE {WORLD, GRID, IMPULSE};

	PlayerController(Object* owner) : Behavior(owner) {};
	virtual ~PlayerController();

	void init() override;
	void update(float dt) override;
	void close() override;

	float speed = 100.f;
	float slidingTime = 3.f;

	ROTATION_MODE rotationMode_ = ROTATION_MODE::MOUSE;
	MOVEMENT_MODE movementMode_ = MOVEMENT_MODE::WORLD;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	Transform* transform_ = nullptr;
	RigidBody* body_ = nullptr;
	vec3 currentPos_, nextPos_, dist_;
	bool moving_ = false;
	bool sliding_ = false;

	PlayerController& operator=(const PlayerController& rhs);
	PlayerController() = delete;

	void fix_camera();

	void move_gridbase();
	void move_worldbase(float dt);
	void move_impulsebase(float dt);

	void rotate_mousebase(float dt);
	void rotate_keyboardbase(float dt);
};

jeEnd