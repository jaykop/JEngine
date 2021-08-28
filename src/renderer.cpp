#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <renderer.hpp>
#include <graphic_system.hpp>
#include <object.hpp>
#include <asset_manager.hpp>

#include <colors.hpp>
#include <math_util.hpp>
#include <transform.hpp>
#include <input_handler.hpp>

#include <sprite.hpp>
#include <model.hpp>
#include <text.hpp>
#include <light.hpp>
#include <emitter.hpp>

#include <glew.h>

jeBegin

using namespace Math;

bool Renderer::renderObj = true;
Renderer::RenderType Renderer::renderType = Renderer::RenderType::NONE;

Renderer::Renderer(Object* owner)
	: Component(owner),
	status(0x000), drawMode_(GL_TRIANGLES), prjType(ProjectType::PERSPECTIVE),
	sfactor_(GL_SRC_ALPHA), dfactor_(GL_ONE_MINUS_SRC_ALPHA)
{
	// connect transform component
	transform_ = owner->get_component<Transform>();
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

bool Renderer::picked(vec3* hitPoint) const
{
	// todo: change function modifying mesh
	vec3 center = transform_->position;
	vec3 half = transform_->scale;

	float half_x = half.x * 0.5f;
	float half_y = half.y * 0.5f;

	// create 8 vertices
	vec3 v1 = center + vec3(-half_x, half_y, center.z);
	vec3 v2 = center + vec3(-half_x, -half_y, center.z);
	vec3 v3 = center + vec3(half_x, -half_y, center.z);
	vec3 v4 = center + vec3(half_x, half_y, center.z);

	bool in1 = InputHandler::ray_intersects_triangle(v1, v2, v3, hitPoint);
	bool in2 = InputHandler::ray_intersects_triangle(v3, v4, v1, hitPoint);

	return in1 || in2;
}

void Renderer::set_parent_renderer()
{
	Object* parentObject = get_owner()->get_parent();
	if (parentObject)
	{
		if (parentObject->has_component<Sprite>())
		{
			parent_ = parentObject->get_component<Sprite>();
			return;
		}
		else if (parentObject->has_component<Text>())
		{
			parent_ = parentObject->get_component<Text>();
			return;
		}
		else if (parentObject->has_component<Emitter>())
		{
			parent_ = parentObject->get_component<Emitter>();
			return;
		}
		else if (parentObject->has_component<Light>())
		{
			parent_ = parentObject->get_component<Light>();
			return;
		}
		else if (parentObject->has_component<Model>())
		{
			parent_ = parentObject->get_component<Model>();
			return;
		}
	}
}

jeEnd