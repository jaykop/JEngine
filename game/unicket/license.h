#pragma once
#include "scene.hpp"

jeBegin

// Generic scene class
class License : public Scene
{
	friend class SceneManager;

public:

	License(const char* name, const char* dir) : Scene(name, dir) {};
	virtual ~License() {};

protected:

	void initialize() override;
	void update(float dt) override;
	void close() override;

private:

	void add_camera();
	void add_fmod();
	void add_rapidjson();
	void add_freetype();

	Object *fmod = nullptr, *rj = nullptr, *ft = nullptr;
	Object* camera = nullptr;

};

jeEnd