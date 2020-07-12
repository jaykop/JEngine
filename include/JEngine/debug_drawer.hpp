#pragma once

#include <vector>
#include <Vertex.hpp>
#include <renderer.hpp>

jeBegin

class Mesh;
class Shader;
class Object;
class Transform;

class DebugDrawer : public Renderer
{
	struct DebugMesh
	{
		Mesh* mesh;
		vec3 color;
	};

public:

	DebugDrawer(Object* owner);
	virtual ~DebugDrawer(void);

	void draw() override;

	void add_line(const vec3& start, const vec3& end, const vec3& color);
	void add_mesh(Mesh* m, const mat4& model_to_world, const vec3& color);

	void render_lines(Shader* pShader, const mat4& model);
	void render_meshes(Shader* pShader);

	void clear();

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& data);

private:

	unsigned vao_, vbo_;

	std::vector<Vertex> vertices_;
	std::vector<DebugMesh> meshes_;

};

jeEnd