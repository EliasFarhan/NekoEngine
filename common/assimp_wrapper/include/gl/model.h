#pragma once
#include "gl/mesh.h"
#include "gl/shader.h"
#include <assimp/scene.h>

namespace neko::assimp
{
class Model
{
public:
    void LoadModel(std::string_view path);
    void Draw(const gl::Shader& shader);
    void Destroy();
private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;

    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene) const;
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                              Texture::TextureType typeName) const;
};
}
