#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <vec4.hpp>
#include <mat4.hpp>

jeBegin

class Light;
class Camera;
class Shader;
class Transform;

class Renderer : public Component {

	jeBaseFriends(Renderer);
	friend class GraphicSystem;

public:

	enum class ProjectType { PERSPECTIVE, ORTHOGONAL };
	enum class RenderType { NONE, POSITION, NORMAL };

	Renderer(Object* owner);
	virtual ~Renderer() {};

	// public methods
	virtual void start_draw(Camera* camera, const mat4& perspective, const vec3& resScalar) = 0;
	virtual void draw() = 0;
	virtual void end_draw() = 0;

	static void draw_lighting_effect(Light* light);

protected:

	virtual void add_to_system() = 0;
	virtual void remove_from_system() = 0;
	virtual void load(const rapidjson::Value& data) = 0;

	const static int IS_FLIPPED = 0x100;
	const static int IS_BILBOARD = 0x010;
	const static int IS_INHERITED = 0x001;

	// private members
	unsigned drawMode_;
	unsigned sfactor_, dfactor_;

	Transform* transform_ = nullptr;

	ProjectType prjType_;
	int status_;

	// static members
	static bool renderObj_;
	static RenderType renderType_;

private:

};

jeEnd