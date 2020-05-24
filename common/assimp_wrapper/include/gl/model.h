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
    [[nodiscard]] size_t GetMeshCount() const
    {
	    return meshes_.size();
    };
    [[nodiscard]] const Mesh& GetMesh(size_t index) const
    {
	    return meshes_[index];
    };
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
