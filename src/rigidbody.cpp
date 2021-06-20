#include <rigidbody.hpp>

jeBegin

jeDefineComponentBuilder(RigidBody);

RigidBody::RigidBody(Object* owner) : Component(owner) {}

RigidBody::~RigidBody() {}


jeEnd