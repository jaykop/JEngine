#pragma once

#include <vec3.hpp>
#include <vec2.hpp>
#include <vertex.hpp>
#include <assets.hpp>

#include <string>
#include <vector>

jeBegin

class assimpMesh {
public:
    // mesh Data
    std::vector<Vertex>   vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    unsigned int VAO;

    // constructor
    assimpMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    // render the mesh
    void Draw();

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh();
};

jeEnd