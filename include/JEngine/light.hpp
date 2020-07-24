#pragma once
#include <vector>
#include <vec4.hpp>
#include <renderer.hpp>

jeBegin

class Mesh;
class Light : public Renderer
{

    jeBaseFriends(Renderer);
    friend class GraphicSystem;

public:

    enum class LightType { NORMAL, DIRECTIONAL, SPOT, POINT};

    Light(Object* owner);
    virtual ~Light();

    static float constant, linear, quadratic;
    static vec3 kAmbientColor, fogColor;

    vec3 direction, ambient, diffuse, specular;
    float innerAngle, outerAngle, fallOff;
    bool activate;
    LightType type;

protected:

    virtual void add_to_system();
    virtual void remove_from_system();
    virtual void load(const rapidjson::Value& data);

    void draw(float dt) override;

private:

    std::vector<Mesh*> meshes_;

    Light() = delete;
    Light(const Light& /*copy*/) = delete;

};

jeDeclareComponentBuilder(Light);

jeEnd