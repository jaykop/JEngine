#pragma once
#include "scene.hpp"

jeBegin

// Generic scene class
class Level1 : public Scene
{
	// Only SceneManager can manage this class
	friend class SceneManager;

public:

	Level1(const char* name) : Scene(name) {};
	virtual ~Level1() {};

protected:

	void load() override;
	void initialize() override;
	void update(float dt) override;
	void close() override;
	void unload() override;

private:

};

jeEnd