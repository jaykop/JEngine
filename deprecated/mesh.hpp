#pragma once
#include <vector>
#include <map>
#include <string>
#include <vertex.hpp>

jeBegin

struct Texture;
class Transform;
class HalfEdgeMesh;

class Mesh
{
	friend class Scene;
	friend class Sprite;
	friend class Model;
	friend class AssetManager; 
	friend class DebugRenderer;

public:

	enum class BVType {
		NONE, AABB, OBB,
		SPHERE_CENTEROID, SPHERE_RITTER,
		SPHERE_LARSSON, SPHERE_PCA,
		ELLIPSOID_PCA
	};

	static void points_along_direction(const vec3& dir, const std::vector<vec3>& vertices,
		int& min_index, int& max_index);
	void initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices);

	Mesh();
	Mesh(const std::vector<Vertex>& vertices,
		const std::vector<unsigned>& indices,
		HalfEdgeMesh* hedge_mesh);
	~Mesh();

	void add_point(vec3 point);
	void add_indice(unsigned indice);

	std::vector<vec3> get_points(void) const;
	vec3 get_point(unsigned index) const;
	unsigned get_point_count() const;

	std::vector<unsigned> get_indices(void) const;
	unsigned get_indice(unsigned index) const;
	unsigned get_indice_count() const;

	void set_texture(unsigned int texture_id);
	unsigned get_texture() const;

private:

	static BVType bvType_;

	Transform* transform = nullptr;
	HalfEdgeMesh* hEdgeMesh = nullptr;

	float rotation;
	bool setNormals, activate;
	unsigned texture_;
	unsigned vao_, vbo_, ebo_, 	vnVao_, vnVbo_, fnVao_, fnVbo_;

	std::string key;
	std::vector<Vertex> vertices_, vertexNormalsDraw, faceNormalsDraw;
	std::vector<unsigned> indices_;
	std::vector<vec3> points_, faceNormals_, vertexNormals_, centers_, vPoints_;
};

jeEnd