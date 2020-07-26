#include <light.hpp>
#include <graphic_system.hpp>
#include <shader.hpp>
#include <mat4.hpp>
#include <transform.hpp>
#include <camera.hpp>
#include <mesh.hpp>
#include <asset_manager.hpp>

jeBegin

jeDefineComponentBuilder(Light);

/*
*   Distance	Constant    Linear	Quadratic
*   =========================================
*   7	        1.0	        0.7	    1.8
*   13	        1.0	        0.35	0.44
*   20	        1.0	        0.22	0.20
*   32	        1.0	        0.14	0.07
*   50	        1.0	        0.09	0.032
*   65	        1.0	        0.07	0.017
*   100	        1.0	        0.045	0.0075
*   160	        1.0	        0.027	0.0028
*   200	        1.0	        0.022	0.0019
*   325	        1.0	        0.014	0.0007
*   600	        1.0	        0.007	0.0002
*   3250	    1.0	        0.0014	0.000007
*/

vec3 Light::kAmbientColor = vec3::zero, Light::fogColor = vec3::zero;

Light::Light(Object* owner)
	: Renderer(owner), ambient(vec3::one), diffuse(vec3::one), specular(vec3::one),
	innerAngle(24.5f), outerAngle(35.5f), fallOff(1.f), 
    constant(1.f), linear(/*0.7f*//*0.09f*/0.045f), quadratic(/*1.8f*//*0.032f*/0.0075f),
    ambientIntensity(0.1f), diffuseIntensity(0.5f), specularIntensity(1.f),
    activate(true), type(LightType::POINT)
{
    meshes_ = AssetManager::get_meshes("cube");
}

Light::~Light()
{
}

void Light::draw(float /*dt*/)
{
    Camera* camera = GraphicSystem::get_camera();
    Shader* shader = GraphicSystem::shader_[GraphicSystem::LIGHT];
    shader->use();

    shader->set_matrix("m4_translate", mat4::translate(transform_->position));
    shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
    shader->set_matrix("m4_rotate", transform_->orientation.to_mat4());
    shader->set_bool("boolean_bilboard", (status & IS_BILBOARD) == IS_BILBOARD);
    shader->set_vec3("v3_color", diffuse);

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

    for (const auto& m : meshes_)
        m->draw(shader);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void Light::add_to_system()
{
	set_parent_renderer();
	GraphicSystem::add_light(this);
}

void Light::remove_from_system()
{
	GraphicSystem::remove_light(this);
}

void Light::load(const rapidjson::Value& /*data*/)
{
}

jeEnd
