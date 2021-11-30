#pragma once
#include "scene.hpp"

jeBegin

// Generic scene class
class Level9 : public Scene
{
	friend class SceneManager;

public:

	Level9(const char* name, const char* dir) : Scene(name, dir) {};
	virtual ~Level9() {};

protected:

	void initialize() override;
	void update(float dt) override;
	void close() override;

private:

	void init_basic();
	void init_emitters();

	Object* emitter1 = nullptr;

};

jeEnd