#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <timer.hpp>
#include <vec3.hpp>

jeBegin

class Animation2D : public Component
{
    jeBaseFriends(Animation2D);
    friend class Renderer;
    friend class GraphicSystem;

public:

    bool is_activated() const;
    void activate(bool toggle);

    void fix_frame(int thFrame);
    void set_frame(int numOfFrame);
    int get_frame() const;

    void set_speed(float speed);
    float get_speed() const;

protected:

    void add_to_system() override {}
    void remove_from_system() override {}
    void load(const rapidjson::Value& /*data*/) override {}

private:

    Animation2D(Object* owner);
    virtual ~Animation2D();

    Timer timer_;
    int frames_;
    bool activated_;
    float realSpeed_, realFrame_, currentFrame_, animationSpeed_;
    vec3 scale_, translate_;

};

jeDeclareComponentBuilder(Animation2D);

jeEnd