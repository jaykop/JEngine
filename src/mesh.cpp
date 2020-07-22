#include <glew.h>
#include <mesh.hpp>
#include <math_util.hpp>
#include <half_edge_mesh.hpp>

jeBegin

using namespace Math;

Mesh::BVType Mesh::bvType_ = Mesh::BVType::BV_NONE;

Mesh::Mesh()
	: rotation(0.f), setNormals(true), activate(true), absMax(0.f),
	min(vec3(1.f, 1.f, 1.f)), max(vec3(1.f, 1.f, 1.f)), centerOffset(vec3(0.f, 0.f, 0.f))
{}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, HalfEdgeMesh* hedge_mesh)
	: rotation(0.f), setNormals(true), activate(true), absMax(0.f),
	min(vec3(1.f, 1.f, 1.f)), max(vec3(1.f, 1.f, 1.f)), centerOffset(vec3(0.f, 0.f, 0.f)),
	hEdgeMesh(hedge_mesh)
{
	initialize(vertices, indices);
}

Mesh::~Mesh()
{
	delete hEdgeMesh;
	hEdgeMesh = nullptr;

	// Delete vao, vbos, ebo
	glDeleteVertexArrays(1, &vao_);
	glDeleteVertexArrays(1, &vnVao_);
	glDeleteVertexArrays(1, &fnVao_);
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vnVbo_);
	glDeleteBuffers(1, &fnVbo_);
}

void Mesh::set_texture(unsigned int texture_id) { texture_ = texture_id; }

unsigned Mesh::get_texture() const {return texture_; }

unsigned Mesh::get_point_count() const { return unsigned(points_.size()); }

unsigned Mesh::get_indice_count() const { return unsigned(indices_.size()); }

std::vector<vec3> Mesh::get_points(void) const { return vPoints_; }

vec3 Mesh::get_point(unsigned index) const { return points_.at(index); }

std::vector<unsigned> Mesh::get_indices(void) const { return indices_; }

unsigned Mesh::get_indice(unsigned index) const { return indices_.at(index); }

void Mesh::add_point(vec3 point) { points_.push_back(point); }

void Mesh::add_indice(unsigned indice) { indices_.push_back(indice); }

void Mesh::points_along_direction(const vec3& dir, const std::vector<vec3>& vertices, int& min_index, int& max_index)
{
	float min_proj = max_float;
	float max_proj = min_float;

	min_index = -1;
	max_index = -1;

	unsigned size = unsigned(vertices.size());
	for (unsigned i = 0; i < size; ++i)
	{
		const vec3& v = vertices[i];

		// project onto direction vector
		float proj = v.dot(dir);

		// check if point is minimum
		if (proj < min_proj)
		{
			min_proj = proj;;
			min_index = i;
		}

		// check if point is maximum
		if (proj > max_proj)
		{
			max_proj = proj;
			max_index = i;
		}
	}
}

void Mesh::initialize(const std::vector<Vertex>& vertices, 
	const std::vector<unsigned>& indices)
{
	vertices_ = vertices;
	indices_ = indices;

	// copy points
	for (const auto& v : vertices_)
		vPoints_.push_back(v.position);

	// generate vertex array
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// generate vertex buffer
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	// set vertex data
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::texCoords)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::color)));
	glEnableVertexAttribArray(3);

	// generate index buffer
	glGenBuffers(1, &ebo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

	// set index data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), &indices_[0], GL_STATIC_DRAW);

	// unbind buffer
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

jeEnd