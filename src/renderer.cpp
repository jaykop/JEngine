#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <renderer.hpp>
#include <graphic_system.hpp>
#include <gl_manager.hpp>
#include <object.hpp>
#include <asset_manager.hpp>

#include <shader.hpp>
#include <mesh.hpp>
#include <texture.hpp>
#include <camera.hpp>
#include <transform.hpp>

#include <math_util.hpp>

jeBegin

using namespace Math;

bool Renderer::renderObj_ = true;
Renderer::RenderType Renderer::renderType_ = Renderer::RenderType::NONE;


Renderer::Renderer(Object* owner)
	: Component(owner),
	status_(0x000), drawMode_(GL_TRIANGLES), prjType(ProjectType::PERSPECTIVE),
	dfactor(GL_ONE_MINUS_SRC_ALPHA), sfactor(GL_SRC_ALPHA),	transform_(nullptr)
{
	
	// connect transform component
	transform_ = owner->get_component<Transform>();
}

void Renderer::start_draw(Camera* camera,
	const mat4& perspective, const vec3& resScalar)
{
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


	//bool hasParent = (pModel->status_ & Model::IS_INHERITED) == Model::IS_INHERITED;
	//glUniform1i(glGetUniformLocation(Shader::pCurrentShader_->programId_, "hasParent"), hasParent);
	//if (hasParent)
	//	ParentPipeline(pModel->pInherited_);

	//if (pModel->pMaterial_ && isLight_)
	//	LightingEffectPipeline(pModel->pMaterial_);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(sfactor, dfactor);
}

void Renderer::end_draw()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
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

jeEnd