#include "app.h"
#include "sqlite3.h"
#include <gl/gles3_include.h>
#include <gl/texture.h>

namespace neko
{
void PerfApp::Init()
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    auto rc = sqlite3_open("instancing_perf.db", &db);
    if (rc != SQLITE_OK)
    {
        logDebug("Could not open db to create table");
    }
    char* errMsg = 0;
    char* createTableSql = "DROP TABLE IF EXISTS RenderData;"
        "CREATE TABLE RenderData(Id INTEGER PRIMARY KEY AUTOINCREMENT, Frame INT, Instance INT, Segment INT, Indice INT, Time INT);";
    rc = sqlite3_exec(db, createTableSql, 0, 0, &errMsg);
    if(rc != SQLITE_OK)
    {
        logDebug(fmt::format("Could not create table with msg: {}", errMsg));
        sqlite3_free(errMsg);
    }
    sqlite3_close(db);
    camera3d_.farPlane = 2000.0f;
    camera3d_.position = Vec3f(0.0f, 300.0f, -300.0f);
    camera3d_.WorldLookAt(Vec3f::zero);
    positions_.reserve(maxInstanceNmb);
    for(int i = 0; i < 1000; i++)
    {
        for(size_t j = 0; j < 100; j++)
        {
            positions_.emplace_back(float(i - 500), 0.0f, float(j));
        }
    }
}

void PerfApp::Update(seconds dt)
{
    RendererLocator::get().Render(this);
}

void PerfApp::Destroy()
{
}

void PerfApp::Render()
{
    if(sphere_.VAO == 0)
    {
        InitGraphics();
    }
    const auto start = std::chrono::high_resolution_clock::now();
    instanceShader_.Bind();
    instanceShader_.SetTexture("ourTexture", texture_);
    instanceShader_.SetMat4("view", camera3d_.GenerateViewMatrix());
    instanceShader_.SetMat4("proj", camera3d_.GenerateProjectionMatrix());
    glBindVertexArray(sphere_.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f) * instanceNmb_, &positions_[0], GL_DYNAMIC_DRAW);

    glDrawElementsInstanced(GL_TRIANGLE_STRIP,
        sphere_.GetIndicesNmb(), 
        GL_UNSIGNED_INT, 
        nullptr, 
        instanceNmb_);
    glFinish();
    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    currentFrame++;
    //Save current frame data
    if(currentFrame < framePerSample)
    {
        frameDatas_.push_back({ elapsed.count(), currentFrame });
    }
    //Switch to new sample
    if (currentFrame == framePerSample)
    {
        const auto oldInstanceNmb = instanceNmb_;
        const auto oldSegmentNmb = segments_;
        const auto oldIndicesNmb = sphere_.GetIndicesNmb();
        auto newInstanceNmb = instanceNmb_ * instanceStep;
        auto newSegmentNmb = segments_;
        if (newInstanceNmb > maxInstanceNmb)
        {
            newInstanceNmb = minInstanceNmb;
            newSegmentNmb *= segmentStep;
        }
        if(newSegmentNmb > maxSegmentNmb)
        {
            currentFrame = framePerSample + 1;
            return;
        }
        sqlite3* db;
        sqlite3_stmt* stmt;
        auto rc = sqlite3_open("instancing_perf.db", &db);
        for(const auto& data : frameDatas_)
        {
            auto sqlInsertQuery = fmt::format(
                "INSERT INTO RenderData (Frame,Instance,Segment,Indice,Time) VALUES({}, {}, {}, {}, {});", 
                data.frame, 
                oldInstanceNmb, 
                oldSegmentNmb, 
                oldIndicesNmb, 
                data.renderTime);
            char* errMsg = 0;
            rc = sqlite3_exec(db, sqlInsertQuery.c_str(), 0, 0, &errMsg);
            if(rc != SQLITE_OK)
            {
                logDebug(fmt::format("Could not insert value, error msg: {}", errMsg));
                sqlite3_free(errMsg);
            }
        }
        sqlite3_close(db);
        ReloadSample(newInstanceNmb, newSegmentNmb);
    }
}

void PerfApp::InitGraphics()
{
    logDebug("Init graphics");
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    instanceShader_.LoadFromFile(
        config.dataRootPath+"shaders/sphere.vert",
        config.dataRootPath+"shaders/sphere.frag");
    texture_ = gl::CreateTextureFromKTX(
        config.dataRootPath + "textures/wall.jpg.ktx", 
        BasicEngine::GetInstance()->GetFilesystem());
    ReloadSample(minInstanceNmb, minSegmentNmb);
    glBindVertexArray(sphere_.VAO);
    glGenBuffers(1, &instanceVbo_);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVbo_);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3f), (void*)0);
    glVertexAttribDivisor(6, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glCheckError();


}

void PerfApp::ReloadSample(size_t instance, size_t segment)
{

    currentFrame = 0;

    instanceNmb_ = instance;
    segments_ = segment;
    sphere_.Destroy();
    sphere_ = gl::RenderSphere(Vec3f::zero, 1.0f, segments_);
    sphere_.Init();

    
}
}
