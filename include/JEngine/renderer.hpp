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
	enum class RenderType { NONE, POSITION, NORMAL, DIFFUSE };

	// static members
	static bool renderObj;
	static RenderType renderType;

	const static unsigned IS_FIXED		= 0x000001;
	const static unsigned IS_FLIPPED	= 0x000010;
	const static unsigned IS_LIGHTEN	= 0x000100;
	const static unsigned IS_BILBOARD	= 0x001000;
	const static unsigned IS_REFLECTED	= 0x010000;
	const static unsigned IS_REFRACTED	= 0x100000;

	ProjectType prjType;
	unsigned status;

	Renderer(Object* owner);
	virtual ~Renderer() {};

	// public methods
	static void draw_lighting_effect(Light* light);
	
	bool picked(vec3* hitPoint = nullptr) const;
	Transform* get_transform() const { return transform_; }

protected:

	virtual void add_to_system() = 0;
	virtual void remove_from_system() = 0;
	virtual void load(const rapidjson::Value& data) = 0;

	virtual void draw(float dt) = 0;
	void set_parent_renderer();

	unsigned drawMode_;
	unsigned sfactor_, dfactor_;

	Transform* transform_ = nullptr;
	Renderer* parent_ = nullptr;

private:

};

jeEnd