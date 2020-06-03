#pragma once
#include "pch.h"

jeBegin

class Object;

jeDeclareUserComponentBuilder(SceneController);

class SceneController : public Behavior {

public:

	SceneController(Object* owner) : Behavior(owner) {};
	virtual ~SceneController() {};

	void init() override;
	void update(float dt) override;
	void close() override;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	SceneController& operator=(const SceneController& rhs);
	SceneController() = delete;

};

jeEnd