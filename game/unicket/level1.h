#pragma once
#include "scene.hpp"

jeBegin

// Generic scene class
class Level1 : public Scene
{
	friend class SceneManager;

public:

	Level1(const char* name, const char* dir) : Scene(name, dir) {};
	virtual ~Level1() {};

protected:

	void initialize() override;
	void update(float dt) override;
	void close() override;

private:

	void init_basic();
	void init_sprite();
	void init_text();
	void init_emitters();

	Object* renderer_2d = nullptr, * rand_obj = nullptr;
	Object* emitter1 = nullptr;

};

jeEnd