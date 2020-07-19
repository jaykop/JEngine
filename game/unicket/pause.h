#pragma once
#include "scene.hpp"

jeBegin

// Generic scene class
class Pause : public Scene
{
	friend class SceneManager;

public:

	Pause(const char* name, const char* dir) : Scene(name, dir) {};
	virtual ~Pause() {};

protected:

	void initialize() override;
	void update(float dt) override;
	void close() override;

private:

	void init_basic();
	void init_sprite();

	Object* renderer_2d = nullptr;

};

jeEnd