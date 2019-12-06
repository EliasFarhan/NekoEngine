#include "neko_editor.h"

namespace neko::editor
{
class SpineEditorSystem : public BasicEditorSystem
{
public:
    explicit SpineEditorSystem(Configuration& config);
	void Init() override;
	void Update(float dt) override;
	void Destroy() override;
	void OnListingView() override;
	void OnMainView() override;
	void OnInspectorView() override;
	void OnSave() override;
	EditorSystemId GetEditorSystemId() const override;

	void SetCurrentSpine(std::string_view spineFilename);
private:
    void ReloadSpineSet();

    std::set<sfml::SpineId> spineSet_;
	sf::RenderTexture screenRenderTexture_;

	EntityManager entityManager_;
	sfml::SpineManager spineManager_;
};
}
