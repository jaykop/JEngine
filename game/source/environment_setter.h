#pragma once
#include "pch.h"

jeBegin

class Object;

jeDeclareUserComponentBuilder(EnvironmentSetter);

class EnvironmentSetter : public Behavior {

public:

	EnvironmentSetter(Object* owner) : Behavior(owner) {};
	virtual ~EnvironmentSetter();

	void init() override;
	void update(float dt) override;
	void close() override;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	EnvironmentSetter& operator=(const EnvironmentSetter& rhs);
	EnvironmentSetter() = delete;

	Object *left_, *right_, *top_, *bottom_;

};

jeEnd