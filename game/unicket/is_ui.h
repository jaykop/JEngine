#pragma once
#include "pch.h"

jeBegin

class Object;

jeDeclareUserComponentBuilder(IsUI);

class IsUI : public Behavior {

public:

	IsUI(Object* owner) : Behavior(owner) {};
	virtual ~IsUI();

	void init() override;
	void update(float dt) override;
	void close() override;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	vec3 initPos;

	IsUI& operator=(const IsUI& rhs);
	IsUI() = delete;

};

jeEnd