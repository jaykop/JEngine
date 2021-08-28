#include <contact.hpp>
#include <physics_system.hpp>
#include <transform.hpp>
#include <object.hpp>

jeBegin

jeDefineComponentBuilder(Contact);

Contact::Contact(Object* owner) : Component(owner) {}

Contact::~Contact() {}

void Contact::add_to_system()
{
}

void Contact::remove_from_system()
{
}

jeEnd