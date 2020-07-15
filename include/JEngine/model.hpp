#pragma once
#include <vector>
#include <string>
#include <renderer.hpp>

jeBegin

class Mesh;

class Model : public Renderer
{
	jeBaseFriends(Model);
	friend class GraphicSystem;

	using Meshes = std::vector<Mesh*>;

public:

	Model(Object* owner);
	virtual ~Model();

	void set_mesh(const std::string& name);
	const Meshes& get_meshes(void) const;

	void start_draw(const vec3& resScalar) override;
	void draw(float dt) override;
	void end_draw() override;

	void draw_normals();
	void draw_debug_info();

private:

	bool renderBoundary_, 
		renderFaceNormals_, 
		renderVertexNormals_;
	bool h_;
	Meshes meshes_;
	//AABB aabb_;
	//OBB obb_;
	//BoundingEllipsoid ellipse_;
	//BoundingSphere sphere_;

	// private methods
	// void GenerateBV(void);

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& data);

};

jeDeclareComponentBuilder(Model);

jeEnd