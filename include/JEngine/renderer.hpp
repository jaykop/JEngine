#pragma once
#include <component_builder.hpp>
#include <component.hpp>

struct vec3;

jeBegin

class Light;
class Shader;
class Transform;

class Renderer : public Component {

	jeBaseFriends(Renderer);
	friend class GraphicSystem;

public:

	enum class ProjectType { PERSPECTIVE, ORTHOGONAL };
	enum class RenderType { NONE, POSITION, NORMAL };

	const static int IS_FIXED = 0x1000;
	const static int IS_FLIPPED = 0x0100;
	const static int IS_BILBOARD = 0x0010;
	const static int IS_INHERITED = 0x0001;

	ProjectType prjType;
	int status;

	Renderer(Object* owner);
	virtual ~Renderer() {};

	// public methods
	virtual void draw(float dt) = 0;

	static void draw_lighting_effect(Light* light);
	bool picked() const;

protected:

	virtual void add_to_system() = 0;
	virtual void remove_from_system() = 0;
	virtual void load(const rapidjson::Value& data) = 0;

	// private members
	unsigned drawMode_;
	unsigned sfactor_, dfactor_;

	Transform* transform_ = nullptr;
	Renderer* parent_ = nullptr;

	// static members
	static bool renderObj_;
	static RenderType renderType_;

private:

};

jeEnd