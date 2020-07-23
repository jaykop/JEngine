#include <fstream>
#include <sstream>
#include <mesh.hpp>
#include <half_edge_mesh.hpp>
#include <math_util.hpp>
#include <colors.hpp>
#include <asset_manager.hpp>
#include <scene_manager.hpp>
#include <scene.hpp>
#include <glew.h>

using namespace Math;

jeBegin

int MAX_POINTS = -1;
static const float normScale = 0.05f;
static const unsigned max_unsinged = (std::numeric_limits<unsigned>::max)();

bool AssetManager::load_obj(const char* path, const char* meshKey, MeshMap* mMap)
{
	if (mMap->find(meshKey) == mMap->end()) {

		std::ifstream obj(path, std::ios::in);
		std::stringstream buffer;
		buffer << obj.rdbuf();

		// Check if obj file is valid 
		if (!obj)
		{
			jeDebugPrint("AssetManager - Failed to load the obj file: %s", path);
			return false;
		}
		
		Mesh* newMesh = new Mesh();
		newMesh->setNormals = true;
		newMesh->key = meshKey;

		parse(buffer.str(), &newMesh);
		initialize_mesh_buffer(newMesh);

		mMap->insert({ meshKey, newMesh });
	}

	return true;
}

void AssetManager::initialize_mesh_buffer(Mesh* mesh)
{
	// Check either if all the objects are initialized
	// and if not, generate them
	if (!mesh->vao_) {
		glGenVertexArrays(1, &mesh->vao_);
		glBindVertexArray(mesh->vao_);
	}
	if (!mesh->vnVao_) {
		glGenVertexArrays(1, &mesh->vnVao_);
		glBindVertexArray(mesh->vnVao_);
	}
	if (!mesh->fnVao_) {
		glGenVertexArrays(1, &mesh->fnVao_);
		glBindVertexArray(mesh->fnVao_);
	}
	if (!mesh->vbo_) {
		glGenBuffers(1, &mesh->vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_);
	}
	if (!mesh->ebo_) {
		glGenBuffers(1, &mesh->ebo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo_);
	}
	if (!mesh->vnVbo_) {
		glGenBuffers(1, &mesh->vnVbo_);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vnVbo_);
	}
	if (!mesh->fnVbo_) {
		glGenBuffers(1, &mesh->fnVbo_);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->fnVbo_);
	}

	int size = int(mesh->vertices_.size());
	if (MAX_POINTS < size)
		MAX_POINTS = size;

	// Decribe the format of vertex and indice
	glBindVertexArray(mesh->vao_);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->vertices_.size(), &mesh->vertices_[0], GL_STATIC_DRAW);
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * mesh->indices_.size(), &mesh->indices_[0], GL_STATIC_DRAW);

	if (mesh->setNormals) {

		// Bind vertex normal buffer object
		glBindVertexArray(mesh->vnVao_);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vnVbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->vertexNormalsDraw.size(), &mesh->vertexNormalsDraw[0], GL_STATIC_DRAW);
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

		// Bind face normal buffer object
		glBindVertexArray(mesh->fnVao_);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->fnVbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->faceNormalsDraw.size(), &mesh->faceNormalsDraw[0], GL_STATIC_DRAW);
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
	}

	glBindVertexArray(0);
}

const std::vector<Mesh*>& AssetManager::get_model(const char* key)
{
	auto found = meshMap_.find(key);
	if (found != meshMap_.end())
		return found->second;

	found = SceneManager::currentScene_->meshes_.find(key);
	if (found != SceneManager::currentScene_->meshes_.end())
		return found->second;

	jeDebugPrint("!AssetManager - Cannot find such name of mesh resource: %s.\n", key);
	return nullptr;
}

void AssetManager::clear_meshes()
{
	for (auto& m : meshMap_) {
		delete m.second;
		m.second = nullptr;
	}
}

void AssetManager::parse(const std::string& data, Mesh** mesh)
{
	vec3 minPoint(max_float, max_float, max_float),
		maxPoint(min_float, min_float, min_float),
		centerOffset;
	float absMax = 0.f;

	// skip any leading white space
	unsigned it = unsigned(data.find_first_not_of("\n\r\0 "));
	unsigned size = unsigned((*mesh)->points_.size());

	while (it != max_unsinged)
	{
		// extract vertex data
		if (data[it] == 'v')
			read_vertex(data, it + 1, (*mesh)->points_, maxPoint, minPoint);

		// extract face data
		if (data[it] == 'f')
			read_face(data, it + 1, (*mesh)->indices_, size);

		// skip to next line
		it = unsigned(data.find_first_of("\n\r\0 ", it));
		it = unsigned(data.find_first_not_of("\n\r\0 ", it));
	}

	// normalize the scale and position
	vec3 sum = { maxPoint.x - minPoint.x,
		maxPoint.y - minPoint.y,
		maxPoint.z - minPoint.z };

	sum *= .5f;

	if (absMax < sum.x)
		absMax = sum.x;
	if (absMax < sum.y)
		absMax = sum.y;
	if (absMax < sum.z)
		absMax = sum.z;

	maxPoint /= absMax;
	minPoint /= absMax;
	centerOffset = ((maxPoint + minPoint) * .5f);
	minPoint -= centerOffset;
	maxPoint -= centerOffset;

	size = unsigned((*mesh)->points_.size());

	// Set vertex container
	for (unsigned i = 0; i < size; i++) {

		// convert normal
		vec3 convertedPos = (*mesh)->points_[i] / absMax - centerOffset;

		// Normal vertexes
		(*mesh)->vertices_.emplace_back(Vertex{ convertedPos, vec3::zero , vec2::zero, vec4::one });
		(*mesh)->vPoints_.emplace_back(convertedPos);
	}

	// generate an half edge mesh
	(*mesh)->hEdgeMesh = new HalfEdgeMesh((*mesh)->vertices_, (*mesh)->indices_);

	// calculate normals
	std::vector<Vertex>& vertices = (*mesh)->vertices_;
	std::vector<Vertex>& fNormals = (*mesh)->faceNormalsDraw;
	std::vector<Vertex>& vNormals = (*mesh)->vertexNormalsDraw;
	HalfEdgeMesh* h_mesh = (*mesh)->hEdgeMesh;

	const unsigned num_vertices = unsigned(vertices.size());
	for (unsigned i = 0; i < num_vertices; ++i)
	{
		Vertex& vert = vertices[i];

		// find surrounding faces
		auto neighbors = h_mesh->get_neighbor_faces(i);

		for (const Face* f : neighbors)
		{
			// get vertex positions of face
			vec3 p1 = f->edge->vertex->pos;
			vec3 p2 = f->edge->next->vertex->pos;
			vec3 p3 = f->edge->Prev()->vertex->pos;

			// calculate face normal
			vec3 face_normal = vec3::cross(p2 - p1, p3 - p2);

			float len = face_normal.length();
			if (len > 0.0f)
			{
				// add to vertex normal
				vert.normal += face_normal.normalized();
			}
		}

		// re-normalize
		float len = vert.normal.length();
		if (len > 0.0f) {
			vert.normal /= len;

			vNormals.emplace_back(Vertex{ vertices[i].position, vec3::zero, vec2::zero });
			vNormals.emplace_back(Vertex{ vertices[i].position + vert.normal * normScale,
				vec3::zero, vec2::zero });
		}
	}

	auto faces = h_mesh->get_faces();

	for (auto& f : faces)
	{
		// get vertex positions of face
		vec3 p1 = f.edge->vertex->pos;
		vec3 p2 = f.edge->next->vertex->pos;
		vec3 p3 = f.edge->Prev()->vertex->pos;

		// calculate face normal
		vec3 face_normal = vec3::cross(p2 - p1, p3 - p2);
		vec3 center((p1 + p2 + p3) / 3.f);

		vec3 c_center = center / absMax - centerOffset; 
		vec3 c_fnorm = face_normal / absMax - centerOffset;  

		fNormals.emplace_back(Vertex{ center, vec3::zero, vec2::zero });
		fNormals.emplace_back(Vertex{ center + face_normal.normalized() * normScale, 
			vec3::zero, vec2::zero });
	}
}

void AssetManager::read_vertex(const std::string& file_data, unsigned pos,
	std::vector<vec3>& points, vec3& maxPoint, vec3& minPoint)
{
	vec3 p;
	const char* c_data = file_data.c_str();

	// Read vertex positions
	pos = get_next_elements(file_data, pos);
	p.x = static_cast<float>(atof(c_data + pos));

	pos = get_next_elements(file_data, pos);
	p.y = static_cast<float>(atof(c_data + pos));

	pos = get_next_elements(file_data, pos);
	p.z = static_cast<float>(atof(c_data + pos));

	// Check absolute max value from vertex
	if (p.x < minPoint.x)
		minPoint.x = p.x;
	else
		maxPoint.x = p.x;

	if (p.y < minPoint.y)
		minPoint.y = p.y;
	else
		maxPoint.y = p.y;

	if (p.z < minPoint.z)
		minPoint.z = p.z;
	else
		maxPoint.z = p.z;

	points.emplace_back(p);
}

void AssetManager::read_face(const std::string& file_data, unsigned pos,
	std::vector<unsigned>& indice, unsigned vertice_size)
{
	const char* c_data = file_data.c_str();

	// read face indices
	pos = get_next_elements(file_data, pos);
	indice.emplace_back(read_index(c_data + pos, vertice_size));

	pos = get_next_elements(file_data, pos);
	indice.emplace_back(read_index(c_data + pos, vertice_size));

	pos = get_next_elements(file_data, pos);
	indice.emplace_back(read_index(c_data + pos, vertice_size));
}

unsigned AssetManager::read_index(const char* data, unsigned vertice_size)
{
	int index = atoi(data);

	// wrap index if needed
	if (index < 0)
		return unsigned(index + vertice_size);

	return unsigned(index - 1);
}

unsigned AssetManager::get_next_elements(const std::string& file_data, unsigned pos)
{
	// skip past current element
	pos = unsigned(file_data.find_first_of(" ", pos));

	// skip past white space
	return unsigned(file_data.find_first_not_of(" ", pos));
}

jeEnd