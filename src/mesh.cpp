#include <glew.h>
#include <mesh.hpp>
#include <math_util.hpp>

jeBegin

using namespace Math;

unsigned Mesh::quadVAO = 0, Mesh::quadVBO = 0, Mesh::quadEBO = 0;
int MAX_POINTS = -1;

float Mesh::quadVertices_[] = {

	// positions        // normal	   // texture	
	-1.0f,  1.0f, 0.0f, 0.f, 0.f, 1.f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.f, 0.f, 1.f, 0.0f, 0.0f, 
	 1.0f, -1.0f, 0.0f, 0.f, 0.f, 1.f, 1.0f, 0.0f, 
	 1.0f,  1.0f, 0.0f, 0.f, 0.f, 1.f, 1.0f, 1.0f, 
};

unsigned Mesh::quadIndices_[] = {

	0, 1, 3,
	1, 2, 3
};

void Mesh::initialize_quad()
{
	// setup plane VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadEBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices_), &quadVertices_[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::texCoords)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, &quadIndices_[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Mesh::remove_quad()
{
	// Return the quad gpu memories
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &quadEBO);
}

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
	// Delete vao, vbos, ebo
	glDeleteVertexArrays(1, &vao_);
	glDeleteVertexArrays(1, &vnVao_);
	glDeleteVertexArrays(1, &fnVao_);
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vnVbo_);
	glDeleteBuffers(1, &fnVbo_);
}

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

void Mesh::initialize(const Vertices& vertices, const Indices& indices)
{
	vertex = vertices;
	indices_ = indices;

	// copy points
	for (auto v : vertex)
		vPoints_.push_back(v.position);

	// generate vertex array
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// generate vertex buffer
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	// set vertex data
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Vertex), &vertex[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::texCoords)));
	glEnableVertexAttribArray(2);

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
void Mesh::describe_mesh_attribs(Mesh* pMesh)
{
	// Check either if all the objects are initialized
	// and if not, generate them
	if (!pMesh->vao_) {
		glGenVertexArrays(1, &pMesh->vao_);
		glBindVertexArray(pMesh->vao_);
	}
	if (!pMesh->vnVao_) {
		glGenVertexArrays(1, &pMesh->vnVao_);
		glBindVertexArray(pMesh->vnVao_);
	}
	if (!pMesh->fnVao_) {
		glGenVertexArrays(1, &pMesh->fnVao_);
		glBindVertexArray(pMesh->fnVao_);
	}
	if (!pMesh->vbo_) {
		glGenBuffers(1, &pMesh->vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, pMesh->vbo_);
	}
	if (!pMesh->ebo_) {
		glGenBuffers(1, &pMesh->ebo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->ebo_);
	}
	if (!pMesh->vnVbo_) {
		glGenBuffers(1, &pMesh->vnVbo_);
		glBindBuffer(GL_ARRAY_BUFFER, pMesh->vnVbo_);
	}
	if (!pMesh->fnVbo_) {
		glGenBuffers(1, &pMesh->fnVbo_);
		glBindBuffer(GL_ARRAY_BUFFER, pMesh->fnVbo_);
	}

	int size = int(pMesh->vertex.size());
	if (MAX_POINTS < size)
		MAX_POINTS = size;

	// Decribe the format of vertex and indice
	glBindVertexArray(pMesh->vao_);
	glBindBuffer(GL_ARRAY_BUFFER, pMesh->vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * pMesh->vertex.size(), &pMesh->vertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::texCoords)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * pMesh->indices_.size(), &pMesh->indices_[0], GL_STATIC_DRAW);

	if (pMesh->setNormals) {

		// Bind vertex normal buffer object
		glBindVertexArray(pMesh->vnVao_);
		glBindBuffer(GL_ARRAY_BUFFER, pMesh->vnVbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * pMesh->vertexNormalsDraw.size(), &pMesh->vertexNormalsDraw[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, Vertex::position)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, Vertex::normal)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, Vertex::texCoords)));
		glEnableVertexAttribArray(2);

		// Bind face normal buffer object
		glBindVertexArray(pMesh->fnVao_);
		glBindBuffer(GL_ARRAY_BUFFER, pMesh->fnVbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * pMesh->faceNormalsDraw.size(), &pMesh->faceNormalsDraw[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, Vertex::position)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, Vertex::normal)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, Vertex::texCoords)));
		glEnableVertexAttribArray(2);
	}

	glBindVertexArray(0);
}

jeEnd