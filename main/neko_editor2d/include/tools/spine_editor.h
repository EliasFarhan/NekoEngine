#include "neko_editor.h"

namespace neko::editor
{
class SpineEditorSystem : public BasicEditorSystem
{
public:
	void Init() override;
	void Update(float dt) override;
	void Destroy() override;
	void OnListingView() override;
	void OnMainView() override;
	void OnInspectorView() override;
	void OnSave() override;
	EditorSystemId GetEditorSystemId() const override;
private:
	sf::RenderTexture screenRenderTexture_;
	sf::CircleShape pivotCircle_;
	sfml::BasicSpineDrawable spineDrawable_;
};
}
