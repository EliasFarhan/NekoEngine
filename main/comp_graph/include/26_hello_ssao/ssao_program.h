#include "comp_graph/sample_program.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "gl/model.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloSsaoProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	enum SsaoFlags : std::uint8_t
	{
		NONE = 0u,
		RESIZE_SCREEN = 1u,
		ENABLE_SSAO = 1u << 1u
	};
	void DestroyFramebuffer();
	void CreateFramebuffer();
	sdl::Camera3D camera_;
	gl::Texture container_;
	gl::Texture containerSpecular_;
	TextureId whiteTexture_ = 0;

	gl::Shader ssaoGeometryShader_;
	gl::Shader ssaoLightingShader_;
	gl::Shader ssaoShader_;
	gl::Shader ssaoBlurShader_;

	gl::RenderQuad plane_{Vec3f::zero, Vec2f::one};
	assimp::Model model_;

	unsigned int gBuffer_ = 0;
	unsigned int gPosition_ = 0;
	unsigned int gNormal_ = 0;
	unsigned int gAlbedoSpec_ = 0;
	unsigned int rbo_ = 0;

	unsigned int ssaoFbo_ = 0, ssaoBlurFbo_ = 0;
	unsigned int ssaoColorBuffer_ = 0, ssaoColorBufferBlur_ = 0;
	
};
}