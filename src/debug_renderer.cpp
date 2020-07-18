#include <debug_renderer.hpp>
#include <mesh.hpp>
#include <shader.hpp>
#include <transform.hpp>
#include <gl_manager.hpp>
#include <camera.hpp>
#include <renderer.hpp>

#include <colors.hpp>
#include <graphic_system.hpp>
#include <mat4.hpp>

jeBegin

jeDefineComponentBuilder(DebugRenderer);

DebugRenderer::DebugRenderer(Object* owner)
	: Renderer(owner)
{
	add_quad(transform_->position, transform_->scale, Color::white);
}

DebugRenderer::~DebugRenderer(void)
{
}

void DebugRenderer::add_to_system() {
	GraphicSystem::add_renderer(this);
}

void DebugRenderer::remove_from_system() {
	GraphicSystem::remove_renderer(this);
}

void DebugRenderer::load(const rapidjson::Value& /*data*/) {

}

void DebugRenderer::draw(float /*dt*/)
{
	Camera* camera = GraphicSystem::get_camera();
	Shader* shader = GLManager::shader_[GLManager::DEBUG];
	shader->use();

	shader->set_matrix("m4_translate", mat4::translate(transform_->position));
	shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
	shader->set_matrix("m4_rotate", transform_->orientation.to_mat4());

	mat4 viewport;

	if (prjType == ProjectType::PERSPECTIVE) {

		viewport = mat4::look_at(camera->position, camera->target, camera->up_);

		mat4 perspective = mat4::perspective(
			camera->fovy, camera->aspect_,
			camera->near_, camera->far_);

		shader->set_matrix("m4_projection", perspective);
	}

	else {

		viewport = mat4::scale(GLManager::resScaler_);

		float right_ = GLManager::get_width() * .5f;
		float left_ = -right_;
		float top_ = GLManager::get_height() * .5f;
		float bottom_ = -top_;

		mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, camera->near_, camera->far_);
		shader->set_matrix("m4_projection", orthogonal);
	}

	// Send camera info to shader
	shader->set_matrix("m4_viewport", viewport);

	if (!vertices_.empty()) {

		glBindVertexArray(GLManager::drVao_);
		glBindBuffer(GL_ARRAY_BUFFER, GLManager::drVbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), &vertices_[0], GL_DYNAMIC_DRAW);
		glDrawArrays(GL_LINES, 0, GLsizei(vertices_.size()));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	if (!meshes_.empty()) {

		// save polygon mode
		GLint polygon_mode[2];
		glGetIntegerv(GL_POLYGON_MODE, polygon_mode);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//pShader->SetBool("uniformColor", true);

		for (auto& m : meshes_)
		{
			//pShader->SetVec3("color", m.color);
			//pShader->SetMat4("model", m.model_to_world);

			glBindVertexArray(m.mesh->vao_);
			glDrawElements(GL_TRIANGLES, GLsizei(m.mesh->indices_.size()), GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}

		glPolygonMode(GL_FRONT_AND_BACK, polygon_mode[0]);
	}
}

void DebugRenderer::add_line(const vec3& start, const vec3& end, const vec3& color)
{
	// add line vertices
	Vertex start_v, end_v;
	start_v.position = start;
	start_v.color = vec4(color, 1.f);
	end_v.position = end;
	end_v.color = vec4(color, 1.f);

	vertices_.emplace_back(start_v);
	vertices_.emplace_back(end_v);
}

void DebugRenderer::add_quad(const vec3& pos, const vec3& size, const vec3& color)
{
	float half_x = size.x * 0.5f;
	float half_y = size.y * 0.5f;

		// create 8 vertices
	vec3 v1 = pos + vec3(-half_x, half_y, size.z);
	vec3 v2 = pos + vec3(-half_x, -half_y, size.z);
	vec3 v3 = pos + vec3(half_x, -half_y, size.z);
	vec3 v4 = pos + vec3(half_x, half_y, size.z);

	// add edges
	add_line(v1, v2, color);
	add_line(v2, v3, color);
	add_line(v3, v4, color);
	add_line(v4, v1, color);

	/*glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), &vertices_[0], GL_DYNAMIC_DRAW);
	glBindVertexArray(0);*/
}

void DebugRenderer::clear()
{
	vertices_.clear();
	meshes_.clear();
}

jeEnd

//void DebugRenderer::AddCube(const vec3& pos, const vec3& size, const vec3& color)
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
//void DebugRenderer::AddCube(const AABB& bounding_box, const vec3& color)
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
//void DebugRenderer::AddCube(const OBB& bounding_box, const vec3& color)
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
//void DebugRenderer::AddSphere(const vec3& center, float radius, const vec3& color)
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
//void DebugRenderer::AddSphere(const BoundingSphere& sphere, const vec3& color)
//{
//	AddSphere(sphere.center, sphere.radius, color);
//}
//
//void DebugRenderer::AddEllipse(const BoundingEllipsoid& ellipse, bool h, const vec3& color)
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
//void DebugRenderer::add_mesh(Mesh* m, const mat4& model_to_world, const vec3& color)
//{
//	DebugMesh d_mesh;
//	d_mesh.mesh = m;
//	d_mesh.color = color;
//	d_mesh.model_to_world = model_to_world;
//
//	meshes_.push_back(d_mesh);
//}
//
//void DebugRenderer::render_lines(Shader* pShader, const mat4& model)
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
//void DebugRenderer::render_meshes(Shader* pShader)
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
//void DebugRenderer::clear()
//{
//	vertex_list_.clear();
//	meshes_.clear();
//}


//bool DebugRenderer::inside(const vec3& point)
//{
//	vec3 center = transform_->position;
//	vec3 half = transform_->scale;
//
//	float half_x = half.x * 0.5f;
//	float half_y = half.y * 0.5f;
//
//	// create 8 vertices
//	vec3 v1 = center + vec3(-half_x, half_y, center.z);
//	vec3 v2 = center + vec3(-half_x, -half_y, center.z);
//	vec3 v3 = center + vec3(half_x, -half_y, center.z);
//	vec3 v4 = center + vec3(half_x, half_y, center.z);
//
//	bool in1 = point.inside(v1, v2, v3);
//	bool in2 = point.inside(v3, v4, v1);
//
//	return in1 || in2;
//}