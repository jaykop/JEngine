#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <renderer.hpp>
#include <graphic_system.hpp>
#include <obj_loader.hpp>
#include <gl_manager.hpp>
#include <object.hpp>

#include <shader.hpp>
#include <mesh.hpp>
#include <texture.hpp>
#include <camera.hpp>
#include <transform.hpp>
#include <animation_2d.hpp>

#include <math_util.hpp>

jeBegin

jeDefineComponentBuilder(Renderer);

using namespace Math;

const static int IS_FLIPPED = 0x100;
const static int IS_BILBOARD = 0x010;
const static int IS_INHERITED = 0x001;

bool Renderer::renderObj_ = true;
Renderer::RenderType Renderer::renderType_ = Renderer::RenderType::NONE;

void Renderer::add_to_system() {
	GraphicSystem::add_model(this);
}

void Renderer::remove_from_system() {
	GraphicSystem::remove_model(this);
}

void Renderer::load(const rapidjson::Value& /*data*/) {

}

Renderer::Renderer(Object* owner)
	: Component(owner), h_(false), is2d(true), renderBoundary_(false),
	renderFaceNormals_(false), renderVertexNormals_(false),
	status_(0x000), drawMode_(GL_TRIANGLES), prjType(ProjectType::PERSPECTIVE),
	dfactor(GL_ONE_MINUS_SRC_ALPHA), sfactor(GL_SRC_ALPHA), animation_(nullptr),
	transform_(nullptr)
{
	
	// connect transform component
	transform_ = owner->get_component<Transform>();
}

void Renderer::set_mesh(const std::string& name)
{
	// get new mesh
	Mesh* newMesh = ObjLoader::get_mesh(name.c_str());

	// add to the mesh
	meshes_.emplace_back(newMesh);

	//if (pMesh_->key_ == std::string("bunny"))
	//	h_ = true;
	//else
	//	h_ = false;
}

const Renderer::Meshes& Renderer::get_meshes(void) const
{
	return meshes_;
}

void Renderer::draw(Camera* camera, const mat4& perspective, const vec3& resScalar)
{
	//if (is_2d) {
	//	
	//	pShader->set_matrix("renderer", pTrans_->model_to_world());

	//	glBindVertexArray(Mesh::quadVAO);
	//	glBindTexture(GL_TEXTURE_2D, meshes_[0]->texture_->id);
	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	//	glBindVertexArray(0);
	//	glBindTexture(GL_TEXTURE_2D, 0);

	//	// continue;
	//}

	//else {
	//	for (const auto& m : meshes_) {

	//		// Set uniform location 
	//		pShader->set_matrix("renderer", pTrans_->model_to_world());

	//		glBindVertexArray(m->vao_);
	//		glBindTexture(GL_TEXTURE_2D, m->texture_->id);
	//		glDrawElements(GL_TRIANGLES, m->get_indice_count(), GL_UNSIGNED_INT, nullptr);
	//		glBindVertexArray(0);
	//		glBindTexture(GL_TEXTURE_2D, 0);
	//	}
	//}

	Shader* shader = GLManager::shader_[GLManager::Pipeline::NORMAL];
	shader->use();

	shader->set_matrix("m4_translate", mat4::translate(transform_->position));
	shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
	shader->set_matrix("m4_rotate", transform_->orientation.to_mat4());
	shader->set_vec3("v3_cameraPosition", camera->position_);
	shader->set_bool("boolean_bilboard", (status_ & IS_BILBOARD) == IS_BILBOARD);

	mat4 viewport;

	if (prjType == ProjectType::PERSPECTIVE) {

		shader->set_matrix("m4_projection", perspective);
		viewport = mat4::look_at(camera->position_, camera->target_, camera->up_);
	}

	else {
		float right_ = GLManager::get_width() * .5f;
		float left_ = -right_;
		float top_ = GLManager::get_height() * .5f;
		float bottom_ = -top_;

		mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, camera->near_, camera->far_);

		shader->set_matrix("m4_projection", orthogonal);

		viewport = mat4::scale(resScalar);
	}

	// Send camera info to shader
	shader->set_matrix("m4_viewport", viewport);

	run_animation();

	//bool hasParent = (pModel->status_ & Model::IS_INHERITED) == Model::IS_INHERITED;
	//glUniform1i(glGetUniformLocation(Shader::pCurrentShader_->programId_, "hasParent"), hasParent);
	//if (hasParent)
	//	ParentPipeline(pModel->pInherited_);

	//if (pModel->pMaterial_ && isLight_)
	//	LightingEffectPipeline(pModel->pMaterial_);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(sfactor, dfactor);

	if (is2d)
		draw_quad(/*meshes_[0]*/nullptr);

	else {
		// Update every mesh
		for (auto mesh : meshes_)
		{
			glBindVertexArray(mesh->vao_);
			glDrawElements(drawMode_, mesh->get_indice_count(), GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void Renderer::run_animation()
{
	for (auto m : meshes_) {

		if (animation_) {

			glBindTexture(GL_TEXTURE_2D, m->texture_->id);

			if (animation_->activated_) {

				float realSpeed = animation_->realSpeed_;

				if (realSpeed <= animation_->timer_.get_elapsed_time()) {

					float nextFrame = animation_->currentFrame_;
					float realFrame = animation_->realFrame_;

					nextFrame = (status_ & IS_FLIPPED) == IS_FLIPPED ? nextFrame - realFrame : nextFrame + realFrame;

					animation_->currentFrame_ = nextFrame >= 1.f ? 0.f : nextFrame;
					animation_->timer_.start();
				}

				animation_->scale_.set(animation_->realFrame_, 1.f, 0.f);
				animation_->translate_.set(animation_->currentFrame_, 0.f, 0.f);

			}

			else {
				animation_->scale_.set(1, 1, 0);
				animation_->translate_.set(0, 0, 0);
			}

			// Send color info to shader
			Shader* shader = GLManager::shader_[GLManager::Pipeline::NORMAL];
			shader->use();

			shader->set_vec4("v4_color", color);
			shader->set_bool("boolean_flip", (status_ & IS_FLIPPED) == IS_FLIPPED);
			shader->set_matrix("m4_aniScale", mat4::scale(animation_->scale_));
			shader->set_matrix("m4_aniTranslate", mat4::translate(animation_->translate_));
		}
	}
}

void Renderer::draw_lighting_effect(Light* /*pLight*/)
{
	//pShader->set_int("renderType", renderType_);

	///*Calculate the light max and set the radius for light volume optimization*/
	//// Calculate the light max
	//float aLightMax = std::fmaxf(
	//	std::fmaxf(pLight->ambientColor_.x, pLight->ambientColor_.y),
	//	pLight->ambientColor_.z);
	//float dLightMax = std::fmaxf(
	//	std::fmaxf(pLight->diffuseColor_.x, pLight->diffuseColor_.y),
	//	pLight->diffuseColor_.z);
	//float sLightMax = std::fmaxf(
	//	std::fmaxf(pLight->specularColor_.x, pLight->specularColor_.y),
	//	pLight->specularColor_.z);
	//float lightMax = std::fmaxf(
	//	std::fmaxf(aLightMax, dLightMax), sLightMax);

	//// Get radius
	//pLight->radius_ = (-Light::linear_ + std::sqrtf(Light::linear_ * Light::linear_
	//	- 4 * Light::quadratic_ * (Light::constant_ - (256.f / 5.f) * lightMax))) / 2 * Light::quadratic_;

	//// Update shader uniform info
	//pShader->set_int(pLight->modeStr_, int(pLight->mode_));
	//pShader->set_vec3(pLight->positionStr_, pLight->pTransform_->position);
	//pShader->set_vec3(pLight->directionStr_, pLight->direction_);
	//pShader->set_vec3(pLight->aColorStr_, pLight->ambientColor_);
	//pShader->set_vec3(pLight->sColorStr_, pLight->specularColor_);
	//pShader->set_vec3(pLight->dColorStr_, pLight->diffuseColor_);
	//pShader->set_float(pLight->fallOffStr_, pLight->fallOff_);
	//pShader->set_float(pLight->radiusStr_, pLight->radius_);
	//pShader->set_float(pLight->innerAngleStr_, pLight->innerAngle_ * deg_to_rad);
	//pShader->set_float(pLight->outerAngleStr_, pLight->outerAngle_ * deg_to_rad);

}

void Renderer::draw_normals()
{
	// Rendering normals
	if (renderVertexNormals_
		|| renderFaceNormals_) {

		Shader * shader = GLManager::shader_[GLManager::Pipeline::NORMAL];
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

void Renderer::draw_quad(Mesh* m)
{
	glBindVertexArray(Mesh::quadVAO);
	// glBindTexture(GL_TEXTURE_2D, m->texture_->id);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::draw_debug_info()
{
	//pDDrawer_->render_lines(pShader, pTrans_->model_to_world());
	//pDDrawer_->render_meshes(pShader);
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