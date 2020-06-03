//#include "math/math.h"
//#include "math/quat.h"
//#include "DebugDrawer.h"
//#include "glad/glad.h"
//#include "Shader.h"
//#include "ObjLoader.h"
//#include "math/Transform.h"
//#include "Mesh.h"
//#include "ColorList.h"
//
//#include "bounding_volumes/aabb.h"
//#include "bounding_volumes/BoundingSphere.h"
//#include "bounding_volumes/obb.h"
//#include "bounding_volumes/BoundingEllipsoid.h"
//
//DebugDrawer::DebugDrawer(void)
//{
//	vertex_list_.clear();
//	meshes_.clear();
//
//	glGenVertexArrays(1, &vao_);
//	glGenBuffers(1, &vbo_);
//	glBindVertexArray(vao_);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
//		reinterpret_cast<void*>(offsetof(Vertex, Vertex::position)));
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
//		reinterpret_cast<void*>(offsetof(Vertex, Vertex::normal)));
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
//		reinterpret_cast<void*>(offsetof(Vertex, Vertex::color)));
//	glEnableVertexAttribArray(2);
//
//	glBindVertexArray(0);
//}
//
//DebugDrawer::~DebugDrawer(void)
//{
//	// destroy buffers
//	glDeleteBuffers(1, &vao_);
//	glDeleteBuffers(1, &vbo_);
//}
//
//void DebugDrawer::add_line(const vec3& start, const vec3& end, const vec3& color)
//{
//	// add line vertices
//	Vertex start_v, end_v;
//	start_v.position = start;
//	start_v.color = color;
//	end_v.position = end;
//	end_v.color = color;
//
//	vertex_list_.push_back(start_v);
//	vertex_list_.push_back(end_v);
//}
//
//void DebugDrawer::AddCube(const vec3& pos, const vec3& size, const vec3& color)
//{
//	// create 8 vertices
//	vec3 v1 = pos + vec3(-size.x, size.y, size.z);
//	vec3 v2 = pos + size;
//	vec3 v3 = pos + vec3(size.x, -size.y, size.z);
//	vec3 v4 = pos + vec3(-size.x, -size.y, size.z);
//	vec3 v5 = pos + vec3(-size.x, size.y, -size.z);
//	vec3 v6 = pos + vec3(size.x, size.y, -size.z);
//	vec3 v7 = pos + vec3(size.x, -size.y, -size.z);
//	vec3 v8 = pos - size;
//
//	// add edges
//	add_line(v1, v2, color);
//	add_line(v2, v3, color);
//	add_line(v3, v4, color);
//	add_line(v4, v1, color);
//
//	add_line(v5, v6, color);
//	add_line(v6, v7, color);
//	add_line(v7, v8, color);
//	add_line(v8, v5, color);
//
//	add_line(v1, v5, color);
//	add_line(v2, v6, color);
//	add_line(v3, v7, color);
//	add_line(v4, v8, color);
//}
//
//void DebugDrawer::AddCube(const AABB& bounding_box, const vec3& color)
//{
//	// create 8 vertices
//	vec3 v1 = bounding_box.min;
//	vec3 v2 = bounding_box.max;
//	vec3 v3(v1.x, v1.y, v2.z);
//	vec3 v4(v1.x, v2.y, v2.z);
//	vec3 v5(v2.x, v1.y, v1.z);
//	vec3 v6(v2.x, v2.y, v1.z);
//	vec3 v7(v2.x, v1.y, v2.z);
//	vec3 v8(v1.x, v2.y, v1.z);
//
//	// add edges
//	add_line(v1, v5, color);
//	add_line(v5, v6, color);
//	add_line(v6, v8, color);
//	add_line(v8, v1, color);
//
//	add_line(v3, v4, color);
//	add_line(v4, v2, color);
//	add_line(v2, v7, color);
//	add_line(v7, v3, color);
//
//	add_line(v1, v3, color);
//	add_line(v8, v4, color);
//	add_line(v6, v2, color);
//	add_line(v5, v7, color);
//}
//
//void DebugDrawer::AddCube(const OBB& bounding_box, const vec3& color)
//{
//	// create 8 vertices
//	vec3 v1(-bounding_box.half_widths);
//	vec3 v2(bounding_box.half_widths);
//	vec3 v3(v1.x, v1.y, v2.z);
//	vec3 v4(v1.x, v2.y, v2.z);
//	vec3 v5(v2.x, v1.y, v1.z);
//	vec3 v6(v2.x, v2.y, v1.z);
//	vec3 v7(v2.x, v1.y, v2.z);
//	vec3 v8(v1.x, v2.y, v1.z);
//
//	mat4 translate = mat4::identity;
//	translate.m03 = bounding_box.center.x;
//	translate.m13 = bounding_box.center.y;
//	translate.m23 = bounding_box.center.z;
//
//	mat4 rot = mat4::identity;
//
//	for (int i = 0; i < 3; ++i)
//		for (int j = 0; j < 3; ++j)
//			rot.m[i][j] = bounding_box.rotation.m[i][j];
//	translate = translate * rot;
//
//	// transform points
//	v1 = (translate * vec4(v1, 1.0f)).to_vec3();
//	v2 = (translate * vec4(v2, 1.0f)).to_vec3();
//	v3 = (translate * vec4(v3, 1.0f)).to_vec3();
//	v4 = (translate * vec4(v4, 1.0f)).to_vec3();
//	v5 = (translate * vec4(v5, 1.0f)).to_vec3();
//	v6 = (translate * vec4(v6, 1.0f)).to_vec3();
//	v7 = (translate * vec4(v7, 1.0f)).to_vec3();
//	v8 = (translate * vec4(v8, 1.0f)).to_vec3();
//
//	// add edges
//	add_line(v1, v5, color);
//	add_line(v5, v6, color);
//	add_line(v6, v8, color);
//	add_line(v8, v1, color);
//
//	add_line(v3, v4, color);
//	add_line(v4, v2, color);
//	add_line(v2, v7, color);
//	add_line(v7, v3, color);
//
//	add_line(v1, v3, color);
//	add_line(v8, v4, color);
//	add_line(v6, v2, color);
//	add_line(v5, v7, color);
//}
//
//void DebugDrawer::AddSphere(const vec3& center, float radius, const vec3& color)
//{
//	// prepare mesh
//	DebugMesh m;
//	m.mesh = ObjLoader::GetMesh("sphere");
//	m.color = color;
//	m.model_to_world.set(radius, 0.0f, 0.0f, center.x,
//		0.0f, radius, 0.0f, center.y,
//		0.0f, 0.0f, radius, center.z,
//		0.0f, 0.0f, 0.0f, 1.0f);
//
//	// store mesh for drawing
//	meshes_.push_back(m);
//}
//
//
//void DebugDrawer::AddSphere(const BoundingSphere& sphere, const vec3& color)
//{
//	AddSphere(sphere.center, sphere.radius, color);
//}
//
//void DebugDrawer::AddEllipse(const BoundingEllipsoid& ellipse, bool h, const vec3& color)
//{
//	// prepare mesh
//	DebugMesh m;
//	m.mesh = ObjLoader::GetMesh("sphere");
//	m.color = color;
//
//	// form transformation
//	mat4 rot = mat4::identity;
//	mat4 scale = mat4::identity;
//	mat4 trans = mat4::identity;
//
//	// set scale
//	scale.m00 = abs(ellipse.half_widths.x);
//	if (h)
//		scale.m00 *= -1.0f;
//	scale.m11 = abs(ellipse.half_widths.y);
//	scale.m22 = abs(ellipse.half_widths.z);
//
//	// set rot
//	for (int i = 0; i < 3; ++i)
//		for (int j = 0; j < 3; ++j)
//			rot.m[i][j] = ellipse.rotation.m[i][j];
//
//	// set translate
//	trans.m03 = ellipse.center.x;
//	trans.m13 = ellipse.center.y;
//	trans.m23 = ellipse.center.z;
//
//	m.model_to_world = trans * rot * scale;
//
//	// store mesh for drawing
//	meshes_.push_back(m);
//}
//
//void DebugDrawer::add_mesh(Mesh* m, const mat4& model_to_world, const vec3& color)
//{
//	DebugMesh d_mesh;
//	d_mesh.mesh = m;
//	d_mesh.color = color;
//	d_mesh.model_to_world = model_to_world;
//
//	meshes_.push_back(d_mesh);
//}
//
//void DebugDrawer::render_lines(Shader* pShader, const mat4& model)
//{
//	if (!vertex_list_.empty()) {
//
//		pShader->SetBool("uniformColor", false);
//		pShader->SetMat4("model", model);
//
//		// bind
//		glBindVertexArray(vao_);
//		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
//
//		// set data
//		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_list_.size(), &vertex_list_[0], GL_DYNAMIC_DRAW);
//
//		// draw lines
//		glDrawArrays(GL_LINES, 0, GLsizei(vertex_list_.size()));
//
//		// unbind
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindVertexArray(0);
//	}
//}
//
//void DebugDrawer::render_meshes(Shader* pShader)
//{
//	if (!meshes_.empty()) {
//
//		// save polygon mode
//		GLint polygon_mode[2];
//		glGetIntegerv(GL_POLYGON_MODE, polygon_mode);
//
//		// set wireframe
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//		pShader->SetBool("uniformColor", true);
//
//		for (DebugMesh& m : meshes_)
//		{
//			pShader->SetVec3("color", m.color);
//			pShader->SetMat4("model", m.model_to_world);
//
//			glBindVertexArray(m.mesh->vao_);
//			glDrawElements(GL_TRIANGLES, GLsizei(m.mesh->indices_.size()), GL_UNSIGNED_INT, nullptr);
//			glBindVertexArray(0);
//		}
//
//		// reset polygon mode
//		glPolygonMode(GL_FRONT_AND_BACK, polygon_mode[0]);
//	}
//}
//
//void DebugDrawer::clear()
//{
//	vertex_list_.clear();
//	meshes_.clear();
//}
