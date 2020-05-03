#pragma once
#include <vector>
#include "mathematics/vector.h"
#include "gl/shader.h"
#include "gl/texture.h"

namespace neko::assimp
{

	struct Vertex
	{
		Vec3f position;
		Vec3f normal;
		Vec2f texCoords;
	};
	struct Texture
	{
		gl::TextureId id;
		enum class TextureType : std::uint8_t
		{
			DIFFUSE,
			SPECULAR
		};
		TextureType type;
	};
	class Mesh
	{
	public:
		std::vector<Vertex> vertices_;
		std::vector<unsigned int> indices_;
		std::vector<Texture> textures_;
		Mesh() = default;
		Mesh(std::vector<Vertex> vertices, const std::vector<unsigned int>& indices,
		     std::vector<Texture> textures);
		void Init();
		void Draw(const gl::Shader& shader);

		void Destroy();
	protected:
		//  render data
		unsigned int VAO, VBO, EBO;
		void SetupMesh();
	};
}
