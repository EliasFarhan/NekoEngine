#pragma once
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

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "graphics/graphics.h"
#include "gl/mesh.h"
#include "gl/shader.h"

namespace neko::gl
{
using ModelId = sole::uuid;
const ModelId INVALID_MODEL_ID = sole::uuid();

class Model
{
public:
    void Draw(const Shader& shader) const;
    void Destroy();
    [[nodiscard]] const assimp::Mesh& GetMesh(size_t meshIndex) const;
    [[nodiscard]] size_t GetMeshCount() const;
    void BindTextures(size_t meshIndex, const Shader & shader) const;
  private:
    friend class ModelLoader;
    std::vector<assimp::Mesh> meshes_;
};


class ModelLoader
{
public:
    enum ModelFlags : std::uint8_t
    {
        NONE = 0u,
        LOADED = 1u << 0u,
        ERROR_LOADING = 1u << 1u,

    };
    explicit ModelLoader(Assimp::Importer&, std::string_view path, ModelId modelId);
    ModelLoader(ModelLoader&&) noexcept ;
    ModelLoader(const ModelLoader& ) = delete;
    ModelLoader& operator=(const ModelLoader&) = delete;
    void Start();
    void Update();
    [[nodiscard]] bool IsDone() const;
    [[nodiscard]] ModelId GetModelId() const;
    [[nodiscard]] const Model* GetModel() const;
    [[nodiscard]] bool HasErrors() const;
private:
    /**
     * \brief Uses the assimp importer to load model from disk
     */
    void LoadModel();
    /**
     * \brief Process all the node of the aiScene, aka the meshes
     */
    void ProcessModel();
    void ProcessNode(aiNode* node);
    void ProcessMesh(assimp::Mesh&, const aiMesh* aMesh);
    void LoadMaterialTextures(const aiMaterial* material, aiTextureType textureType, std::string_view directory,
                              assimp::Mesh& mesh);
    /**
     * \brief method called on the Render thread to create the VAOs of the meshes
     */
    void UploadMeshesToGL();
    std::string path_;
    std::string directoryPath_;

    ModelId modelId_ = INVALID_MODEL_ID;

    std::reference_wrapper<Assimp::Importer> importer_;
    const aiScene* scene = nullptr;
    Model model_;

    Job loadModelJob_;
    Job processModelJob_;
    Job uploadMeshesToGLJob_;

    std::uint8_t flags_ = NONE;



};



class ModelManager : public SystemInterface
{
public:
    explicit ModelManager();
    ModelId LoadModel(std::string_view path);
    [[nodiscard]] const Model* GetModel(ModelId);
    [[nodiscard]] bool IsLoaded(ModelId);

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

private:
    std::map<std::string, ModelId> modelPathMap_;
    std::map<ModelId, Model> modelMap_;
    std::queue<ModelLoader> modelLoaders_;
    Assimp::Importer importer_;
};
}
