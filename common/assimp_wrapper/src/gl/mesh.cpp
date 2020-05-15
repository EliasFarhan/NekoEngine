
#include "gl/mesh.h"
#include "gl/gles3_include.h"

#include <utility>

namespace neko::assimp
{
Mesh::Mesh(std::vector<Vertex> vertices, const std::vector<unsigned>& indices,
           std::vector<Texture> textures) : vertices_(std::move(vertices)), indices_(indices), textures_(std::move(textures))
{
    SetupMesh();
}

void Mesh::Init()
{
    SetupMesh();
}

void Mesh::Draw(const gl::Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures_.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name;
    	switch(textures_[i].type)
    	{
        case Texture::TextureType::DIFFUSE:
            name = "texture_diffuse";
            number = std::to_string(diffuseNr++);
    		break;
        case Texture::TextureType::SPECULAR:
            name = "texture_specular";
            number = std::to_string(specularNr++);
    		break;
        default: ;
        }
        shader.SetInt("material." + name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::Destroy()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int),
        &indices_[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}
}
