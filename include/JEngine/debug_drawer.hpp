#pragma once
//#include "math/mat4.h"
//#include "Vertex.h"
//#include <vector>
//
//struct OBB;
//struct AABB;
//struct BoundingSphere;
//struct BoundingEllipsoid;
//
//class Mesh;
//class Shader;
//class Transform;
//using line_vertex = std::vector<Vertex>;
//
//struct DebugMesh
//{
//public:
//
//	Mesh* mesh;
//	mat4 model_to_world;
//	vec3 color;
//};
//
//class DebugDrawer
//{
//
//public:
//	// ctor / dtor
//	DebugDrawer(void);
//	~DebugDrawer(void);
//
//	// public methods
//	void add_line(const vec3& start, const vec3& end, const vec3& color);
//	void AddCube(const vec3& pos, const vec3& size, const vec3& color);
//	void AddCube(const AABB& bounding_box, const vec3& color);
//	void AddCube(const OBB& bounding_box, const vec3& color);
//	void AddSphere(const vec3& center, float radius, const vec3& color);
//	void AddSphere(const BoundingSphere& sphere, const vec3& color);
//	void AddEllipse(const BoundingEllipsoid& ellips, bool h, const vec3& color);
//	void add_mesh(Mesh* m, const mat4& model_to_world, const vec3& color);
//
//	void render_lines(Shader* pShader, const mat4& model);
//	void render_meshes(Shader* pShader);
//
//	void clear();
//
//	Transform* pTrans_ = nullptr;
//
//private:
//
//	// private members
//	unsigned vao_, vbo_;
//
//	line_vertex vertex_list_;
//	std::vector<DebugMesh> meshes_;
//
//}; // class debug_drawer

