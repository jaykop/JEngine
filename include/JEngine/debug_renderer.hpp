#pragma once

#include <renderer.hpp>
#include <Vertex.hpp>
#include <vector>

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

	void start_draw(Camera* camera, const mat4& perspective, const vec3& resScalar) override;
	void end_draw() override;

	void draw() override;

	void add_line(const vec3& start, const vec3& end, const vec3& color);
	void add_quad(const vec3& pos, const vec3& size, const vec3& color);
	void add_mesh(Mesh* m, /*const mat4& model_to_world,*/ const vec3& color);

protected:

	void add_to_system() override;
	void remove_from_system() override;
	void load(const rapidjson::Value& data) override;

private:

	void clear();

	unsigned vao_, vbo_, texture_;

	std::vector<Vertex> vertices_;
	std::vector<DebugMesh> meshes_;

};

jeDeclareComponentBuilder(DebugRenderer);

jeEnd