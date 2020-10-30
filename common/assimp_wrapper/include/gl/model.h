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
