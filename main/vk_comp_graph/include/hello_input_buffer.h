#pragma once

#include "sample_program.h"
#include <vulkan/vulkan.h>

namespace neko::vk
{

class HelloInputBuffer : public SampleProgram
{
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);
            return attributeDescriptions;
        }

    };
public:
    using SampleProgram::SampleProgram;
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

    void OnEvent(const SDL_Event& event) override;

    void CleanupSwapChain() override;
    void CreateSwapChain() override;
private:
    void CreateGraphicsPipeline();
    void CreateInputBuffer();
    void CleanupInputBuffer();

    VkPipelineLayout pipelineLayout_;
    VkPipeline graphicsPipeline_;
    VkBuffer vertexBuffer_;
    VmaAllocation allocation_;

    const std::vector<Vertex> vertices_ = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
};
}
