#pragma once
#include <vec4.hpp>
#include <renderer.hpp>

jeBegin

class Object;
class Animation2D;

class Sprite : public Renderer
{
	jeBaseFriends(Sprite);
	friend class Animation2D;
	friend class GraphicSystem;

public:

	Sprite(Object* owner);
	virtual ~Sprite() {}

	void set_texture(unsigned texture);
	unsigned get_texture() const;

	void run_animation();

	void draw() override;

	vec4 color;
	
protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& data);

private:

	Animation2D* animation_ = nullptr;
	unsigned texture_;
};

jeDeclareComponentBuilder(Sprite);

jeEnd