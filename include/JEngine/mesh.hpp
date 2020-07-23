#pragma once
#include <vertex.hpp>
#include <assets.hpp>

jeBegin

class Shader;
class Mesh {

    friend class Model;
    friend class DebugRenderer;
    friend class GraphicSystem;

public:

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    unsigned get_indices_count() const;

private:

    void draw(Shader* shader);
    
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;

    unsigned int vao_, vbo_, ebo_;

    void setup_mesh();
};

jeEnd