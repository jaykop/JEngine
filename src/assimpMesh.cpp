#include <assimpModel.hpp>
#include <shader.hpp>
#include <shader.hpp>
#include <camera.hpp>
#include <transform.hpp>
#include <mat4.hpp>
#include <graphic_system.hpp>

jeBegin

// constructor
assimpMesh::assimpMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

// render the mesh
void assimpMesh::Draw()
{
    Camera* camera = GraphicSystem::get_camera();
    Shader* shader = GraphicSystem::shader_[GraphicSystem::MODEL];
    shader->use();

    shader->set_matrix("m4_translate", mat4::translate(vec3::zero));
    shader->set_matrix("m4_scale", mat4::scale(vec3::one * 10));
    shader->set_matrix("m4_rotate", mat4::identity);
   // shader->set_vec3("v3_cameraPosition", camera->position);
    //shader->set_bool("boolean_bilboard", (status & IS_BILBOARD) == IS_BILBOARD);
    //shader->set_bool("boolean_flip", (status & IS_FLIPPED) == IS_FLIPPED);
   // shader->set_vec4("v4_color", color);

    mat4 perspective = mat4::perspective(
        camera->fovy_ + camera->zoom, camera->aspect_,
        camera->near_, camera->far_);

    shader->set_matrix("m4_projection", perspective);

    /*switch (prjType)
    {
    case ProjectType::PERSPECTIVE:
    {

        mat4 perspective = mat4::perspective(
            camera->fovy_ + camera->zoom, camera->aspect_,
            camera->near_, camera->far_);

        shader->set_matrix("m4_projection", perspective);
        break;
    }

    case ProjectType::ORTHOGONAL:
    default:
    {
        float right_ = GLManager::get_width() * GLManager::resScaler_.x;
        float left_ = -right_;
        float top_ = GLManager::get_height() * GLManager::resScaler_.y;
        float bottom_ = -top_;

        mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, camera->near_, camera->far_);
        shader->set_matrix("m4_projection", orthogonal);
        break;
    }
    }*/

    //bool fixed = (status & IS_FIXED) == IS_FIXED;
    //shader->set_bool("boolean_fix", fixed);

    //if (!fixed)
    //{
    //    // Send camera info to shader
    //    // mat4 viewport = mat4::look_at(camera->position, camera->right_, camera->up_, camera->back_);
        mat4 viewport = mat4::look_at(camera->position, camera->position + camera->back_, camera->up_);
        shader->set_matrix("m4_viewport", viewport);
    //}

    //bool isHerited = parent_ != nullptr;
    //shader->set_bool("boolean_herited", isHerited);
    //if (isHerited)
    //{
    //    Transform* pTransform = parent_->get_transform();
    //    shader->set_matrix("m4_parentTranslate", mat4::translate(pTransform->position));
    //    shader->set_matrix("m4_parentScale", mat4::scale(pTransform->scale));
    //    shader->set_matrix("m4_parentRotate", pTransform->orientation.to_mat4());
    //}

    //if (pModel->pMaterial_ && isLight_)
    //	LightingEffectPipeline(pModel->pMaterial_);

    //glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        shader->set_uint((name + number).c_str(), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void assimpMesh::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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