#pragma once
#include <string>
#include <renderer.hpp>

jeBegin

class Mesh;
class Object;
class DebugRenderer;

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

	void draw() override;
	void draw_normals();
	void draw_debug_info();
	// void on_gui(void) override;

	DebugRenderer* ddrawer_ = nullptr;

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