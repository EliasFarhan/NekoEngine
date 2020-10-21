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

#include "24_hello_cascaded_shadow/cascaded_shadow_program.h"
#include "imgui.h"
#include "mathematics/aabb.h"

namespace neko
{

void HelloCascadedShadowProgram::Init()
{
    textureManager_.Init();
    const auto& config = BasicEngine::GetInstance()->config;
    glCheckError();
    plane_.Init();
    // Create the FBO
    glGenFramebuffers(1, &fbo_);

    // Create the depth buffer
    glGenTextures(shadowMaps_.size(), &shadowMaps_[0]);

    for (auto shadowMap : shadowMaps_)
    {
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    }

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMaps_[0], 0);

    // Disable writes to the color buffer
    GLenum drawBuffers = GL_NONE;
    glDrawBuffers(1, &drawBuffers);
    glReadBuffer(GL_NONE);

    glCheckFramebuffer();
    glCheckError();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    simpleDepthShader_.LoadFromFile(
            config.dataRootPath + "shaders/24_hello_cascaded_shadow/simple_depth.vert",
            config.dataRootPath + "shaders/24_hello_cascaded_shadow/simple_depth.frag"
    );
    shadowShader_.LoadFromFile(
            config.dataRootPath + "shaders/24_hello_cascaded_shadow/shadow.vert",
            config.dataRootPath + "shaders/24_hello_cascaded_shadow/shadow.frag"
    );
    brickWallId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/brickwall/brickwall.jpg");

    dragonModel_.LoadModel(config.dataRootPath + "model/dragon/dragon.obj");
    glGenTextures(1, &whiteTexture_);
    glBindTexture(GL_TEXTURE_2D, whiteTexture_);
    unsigned char white[] = {255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glCheckError();

    camera_.position = Vec3f(0, 3.0f, -3.0f);
    camera_.WorldLookAt(Vec3f::forward * camera_.farPlane / 2.0f);
    camera_.farPlane = 100.0f;
}

void HelloCascadedShadowProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->config;
    camera_.SetAspect(config.windowSize.x, config.windowSize.y);
    camera_.Update(dt);	textureManager_.Update(dt);
}

void HelloCascadedShadowProgram::Destroy()
{
    simpleDepthShader_.Destroy();
    shadowShader_.Destroy();
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(shadowMaps_.size(), &shadowMaps_[0]);
    dragonModel_.Destroy();
    plane_.Destroy();
    textureManager_.Destroy();
}

void HelloCascadedShadowProgram::DrawImGui()
{
    ImGui::Begin("Cascaded Shadow Program");

    ImGui::SliderFloat("Near Ratio", &cascadedNearRatio_, 0.0001f, cascadedMiddleRatio_);
    ImGui::SliderFloat("Middle Ratio", &cascadedMiddleRatio_, cascadedNearRatio_, 1.0f);
    if (ImGui::InputFloat3("Light Direction", &lights_[0].direction[0]))
    {
        lights_[1].direction = lights_[2].direction = lights_[0].direction;
    }
    bool enableAabbCascade = flags_ & ENABLE_AABB_CASCADE;
    if(ImGui::Checkbox("Enable AABB Cascade", &enableAabbCascade))
    {
        flags_ = enableAabbCascade ? flags_ | ENABLE_AABB_CASCADE : flags_ & ~ENABLE_AABB_CASCADE;
    }
    bool enableCascadeColor = flags_ & ENABLE_CASCADE_COLOR;
    if(ImGui::Checkbox("Enable Cascade Color", &enableCascadeColor))
    {
        flags_ = enableCascadeColor? flags_ | ENABLE_CASCADE_COLOR : flags_ & ~ENABLE_CASCADE_COLOR;
    }
    ImGui::End();
}

void HelloCascadedShadowProgram::Render()
{
    if (!dragonModel_.IsLoaded())
    {
        return;
    }
    if (brickWall_ == INVALID_TEXTURE_NAME)
    {
        brickWall_ = textureManager_.GetTexture(brickWallId_).name;
        return;
    }
    std::lock_guard<std::mutex> lock(updateMutex_);
    glCheckError();
    //Cascade Shadow Pass
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    simpleDepthShader_.Bind();
    for (int i = 0; i < 3; i++)
    {
        ShadowPass(i);
    }
    //Render scene from camera
    const auto& config = BasicEngine::GetInstance()->config;
    glViewport(0, 0, config.windowSize.x, config.windowSize.y);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shadowShader_.Bind();

    shadowShader_.SetMat4("view", camera_.GenerateViewMatrix());
    shadowShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
    shadowShader_.SetTexture("material.texture_diffuse1", whiteTexture_);
    for (size_t i = 0; i < lights_.size(); i++)
    {
        shadowShader_.SetMat4("lightSpaceMatrices[" + std::to_string(i) + "]", lights_[i].lightSpaceMatrix);
        shadowShader_.SetVec3("lights[" + std::to_string(i) + "].direction", lights_[i].direction);
    }
    shadowShader_.SetVec3("viewPos", camera_.position);
    for (size_t i = 0; i < shadowMaps_.size(); i++)
    {
        shadowShader_.SetTexture("lights[" + std::to_string(i) + "].shadowMap", shadowMaps_[i], i + 3);
    }
    shadowShader_.SetBool("enableCascadeColor", flags_ & ENABLE_CASCADE_COLOR);
    shadowShader_.SetFloat("farPlane", camera_.farPlane);
    shadowShader_.SetFloat("bias", shadowBias_);
    shadowShader_.SetFloat("maxNearCascade", camera_.farPlane * cascadedNearRatio_);
    shadowShader_.SetFloat("maxMiddleCascade", camera_.farPlane * cascadedMiddleRatio_);
    RenderScene(shadowShader_);
}

void HelloCascadedShadowProgram::OnEvent(const SDL_Event& event)
{
    camera_.OnEvent(event);
}

Camera2D HelloCascadedShadowProgram::CalculateOrthoLight(float cascadeNear, float cascadeFar, Vec3f lightDir) const
{
    Camera2D lightCamera;
    Camera3D camera = static_cast<Camera3D>(camera_);
    camera.nearPlane = cascadeNear;
    camera.farPlane = cascadeFar;
    lightCamera.position = Vec3f::zero;
    lightCamera.WorldLookAt(lightDir);

    const auto tanHalfFovY = Tan(camera_.fovY / 2.0f);
    const auto tanHalfFovX = tanHalfFovY * camera_.aspect;
    const float nearX = cascadeNear * tanHalfFovX;
    const float nearY = cascadeNear * tanHalfFovY;
    const float farX = cascadeFar * tanHalfFovX;
    const float farY = cascadeFar * tanHalfFovY;

    const auto view = camera.GenerateViewMatrix();
    const auto lightView = lightCamera.GenerateViewMatrix();
    Vec3f frustumCorners[8] =
    {
        // near face
        camera.position-cascadeNear*camera.reverseDir+camera.rightDir*nearX+camera.upDir*nearY,
        camera.position-cascadeNear*camera.reverseDir-camera.rightDir*nearX+camera.upDir*nearY,
        camera.position-cascadeNear*camera.reverseDir+camera.rightDir*nearX-camera.upDir*nearY,
        camera.position-cascadeNear*camera.reverseDir-camera.rightDir*nearX-camera.upDir*nearY,

        camera.position-cascadeFar*camera.reverseDir+camera.rightDir*farX+camera.upDir*farY,
        camera.position-cascadeFar*camera.reverseDir-camera.rightDir*farX+camera.upDir*farY,
        camera.position-cascadeFar*camera.reverseDir+camera.rightDir*farX-camera.upDir*farY,
        camera.position-cascadeFar*camera.reverseDir-camera.rightDir*farX-camera.upDir*farY,
    };

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();

    float radius = 0.0f;
    Vec3f center = Vec3f::zero;
    for (auto & frustumCorner : frustumCorners)
    {
        center += frustumCorner;
        if(flags_ & ENABLE_AABB_CASCADE)
        {
            const auto frustumCornersLight = lightView * Vec4f(frustumCorner, 1.0f);
            minX = std::min(minX, frustumCornersLight.x);
            maxX = std::max(maxX, frustumCornersLight.x);
            minY = std::min(minY, frustumCornersLight.y);
            maxY = std::max(maxY, frustumCornersLight.y);
            minZ = std::min(minZ, frustumCornersLight.z);
            maxZ = std::max(maxZ, frustumCornersLight.z);
        }
    }
    if(flags_ & ENABLE_AABB_CASCADE)
    {
        //const auto maxSizeLength = std::max((maxX-minX)/2.0f, (maxY-minY)/2.0f);
        const auto size = Vec2f((maxX - minX) / 2.0f, (maxY - minY) / 2.0f);
        //const auto lightCenter = lightView.Inverse()*Vec4f((minX+maxX)/2.0f, (minY+maxY)/2.0f, (minZ+maxZ)/2.0f, 1.0f);
        center /= 8.0f;// = Vec3f(lightCenter);
        lightCamera.SetExtends(size);
        lightCamera.nearPlane = 0.0f;

        lightCamera.farPlane = (maxZ-minZ)*1.7f;
        lightCamera.position = center + lightCamera.reverseDir * lightCamera.farPlane/2.0f;
    }
    else
    {
        center /= 8.0f;
        for(const auto& frustumCorner : frustumCorners)
        {
            const float length = (frustumCorner-center).SquareMagnitude();
            if(length > radius)
            {
                radius = length;
            }
        }
        radius = std::sqrt(radius);
        lightCamera.SetExtends(Vec2f(radius, radius));
        lightCamera.farPlane = 2.0f * radius;
        lightCamera.nearPlane = 0.0f;
        lightCamera.position = center + lightCamera.reverseDir * radius;
    }
    return lightCamera;
}

void HelloCascadedShadowProgram::ShadowPass(int cascadeIndex)
{
    const auto cascadeNear = cascadeIndex == 0 ? camera_.nearPlane :
                             cascadeIndex == 1 ? camera_.farPlane * cascadedNearRatio_ :
                             camera_.farPlane * cascadedMiddleRatio_;
    const auto cascadeFar = cascadeIndex == 0 ? camera_.farPlane * cascadedNearRatio_ :
                            cascadeIndex == 1 ? camera_.farPlane * cascadedMiddleRatio_ :
                            camera_.farPlane;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMaps_[cascadeIndex], 0);
    glClear(GL_DEPTH_BUFFER_BIT);


    auto lightCamera = CalculateOrthoLight(cascadeNear, cascadeFar, lights_[cascadeIndex].direction);
    lights_[cascadeIndex].position = lightCamera.position;
    lights_[cascadeIndex].lightSpaceMatrix =
            lightCamera.GenerateProjectionMatrix() * lightCamera.GenerateViewMatrix();

    simpleDepthShader_.SetFloat("lightFarPlane", lightCamera.farPlane);
    simpleDepthShader_.SetMat4("lightSpaceMatrix",
                               lights_[cascadeIndex].lightSpaceMatrix);

    RenderScene(simpleDepthShader_);
}

void HelloCascadedShadowProgram::RenderScene(const gl::Shader& shader)
{
    for (int z = 0; z < 5; z++)
    {
        for (int x = -1; x < 2; x++)
        {
            auto model = Mat4f::Identity;
            model = Transform3d::Scale(model, Vec3f::one * 0.2f);
            model = Transform3d::Translate(model,
                                           Vec3f(-10.0f * float(x), 0.0f, 10.0f * float(z) + 5.0f));
            shader.SetMat4("model", model);
            shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
            dragonModel_.Draw(shader);
        }
    }
    auto model = Mat4f::Identity;
    model = Transform3d::Rotate(model, Quaternion::AngleAxis(degree_t(-90.0f), Vec3f::right));
    model = Transform3d::Scale(model, Vec3f::one * camera_.farPlane);
    model = Transform3d::Translate(model, Vec3f::forward * camera_.farPlane / 2.0f);
    shader.SetTexture("material.texture_diffuse1", brickWall_);
    shader.SetMat4("model", model);
    shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
    plane_.Draw();
}
}
