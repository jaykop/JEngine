#pragma once
#include "scene.hpp"

jeBegin

class assimpModel;

// Generic scene class
class Level5 : public Scene
{
	friend class SceneManager;

public:

	Level5(const char* name, const char* dir) : Scene(name, dir) {};
	virtual ~Level5() {};

protected:

	void initialize() override;
	void update(float dt) override;
	void close() override;

private:

	void init_basic();
	void init_lights();

	Object* light = nullptr, * model = nullptr;
};

jeEnd