#include <model.hpp>
#include <transform.hpp>
#include <mesh.hpp>
#include <shader.hpp>
#include <asset_manager.hpp>
#include <graphic_system.hpp>
#include <gl_manager.hpp>
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

void Model::set_mesh(const std::string& name)
{
	// get new mesh
	Mesh* newMesh = AssetManager::get_mesh(name.c_str());

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

void Model::draw(float /*dt*/)
{
	Camera* camera = GraphicSystem::get_camera();

	//Shader* shader = GLManager::shader_[GLManager::NORMAL];
	//shader->use();

	//shader->set_matrix("m4_translate", mat4::translate(transform_->position));
	//shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
	//shader->set_matrix("m4_rotate", transform_->orientation.to_mat4());
	//shader->set_vec3("v3_cameraPosition", camera->position_);
	//shader->set_bool("boolean_bilboard", (status_ & IS_BILBOARD) == IS_BILBOARD);
	//// shader->set_vec4("v4_color", color);

	//mat4 viewport;

	//if (prjType_ == ProjectType::PERSPECTIVE) {

	//	shader->set_matrix("m4_projection", perspective);
	//	viewport = mat4::look_at(camera->position_, camera->target_, camera->up_);
	//}

	//else {
	//	float right_ = GLManager::get_width() * .5f;
	//	float left_ = -right_;
	//	float top_ = GLManager::get_height() * .5f;
	//	float bottom_ = -top_;

	//	mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, camera->near_, camera->far_);

	//	shader->set_matrix("m4_projection", orthogonal);

	//	viewport = mat4::scale(resScalar);
	//}

	//// Send camera info to shader
	//shader->set_matrix("m4_viewport", viewport);

	////bool hasParent = (pModel->status_ & Model::IS_INHERITED) == Model::IS_INHERITED;
	////glUniform1i(glGetUniformLocation(Shader::pCurrentShader_->programId_, "hasParent"), hasParent);
	////if (hasParent)
	////	ParentPipeline(pModel->pInherited_);

	////if (pModel->pMaterial_ && isLight_)
	////	LightingEffectPipeline(pModel->pMaterial_);

	//glEnable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	//glBlendFunc(sfactor_, dfactor_);

	for (auto mesh : meshes_)
	{
		glBindVertexArray(mesh->vao_);
		glDrawElements(drawMode_, mesh->get_indice_count(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);
}

void Model::draw_normals()
{
	// Rendering normals
	if (renderVertexNormals_
		|| renderFaceNormals_) {

		Shader* shader = GLManager::shader_[GLManager::NORMAL];
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