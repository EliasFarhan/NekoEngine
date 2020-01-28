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

    void OpenScene(std::string_view scenePath);

	void OnListingView() override;
	void OnMainView() override;
	void OnInspectorView() override;

    void OnSave() override;

    EditorSystemId GetEditorSystemId() const override;
    static EditorSystemId GenerateEditorSystemIdFrom(SceneId sceneId);
};
}