#pragma once
#include "gl/mesh.h"
#include "gl/shader.h"
#include <assimp/scene.h>

namespace neko::assimp
{
class Model
{
public:
    Model();
    void LoadModel(std::string_view path);
    bool IsLoaded() const;
    void Draw(const gl::Shader& shader);
    void Destroy();
private:
    // model data
    std::vector<Mesh> meshes_;
    std::string directory_;
    std::string path_;
    Job processModelJob_;
	
    void ProcessModel();
    void ProcessNode(aiNode* node, const aiScene* scene);
};
}
