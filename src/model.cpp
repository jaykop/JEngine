#include <model.hpp>
#include <transform.hpp>
#include <mesh.hpp>
#include <shader.hpp>
#include <asset_manager.hpp>
#include <graphic_system.hpp>
#include <camera.hpp>

jeBegin

jeDefineComponentBuilder(Model);

Model::Model(Object* owner)
	: Renderer(owner), renderBoundary_ (false),
	renderFaceNormals_ (false),	renderVertexNormals_(false),
	h_(false)
{}

Model::~Model()
{
	meshes_.clear();
}

void Model::add_to_system() {
	GraphicSystem::add_renderer(this);
}

void Model::remove_from_system() {
	GraphicSystem::remove_renderer(this);
}

void Model::load(const rapidjson::Value& /*data*/) {

}

void Model::add_mesh(Mesh* mesh)
{
	// get new mesh
	Mesh* newMesh = mesh;

	// add to the mesh
	meshes_.emplace_back(newMesh);

	//if (pMesh_->key_ == std::string("bunny"))
	//	h_ = true;
	//else
	//	h_ = false;
}

const Model::Meshes& Model::get_meshes(void) const
{
	return meshes_;
}

void Model::draw_debug_info()
{
	//pDDrawer_->render_lines(pShader, pTrans_->model_to_world());
	//pDDrawer_->render_meshes(pShader);
}

void Model::set_draw_mode(unsigned mode)
{
	drawMode_ = mode;
}

void Model::draw(float /*dt*/)
{
	Camera* camera = GraphicSystem::get_camera();
	// Shader* shader = GLManager::shader_[GLManager::SPRITE];
	Shader* shader = GraphicSystem::shader_[GraphicSystem::MODEL];
	shader->use();

	shader->set_matrix("m4_translate", mat4::translate(transform_->position));
	shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
	shader->set_matrix("m4_rotate", transform_->orientation.to_mat4());
	shader->set_vec3("v3_cameraPosition", camera->position);
	shader->set_bool("boolean_bilboard", (status & IS_BILBOARD) == IS_BILBOARD);
	shader->set_bool("boolean_flip", (status & IS_FLIPPED) == IS_FLIPPED);
	// shader->set_vec4("v4_color", color);

	switch (prjType)
	{
	case ProjectType::PERSPECTIVE:
	{

		mat4 perspective = mat4::perspective(
			camera->fovy_ + camera->zoom, camera->aspect_,
			camera->near_, camera->far_);

		shader->set_matrix("m4_projection", perspective);
		break;
	}

	case ProjectType::ORTHOGONAL:
	default:
	{
		float right_ = GraphicSystem::width_ * GraphicSystem::resScaler_.x;
		float left_ = -right_;
		float top_ = GraphicSystem::height_ * GraphicSystem::resScaler_.y;
		float bottom_ = -top_;

		mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, camera->near_, camera->far_);
		shader->set_matrix("m4_projection", orthogonal);
		break;
	}
	}

	bool fixed = (status & IS_FIXED) == IS_FIXED;
	shader->set_bool("boolean_fix", fixed);

	if (!fixed)
	{
		// Send camera info to shader
		// mat4 viewport = mat4::look_at(camera->position, camera->right_, camera->up_, camera->back_);
		mat4 viewport = mat4::look_at(camera->position, camera->position + camera->back_, camera->up_);
		shader->set_matrix("m4_viewport", viewport);
	}

	bool isHerited = parent_ != nullptr;
	shader->set_bool("boolean_herited", isHerited);
	if (isHerited)
	{
		Transform* pTransform = parent_->get_transform();
		shader->set_matrix("m4_parentTranslate", mat4::translate(pTransform->position));
		shader->set_matrix("m4_parentScale", mat4::scale(pTransform->scale));
		shader->set_matrix("m4_parentRotate", pTransform->orientation.to_mat4());
	}

	//if (pModel->pMaterial_ && isLight_)
	//	LightingEffectPipeline(pModel->pMaterial_);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(sfactor_, dfactor_);

	// save polygon mode
	//GLint polygon_mode[2];
	//glGetIntegerv(GL_POLYGON_MODE, polygon_mode);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (const auto& mesh : meshes_)
	{
		glBindVertexArray(mesh->vao_);
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh->get_point_count());
		glDrawElements(GL_TRIANGLE_STRIP, mesh->get_indice_count(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	//glPolygonMode(GL_FRONT_AND_BACK, polygon_mode[0]); 
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void Model::draw_normals()
{
	// Rendering normals
	if (renderVertexNormals_
		|| renderFaceNormals_) {

		// Shader* shader = GLManager::shader_[GLManager::SPRITE];
		Shader* shader = GraphicSystem::shader_[GraphicSystem::MODEL];
		shader->use();

		for (const auto& m : meshes_) {

			// Set uniform location 
			shader->set_matrix("model", transform_->model_to_world());
			shader->set_bool("uniformColor", false);

			if (renderVertexNormals_) {
				glBindVertexArray(m->vnVao_);
				glDrawArrays(GL_LINES, 0, GLsizei(m->vertexNormalsDraw.size()));
			}

			if (renderFaceNormals_) {
				glBindVertexArray(m->fnVao_);
				glDrawArrays(GL_LINES, 0, GLsizei(m->faceNormalsDraw.size()));
			}

			// Unbind the vao
			glBindVertexArray(0);
		}
	}
}

//
//void Renderer::GenerateBV(void)
//{
//	auto points = pMesh_->GetPoints();
//	pDDrawer_->clear();
//
//	switch (Mesh::bvType_)
//	{
//	case Mesh::BVType::BV_AABB:
//		aabb_ = AABB::Generate(points);
//		pDDrawer_->AddCube(aabb_, yellow);
//		break;
//	case Mesh::BVType::BV_SPHERE_CENTEROID:
//		sphere_ = BoundingSphere::GenerateCentroid(points);
//		pDDrawer_->AddSphere(sphere_.Transformed(*pTrans_), yellow);
//		break;
//	case Mesh::BVType::BV_SPHERE_RITTER:
//		sphere_ = BoundingSphere::GenerateRitter(points);
//		pDDrawer_->AddSphere(sphere_.Transformed(*pTrans_), yellow);
//		break;
//	case Mesh::BVType::BV_SPHERE_LARSSON:
//		sphere_ = BoundingSphere::GenerateLarsson(points);
//		pDDrawer_->AddSphere(sphere_.Transformed(*pTrans_), yellow);
//		break;
//	case Mesh::BVType::BV_SPHERE_PCA:
//		sphere_ = BoundingSphere::GeneratePCA(points);
//		pDDrawer_->AddSphere(sphere_.Transformed(*pTrans_), yellow);
//		break;
//	case Mesh::BVType::BV_ELLIPSOID_PCA:
//		ellipse_ = BoundingEllipsoid::Generate(points);
//		pDDrawer_->AddEllipse(ellipse_.Transformed(*pTrans_), h_, yellow);
//		break;
//	case Mesh::BVType::BV_OBB:
//		obb_ = OBB::Generate(points);
//		pDDrawer_->AddCube(obb_, yellow);
//		break;
//
//	case Mesh::BVType::BV_NONE:
//	default:
//		break;
//	}
//}

jeEnd