#pragma once

namespace neko::editor
{
class EditorSceneSystem : public NekoEditorSystem
{
public:
	using NekoEditorSystem::NekoEditorSystem;
    void Init() override;

    void Update(float dt) override;

    void Destroy() override;


};
}