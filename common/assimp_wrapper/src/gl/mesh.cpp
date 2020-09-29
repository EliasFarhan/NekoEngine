/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "gl/mesh.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "assimp/material.h"


#include "gl/gles3_include.h"
#include "graphics/graphics.h"
#include "graphics/texture.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko::assimp
{
Mesh::Mesh() : loadMeshToGpu([this]
{
    SetupMesh();
})
{
}

void Mesh::Init()
{
#ifdef NEKO_SAMETHREAD
    loadMeshToGpu.Execute();
    const TextureManagerInterface& textureManager = TextureManagerLocator::get();
    for (auto& texture : textures_)
    {
        texture.textureName = textureManager.GetTexture(texture.textureId).name;
    }

#else
    RendererLocator::get().AddPreRenderJob(&loadMeshToGpu);
    const TextureManagerInterface& textureManager = TextureManagerLocator::get();
    for (auto& texture : textures_)
    {
    	//Waiting for texture to be loaded
    	while(!textureManager.IsTextureLoaded(texture.textureId))
    	{
    		
    	}
        texture.textureName = textureManager.GetTexture(texture.textureId).name;
    }
#endif
}


void Mesh::Draw(const gl::Shader& shader) const
{
    BindTextures(shader);
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

	for(auto& texture : textures_)
	{
        gl::DestroyTexture(texture.textureName);
	}
    textures_.clear();
    vertices_.clear();
    indices_.clear();

    loadMeshToGpu.Reset();
}


void Mesh::ProcessMesh(
    const aiMesh* mesh, 
    const aiScene* scene,
    const std::string_view directory)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Process Assimp Mesh");
#endif

    min_ = Vec3f(mesh->mAABB.mMin);
    max_ = Vec3f(mesh->mAABB.mMax);
	
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        Vec3f vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;
    	//TODO: why is tangent sometimes null even with CalcTangent
        if (mesh->mTangents != nullptr)
        {
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;
        }
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            Vec2f vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
            vertex.texCoords = Vec2f(0.0f, 0.0f);

        vertices_.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
	    const aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices_.push_back(face.mIndices[j]);
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        textures_.reserve(
        material->GetTextureCount(aiTextureType_SPECULAR) +
	        material->GetTextureCount(aiTextureType_DIFFUSE) + 
            material->GetTextureCount(aiTextureType_HEIGHT));
    	
        material->Get(AI_MATKEY_SHININESS, specularExponent_);
        LoadMaterialTextures(material,
            aiTextureType_DIFFUSE, Texture::TextureType::DIFFUSE, directory);
        LoadMaterialTextures(material,
            aiTextureType_SPECULAR, Texture::TextureType::SPECULAR, directory);
        LoadMaterialTextures(material,
            aiTextureType_HEIGHT, Texture::TextureType::HEIGHT, directory);
    }
}

bool Mesh::IsLoaded() const
{
    if (!loadMeshToGpu.IsDone())
    {
        return false;
    }
    const TextureManagerInterface& textureManager = TextureManagerLocator::get();
	for(const auto& texture : textures_)
	{
        if (!textureManager.IsTextureLoaded(texture.textureId))
        {
            return false;
        }

	}
    return true;
}

void Mesh::SetupMesh()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Create Mesh VAO");
    EASY_BLOCK("Generate Buffers");
    EASY_BLOCK("Generate VAO");
#endif
    glCheckError();
    glGenVertexArrays(1, &VAO);
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
    EASY_BLOCK("Generate VBO");
#endif
    glGenBuffers(1, &VBO);
    glCheckError();
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
    EASY_BLOCK("Generate EBO");
#endif
    glGenBuffers(1, &EBO);
    glCheckError();
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
    EASY_END_BLOCK;
    EASY_BLOCK("Copy Buffers");
#endif
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);
    glCheckError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int),
        &indices_[0], GL_STATIC_DRAW);
        glCheckError();
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
    EASY_BLOCK("Vertex Attrib");
#endif

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);
        glCheckError();
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
        glCheckError();
    // vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glCheckError();
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    glCheckError();
    glBindVertexArray(0);
    glCheckError();
}

Sphere Mesh::GenerateBoundingSphere() const
{
    Sphere s;
    s.radius_ = std::max(std::max(max_.x - min_.x, max_.y - min_.y), max_.z - min_.z);
    s.center_ = min_ + (max_ - min_) / 2.0f;
    return s;
}

void Mesh::LoadMaterialTextures(
    aiMaterial* material,
    aiTextureType aiTexture,
    Texture::TextureType texture,
    const std::string_view directory)
{
    auto& textureManager = TextureManagerLocator::get();
    for (unsigned int i = 0; i < material->GetTextureCount(aiTexture); i++)
    {
        aiString str;
        material->GetTexture(aiTexture, i, &str);
        
        textures_.emplace_back();
        auto& assTexture = textures_.back();
        assTexture.type = texture;
        std::string path = directory.data();
        path += '/';
    	path += str.C_Str();
    	
        assTexture.textureId = textureManager.LoadTexture(path);
    }
}

void Mesh::BindTextures(const gl::Shader& shader) const
{
    glCheckError();
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    for (size_t i = 0; i < textures_.size(); i++)
    {
        // activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
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
            case Texture::TextureType::HEIGHT:
                name = "texture_normal";
                number = std::to_string(normalNr++);
                break;
            default: ;
        }
        shader.SetInt("material." + name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures_[i].textureName);
    }
    shader.SetFloat("material.shininess", specularExponent_);
    shader.SetBool("enableNormalMap", normalNr > 1);
    glActiveTexture(GL_TEXTURE0);
    glCheckError();
}
}
