#pragma once
#include <macros.hpp>
#include <vec3.hpp>
#include <map>
#include <vector>
#include <string>

jeBegin

class Mesh;
class HalfEdgeMesh;

class ObjLoader
{
public:

	using MeshList = std::map<std::string, Mesh*>;

	static bool load(const char* path);
	static Mesh* get_mesh(const char* name);
	static std::string parse_name(const char* name);
	static void clear();

	static MeshList meshList;
	static std::string key;
	static vec3 maxPoint, minPoint;

private:

	static void update_max_min(const vec3& v);
	static void convert_mesh(Mesh** mesh);

	static void parse_vertex(const std::string& data, Mesh** pMesh);
	static void read_vertex(const std::string& file_data, unsigned pos, std::vector<vec3>& vertices);
	static void read_face(const std::string& file_data, unsigned pos, std::vector<unsigned>& indice, unsigned vertice_size);
	static unsigned read_index(const char* data, unsigned vertice_size);
	static unsigned get_next_elements(const std::string& file_data, unsigned pos);
	static void calculate_normals(Mesh** pMesh);

	static vec3 get_converted_position(const vec3& position, const vec3& centerOffset, float absMax);

};

jeEnd