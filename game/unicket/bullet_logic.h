#pragma once
#include "pch.h"

jeBegin

class Object;

jeDeclareUserComponentBuilder(BulletLogic);

class BulletLogic : public Behavior {

public:

	BulletLogic(Object* owner) : Behavior(owner) {};
	virtual ~BulletLogic();

	void init() override;
	void update(float dt) override;
	void close() override;

	float speed;
	vec3 vel, startPos;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	BulletLogic& operator=(const BulletLogic& rhs);
	BulletLogic() = delete;

};

jeEnd