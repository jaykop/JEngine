#include <model.hpp>
#include <fstream>
#include <sstream>

#include <graphic_system.hpp>
#include <math_util.hpp>

#include <glew.h>

#include <map>
#include <mesh.hpp>
#include <iostream>

#include <shader.hpp>
#include <camera.hpp>
#include <transform.hpp>
#include <mat4.hpp>

jeBegin

jeDefineComponentBuilder(Model);

// constructor, expects a filepath to a 3D model.
Model::Model(Object* owner)
    : Renderer(owner), gammaCorrection(false), color(vec4::one)
{
}

Model::~Model()
{
}

void Model::add_to_system()
{
    set_parent_renderer();
    GraphicSystem::add_renderer(this);
}

void Model::remove_from_system()
{
    GraphicSystem::remove_renderer(this);
}

void Model::load(const rapidjson::Value& /*data*/) {

}

// draws the model, and thus all its meshes
void Model::draw(float /*dt*/)
{
    Camera* camera = GraphicSystem::get_camera();
    Shader* shader = GraphicSystem::shader_[GraphicSystem::MODEL];
    shader->use();

    shader->set_matrix("m4_translate", mat4::translate(transform_->position));
    shader->set_matrix("m4_scale", mat4::scale(transform_->scale));
    shader->set_matrix("m4_rotate", transform_->orientation.to_mat4());
    shader->set_vec3("v3_cameraPosition", camera->position);
    shader->set_bool("boolean_bilboard", (status & IS_BILBOARD) == IS_BILBOARD);
    shader->set_vec4("v4_color", color);

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

void Model::set_meshes(std::vector<Mesh*> meshes)
{
    meshes_ = meshes;
}

jeEnd