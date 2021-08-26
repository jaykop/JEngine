#pragma once
#include "pch.h"

jeBegin

class Object;

jeDeclareUserComponentBuilder(ExplodingParticleController);

class ExplodingParticleController : public Behavior {

public:

	ExplodingParticleController(Object* owner) : Behavior(owner) {};
	virtual ~ExplodingParticleController();

	void init() override;
	void update(float dt) override;
	void close() override;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

};

jeEnd