#pragma once
#include <assets.hpp>
#include <renderer.hpp>
#include <vec4.hpp>

jeBegin

class Mesh;
class Model : public Renderer
{
public:

    bool gammaCorrection;

    Model(Object* owner);
    virtual ~Model();

    void set_meshes(std::vector<Mesh*> meshes);

    vec4 color;

protected:

    void add_to_system();
    void remove_from_system();
    void load(const rapidjson::Value& /*data*/);

private:

    void draw(float dt) override;

    std::vector<Mesh*> meshes_;
};

jeDeclareComponentBuilder(Model);

jeEnd