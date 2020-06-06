#include "17_hello_frustum/frustum_program.h"
#include "imgui.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
namespace neko
{


void HelloFrustumProgram::Init()
{
    asteroidPositions_.resize(maxAsteroidNmb_);
    asteroidForces_.resize(maxAsteroidNmb_);
    asteroidVelocities_.resize(maxAsteroidNmb_);
    asteroidCulledPositions_.reserve(maxAsteroidNmb_);
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Calculate Positions");
#endif
    //Calculate init pos and velocities
    for (size_t i = 0; i < maxAsteroidNmb_; i++)
    {
        const float radius = RandomRange(20.0f, 300.0f);
        const degree_t angle = degree_t(RandomRange(0.0f, 360.0f));
        Vec3f position = Vec3f::forward;
        position = Vec3f(Transform3d::RotationMatrixFrom(angle, Vec3f::up) * Vec4f(position));
        position *= radius;
        asteroidPositions_[i] = position;
    }
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    const auto& config = BasicEngine::GetInstance()->config;
    model_.LoadModel(config.dataRootPath + "model/rock/rock.obj");

    
    vertexInstancingDrawShader_.LoadFromFile(
        config.dataRootPath + "shaders/17_hello_frustum/asteroid_vertex_instancing.vert",
        config.dataRootPath + "shaders/17_hello_frustum/asteroid.frag");
    screenShader_.LoadFromFile(config.dataRootPath + "shaders/17_hello_frustum/screen.vert",
        config.dataRootPath + "shaders/17_hello_frustum/screen.frag"
    );

    camera_.position = Vec3f(0.0f, 600.0f, -500.0f);
    camera_.farPlane = 1'000.0f;
    camera_.LookAt(Vec3f::zero);

    overCamera_.position = Vec3f(0.0f, 600.0f, -500.0f);
    overCamera_.farPlane = 1'000.0f;
    overCamera_.LookAt(Vec3f::zero);
    overCamera_.SetAspect(1024,1024);
	
    mainPlane_.Init();

    //Create Screen FBO
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    //Generate Color Buffer for FBO
    glGenTextures(1, &overViewTexture_);
    glBindTexture(GL_TEXTURE_2D, overViewTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024,1024, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    //Bind the Color Buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, overViewTexture_, 0);

    //Generate the Depth-Stencil RenderBuffer Object
    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,1024,1024);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //Bind depth-stencil RBO to screen FBO 
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        logDebug("[Error] Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HelloFrustumProgram::Update(seconds dt)
{
    if (!model_.IsLoaded())
    {
        return;
    }

    std::lock_guard<std::mutex> lock(updateMutex_);
    dt_ = dt.count();
    auto* engine = BasicEngine::GetInstance();
    const auto& config = BasicEngine::GetInstance()->config;
    camera_.SetAspect(config.windowSize.x, config.windowSize.y);
    camera_.Update(dt);
    //Kicking the velocity calculus for force and velocities
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Calculate Positions");
#endif
    asteroidCulledPositions_.clear();
    
    CalculateForce(0, asteroidNmb_);
    CalculateVelocity(0, asteroidNmb_);
    CalculatePositions(0, asteroidNmb_);
    Culling(0, asteroidNmb_);
	
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif


}

void HelloFrustumProgram::Destroy()
{
    model_.Destroy();
    vertexInstancingDrawShader_.Destroy();
    screenShader_.Destroy();
    mainPlane_.Destroy();
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(1, &overViewTexture_);
    glDeleteRenderbuffers(1, &rbo_);
}

void HelloFrustumProgram::DrawImGui()
{
    std::lock_guard<std::mutex> lock(updateMutex_);
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Asteroid Draw Imgui");
#endif
    ImGui::Begin("Frustum Culling");

    ImGui::SliderScalar("Asteroid Nmb", ImGuiDataType_U64, &asteroidNmb_, &minAsteroidNmb_, &maxAsteroidNmb_);

    const size_t minChunkSize = 100;
    const size_t maxChunkSize = 10'000;
    ImGui::SliderScalar("Instance Chunk Size", ImGuiDataType_U64, &instanceChunkSize_, &minChunkSize, &maxChunkSize);
    ImGui::LabelText("Asteroid Actual Nmb", "%zu", asteroidCulledPositions_.size());
    ImGui::End();
}

void HelloFrustumProgram::Render()
{
    if (!model_.IsLoaded())
    {
        return;
    }

    std::lock_guard<std::mutex> lock(updateMutex_);
    if (instanceVBO_ == 0)
    {
        const auto& asteroidMesh = model_.GetMesh(0);

        glBindVertexArray(asteroidMesh.GetVao());
        glGenBuffers(1, &instanceVBO_);

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO_);
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3f), (void*)0);
        glVertexAttribDivisor(5, 1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }


#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Draw Vertex Buffer Instaning");
#endif
    vertexInstancingDrawShader_.Bind();
    const auto& asteroidMesh = model_.GetMesh(0);
    asteroidMesh.BindTextures(vertexInstancingDrawShader_);

    const std::function<void()> drawAsteroids = [this, &asteroidMesh]() {
        const auto actualAsteroidNmb = asteroidCulledPositions_.size();

        for (size_t chunk = 0; chunk < actualAsteroidNmb / instanceChunkSize_ + 1; chunk++)
        {
            const size_t chunkBeginIndex = chunk * instanceChunkSize_;
            const size_t chunkEndIndex = std::min(actualAsteroidNmb, (chunk + 1) * instanceChunkSize_);
            if (chunkEndIndex > chunkBeginIndex)
            {
                const size_t chunkSize = chunkEndIndex - chunkBeginIndex;
#ifdef EASY_PROFILE_USE
                EASY_BLOCK("Set VBO Model Matrices");
#endif
                glBindBuffer(GL_ARRAY_BUFFER, instanceVBO_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f) * chunkSize, &asteroidCulledPositions_[chunkBeginIndex], GL_DYNAMIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
#ifdef EASY_PROFILE_USE
                EASY_END_BLOCK
                    EASY_BLOCK("Draw Mesh");

#endif
                glBindVertexArray(asteroidMesh.GetVao());
                glDrawElementsInstanced(GL_TRIANGLES, asteroidMesh.GetElementsCount(), GL_UNSIGNED_INT, 0,
                    chunkSize);
                glBindVertexArray(0);
            }
        }
    };
	//Draw overview on framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glViewport(0, 0, 1024, 1024);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vertexInstancingDrawShader_.SetMat4("view", overCamera_.GenerateViewMatrix());
    vertexInstancingDrawShader_.SetMat4("projection", overCamera_.GenerateProjectionMatrix());
    drawAsteroids();
	//Draw the true astroids view
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    const auto& config = BasicEngine::GetInstance()->config;
    glViewport(0, 0, config.windowSize.x, config.windowSize.y);
    vertexInstancingDrawShader_.SetMat4("view", camera_.GenerateViewMatrix());
    vertexInstancingDrawShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
    drawAsteroids();
	
	//Draw the mini view on top left
    glDisable(GL_DEPTH_TEST);
    screenShader_.Bind();
    const float miniMapSize = 0.2f;
    screenShader_.SetVec2("offset", Vec2f((1.0f - miniMapSize/camera_.aspect) , 1.0f - miniMapSize));
    screenShader_.SetVec2("scale", Vec2f( miniMapSize/camera_.aspect, miniMapSize));

    screenShader_.SetInt("screenTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, overViewTexture_);
    mainPlane_.Draw();
    glEnable(GL_DEPTH_TEST);

}

void HelloFrustumProgram::OnEvent(const SDL_Event& event)
{
    camera_.OnEvent(event);
}

void HelloFrustumProgram::CalculateForce(size_t begin, size_t end)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Calculate Forces");
#endif
    const size_t endCount = std::min(end, asteroidNmb_);
    for (auto i = begin; i < endCount; i++)
    {
        const auto deltaToCenter = Vec3f::zero - asteroidPositions_[i];
        const auto r = deltaToCenter.Magnitude();
        const auto force = gravityConst * centerMass * asteroidMass / (r * r);
        asteroidForces_[i] = deltaToCenter / r * force;
    }
}

void HelloFrustumProgram::CalculateVelocity(size_t begin, size_t end)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Calculate Velocities");
#endif
    const size_t endCount = std::min(end, asteroidNmb_);
    for (auto i = begin; i < endCount; i++)
    {
        const auto deltaToCenter = Vec3f::zero - asteroidPositions_[i];
        auto velDir = Vec3f(-deltaToCenter.z, 0.0f, deltaToCenter.x);
        velDir = velDir.Normalized();

        const auto speed = std::sqrt(asteroidForces_[i].Magnitude() / asteroidMass * deltaToCenter.Magnitude());
        asteroidVelocities_[i] = velDir * speed;
    }
}

void HelloFrustumProgram::CalculatePositions(size_t begin, size_t end)
{
    const size_t endCount = std::min(end, asteroidNmb_);
    for (auto i = begin; i < endCount; i++)
    {
        asteroidPositions_[i] += asteroidVelocities_[i] * dt_;
    }
}

void HelloFrustumProgram::Culling(size_t begin, size_t end)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Culling");
#endif
	const auto asteroidRadius = model_.GetMesh(0).GenerateBoundingSphere().radius_;
    const auto cameraDir = -camera_.reverseDirection;
    const auto cameraRight = camera_.GetRight();
    const auto cameraUp = camera_.GetUp();
    const auto fovX = camera_.GetFovX();
	
    const auto rightQuaternion = Quaternion::AngleAxis(fovX / 2.0f, cameraUp);
    const auto rightNormal = Vec3f(Transform3d::RotationMatrixFrom(rightQuaternion) * Vec4f(cameraRight));
    const auto leftQuaternion = Quaternion::AngleAxis(-fovX / 2.0f, cameraUp);
    const auto leftNormal = Vec3f(Transform3d::RotationMatrixFrom(leftQuaternion) * Vec4f(-cameraRight));
    const auto topQuaternion = Quaternion::AngleAxis(camera_.fovY / 2.0f, cameraRight);
    const auto topNormal = Vec3f(Transform3d::RotationMatrixFrom(topQuaternion) * Vec4f(-cameraUp));
    const auto bottomQuaternion = Quaternion::AngleAxis(-camera_.fovY / 2.0f, cameraRight);
    const auto bottomNormal = Vec3f(Transform3d::RotationMatrixFrom(bottomQuaternion) * Vec4f(cameraUp));
    for (size_t i = begin; i < end; i++)
    {
        const auto asteroidPos = asteroidPositions_[i];
        //Near
        {
            const auto planePos = camera_.position + cameraDir * camera_.nearPlane;
            const auto asterPos = asteroidPos - planePos;
            const auto v = Vec3f::Dot(cameraDir, asterPos);
        	if( v < -asteroidRadius)
                continue;
        }
    	//Far
        {
            const auto planePos = camera_.position + cameraDir * camera_.farPlane;
            const auto asterPos = asteroidPos - planePos;
            const auto v = Vec3f::Dot(camera_.reverseDirection, asterPos);
            if (v < -asteroidRadius)
                continue;
        }

    	//Right
        {
            const auto asterPos = asteroidPos - camera_.position;
            const auto v = Vec3f::Dot(rightNormal, asterPos);
        	if(v < -asteroidRadius)
        	{
                continue;
        	}
        }
    	//Left
        {
            const auto asterPos = asteroidPos - camera_.position;
            const auto v = Vec3f::Dot(leftNormal, asterPos);
            if (v < -asteroidRadius)
            {
                continue;
            }
        }
    	//Top
        {

            const auto asterPos = asteroidPos - camera_.position;
            const auto v = Vec3f::Dot(topNormal, asterPos);
            if (v < -asteroidRadius)
            {
                continue;
            }
        }
        //Bottom
        {
            
            const auto asterPos = asteroidPos - camera_.position;
            const auto v = Vec3f::Dot(bottomNormal, asterPos);
            if (v < -asteroidRadius)
            {
                continue;
            }
        }

        asteroidCulledPositions_.push_back(asteroidPositions_[i]);
    }
	
}
}


