#pragma once

#include <renderer.hpp>
#include <Vertex.hpp>
#include <vector>

#include <vec3.hpp>

jeBegin

class Mesh;

class DebugRenderer : public Renderer {

	jeBaseFriends(DebugRenderer);

	struct DebugMesh
	{
		Mesh* mesh;
		vec3 color;
	};

	friend class Renderer;
	friend class Sprite;
	friend class GraphicSystem;

public:

	DebugRenderer(Object* owner);
	virtual ~DebugRenderer(void);
	
	// bool inside(const vec3& point);

	void add_line(const vec3& start, const vec3& end, const vec3& color);
	void add_quad(const vec3& pos, const vec3& size, const vec3& color);
	void add_mesh(Mesh* m, /*const mat4& model_to_world,*/ const vec3& color);

protected:

	void add_to_system() override;
	void remove_from_system() override;
	void load(const rapidjson::Value& data) override;

	void draw(float dt) override;

private:

	void clear();

	std::vector<Vertex> vertices_;
	std::vector<DebugMesh> meshes_;

};

jeDeclareComponentBuilder(DebugRenderer);

jeEnd