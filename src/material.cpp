#include <material.hpp>
#include <physics_system.hpp>
#include <transform.hpp>
#include <object.hpp>

jeBegin

jeDefineComponentBuilder(Material);

Material::Material(Object* owner) : Component(owner) {}

Material::~Material() {}

void Material::add_to_system()
{
}

void Material::remove_from_system()
{
}

jeEnd