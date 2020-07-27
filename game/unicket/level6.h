#pragma once
#include "scene.hpp"

jeBegin

// Generic scene class
class Level6 : public Scene
{
	friend class SceneManager;

public:

	Level6(const char* name, const char* dir) : Scene(name, dir) {};
	virtual ~Level6() {};

protected:

	void initialize() override;
	void update(float dt) override;
	void close() override;

private:

	void init_basic();
	void init_models();

};

jeEnd