#pragma once
#include "scene.hpp"


jeBegin

class Object;
using ObjectMap = std::unordered_map<const char*, Object*>;

// Generic scene class
class TestScene : public Scene
{
	// Only SceneManager can manage this class
	friend class SceneManager;

public:

	TestScene(const char* name) : Scene(name) {};
	virtual ~TestScene() {};

protected:

	void initialize() override;
	void update(float dt) override;
	void close() override;

	void init_basic();
	void init_sprite();
	void init_text();
	void init_emitters();

private:

	Object* renderer_2d = nullptr, *rand_obj = nullptr;
	Object* emitter1 = nullptr;

};

jeEnd