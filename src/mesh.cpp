#include <mesh.hpp>
#include <shader.hpp>
#include <graphic_system.hpp>
#include <asset_manager.hpp>

#include <vec3.hpp>
#include <vec2.hpp>
#include <string>

jeBegin

// constructor
Mesh::Mesh(const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
    : vao_(0), vbo_(0), ebo_(0), defaultTexture_(0)
{
    vertices_ = vertices;
    indices_ = indices;
    textures_ = textures;

    // set default texture
    if (textures_.empty())
        defaultTexture_ = AssetManager::get_texture("rect");

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setup_mesh();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
}

unsigned Mesh::get_indices_count() const
{
    return static_cast<unsigned>(indices_.size());
}

// render the mesh
void Mesh::draw(Shader* shader)
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    if (defaultTexture_)
    {
        glActiveTexture(GL_TEXTURE0);
        shader->set_uint("gDiffuse", 0);
        glBindTexture(GL_TEXTURE_2D, defaultTexture_);
    }

    for (unsigned int i = 0; i < textures_.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures_[i].type;
        if (name == "gDiffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "gSpecular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if (name == "gAmbient")
            number = std::to_string(heightNr++); // transfer unsigned int to stream
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        shader->set_uint((name).c_str(), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }
    
    // draw mesh
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setup_mesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    // vertex color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    //// vertex tangent
    //glEnableVertexAttribArray(3);
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    //// vertex bitangent
    //glEnableVertexAttribArray(4);
    //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
}

jeEnd