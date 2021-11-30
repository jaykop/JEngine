#pragma once
#include "scene.hpp"

jeBegin

// Generic scene class
class Level7 : public Scene
{
	friend class SceneManager;

public:

	Level7(const char* name, const char* dir) : Scene(name, dir) {};
	virtual ~Level7() {};

protected:

	void initialize() override;
	void update(float dt) override;
	void close() override;

private:

	void init_basic();
	void init_sprite();
	void init_ui();

	Object* renderer_2d = nullptr;

	std::vector<int> spherePos;
	std::vector<Object*> lights;

};

jeEnd