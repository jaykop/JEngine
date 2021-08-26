#pragma once
#include "scene.hpp"

jeBegin

// Generic scene class
class Level2 : public Scene
{
	friend class SceneManager;

public:

	Level2(const char* name, const char* dir) : Scene(name, dir) {};
	virtual ~Level2() {};

protected:

	void initialize() override;
	void update(float dt) override;
	void close() override;

private:

	void init_basic();

	Object* scriptController = nullptr;

};

jeEnd