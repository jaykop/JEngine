#pragma once

#include <vector>
#include <Vertex.hpp>

jeBegin

class Mesh;
class Shader;
class Object;
class Transform;

class DebugDrawer 
{
	struct DebugMesh
	{
		Mesh* mesh;
		vec3 color;
	};

	friend class Renderer;
	friend class Sprite;

public:

	DebugDrawer();
	virtual ~DebugDrawer(void);

	void add_line(const vec3& start, const vec3& end, const vec3& color);
	void add_quad(const vec3& pos, const vec3& size, const vec3& color);
	void add_mesh(Mesh* m, /*const mat4& model_to_world,*/ const vec3& color);

private:

	void draw_debugInfo();
	void render_lines();
	void render_meshes();
	void clear();

	unsigned vao_, vbo_, texture_;

	std::vector<Vertex> vertices_;
	std::vector<DebugMesh> meshes_;

};

jeEnd