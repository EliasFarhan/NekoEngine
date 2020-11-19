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

#include <vector>

#include "assimp/material.h"
#include "mathematics/vector.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "mathematics/circle.h"

struct aiMesh;
struct aiScene;

namespace neko::assimp
{

	struct Vertex
	{
		Vec3f position;
		Vec3f normal;
		Vec2f texCoords;
        Vec3f tangent;
        Vec3f bitangent;
    };
	struct Texture
	{
		Texture() = default;
		TextureId textureId = INVALID_TEXTURE_ID;
		TextureName textureName = INVALID_TEXTURE_NAME;
		enum class TextureType : std::uint8_t
		{
			DIFFUSE,
			SPECULAR,
			HEIGHT
		};
		TextureType type = TextureType::DIFFUSE;
	};
	class Mesh
	{
	public:
		Mesh();
		void Init();
		void Draw(const gl::Shader& shader) const;
        void BindTextures(const gl::Shader& shader) const;
		void Destroy();

		void ProcessMesh(const aiMesh* mesh, const aiScene* scene,
			const std::string_view directory);
		bool IsLoaded() const;


		[[nodiscard]] unsigned int GetVao() const {return VAO;}
		[[nodiscard]] size_t GetElementsCount() const {return indices_.size();}

		[[nodiscard]] Sphere GenerateBoundingSphere() const;
	protected:

		void LoadMaterialTextures(aiMaterial* material, aiTextureType aiTexture, Texture::TextureType texture,
			const std::string_view directory);
		std::vector<Vertex> vertices_;
		std::vector<unsigned int> indices_;
		std::vector<Texture> textures_;
		float specularExponent_ = 0.0f;
		Vec3f min_, max_;
		Job loadMeshToGpu;
		//  render data
		unsigned int VAO = 0, VBO = 0, EBO = 0;
		/**
		 * \brief This function is called on the render thread as a pre-render job
		 */
		void SetupMesh();
	};
}
