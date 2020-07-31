#pragma once
#include <vertex.hpp>
#include <assets.hpp>

jeBegin

class Shader;
class Mesh {

    friend class Light;
    friend class Model;
    friend class AssetManager;
    friend class DebugRenderer;
    friend class GraphicSystem;

public:

    Mesh(const std::vector<Vertex>& vertices, 
        const std::vector<unsigned int>& indices, 
        const std::vector<Texture>& textures);
    ~Mesh();

    unsigned get_indices_count() const;

private:

    void draw(Shader* shader, bool envr);
    
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;

    unsigned int vao_, vbo_, ebo_, defaultTexture_;

    vec3 minPosition_, maxPosition_;

    void setup_mesh();
};

jeEnd