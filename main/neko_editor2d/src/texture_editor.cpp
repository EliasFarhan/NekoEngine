#include "tools/texture_editor.h"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Window/Event.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#include "utilities/file_utility.h"
#include "sfml_engine/vector.h"

namespace neko::editor
{

enum class TextureOrigin : std::uint8_t
{
	TOPLEFT = 0,
	CENTER,
	TOPRIGHT,
	BOTTOMRIGHT,
	BOTTOMLEFT,
	TOP,
	BOTTOM,
	RIGHT,
	LEFT,
	CUSTOM,
	LENGTH
};

TextureEditorSystem::TextureEditorSystem(NekoEditor& nekoEditor, sfml::TextureManager& textureManager) :
	BasicEditorSystem(nekoEditor.config),
	editor_(nekoEditor),
	textureManager_(textureManager)
{
	screenRenderTexture_.create(config_.gameWindowSize.first, config_.gameWindowSize.second);
}

void TextureEditorSystem::Init()
{
	pivotCircle_.setFillColor(sf::Color::Transparent);
	pivotCircle_.setOutlineColor(sf::Color::Red);
	pivotCircle_.setOutlineThickness(2.0f);
	
	editorMode_ = EditorSystemMode::TextureMode;
	ReloadTextureSet();
}

void TextureEditorSystem::Update([[maybe_unused]] float dt)
{
	screenRenderTexture_.clear(sf::Color::Black);
	const auto* texture = textureManager_.GetTexture(currentTextureId_);
	pivotCircle_.setPosition(sf::Vector2f(texture->origin * Vec2f(sf::Vector2f(texture->texture.getSize()))));
	pivotCircle_.setRadius(texture->texture.getSize().x * 0.1f);
	pivotCircle_.setOutlineThickness(texture->texture.getSize().x * 0.04f);

	pivotCircle_.setOrigin(sf::Vector2f(pivotCircle_.getRadius(), pivotCircle_.getRadius()));
	screenRenderTexture_.draw(screenSprite_);
	screenRenderTexture_.draw(pivotCircle_);
	screenRenderTexture_.display();
}

void TextureEditorSystem::Destroy()
{
}

void TextureEditorSystem::OnListingView()
{
	for (auto& textureId : textureSet_)
	{
		const auto textureName = textureManager_.GetTexturePath(textureId);
		if (ImGui::Selectable(textureName.c_str(), textureId == currentTextureId_))
		{
			editor_.RemoveEditorSystem(currentTextureId_, false);
			SetCurrentTextureId(textureId);
			editor_.SetCurrentEditorSystem(this);
		}
	}
}

void TextureEditorSystem::OnMainView()
{
	const auto tintColor = sf::Color::White;
	const auto borderColor = sf::Color::White;


	const auto imWindowSize = ImGui::GetWindowSize();
	auto windowSize = sf::Vector2f(imWindowSize.x, imWindowSize.y);
	const auto ratio = sf::Vector2f(windowSize.x / screenRenderTexture_.getSize().x,
		windowSize.y / screenRenderTexture_.getSize().y);
	windowSize = sf::Vector2f(screenRenderTexture_.getSize()) * (ratio.x < ratio.y ? ratio.x : ratio.y);


	ImGui::Image(screenRenderTexture_, sf::Vector2f(windowSize.x - 25.f, windowSize.y - 40.f),
		sf::FloatRect(sf::Vector2f(), sf::Vector2f(screenRenderTexture_.getSize())), tintColor, borderColor);
}

void TextureEditorSystem::OnInspectorView()
{
	if (currentTextureId_ == sfml::INVALID_TEXTURE_ID)
	{
		return;
	}
	const auto* texture = textureManager_.GetTexture(currentTextureId_);
	const auto texturePath = textureManager_.GetTexturePath(currentTextureId_);
	const auto textureName = GetFilename(texturePath);
	ImGui::LabelText("Texture Name", "%s", textureName.c_str());
	ImGui::LabelText("Texture Size", "(%d, %d)", texture->texture.getSize().x, texture->texture.getSize().y);
	ImGui::Separator();
	float tmpOrigin[2] = { texture->origin.x, texture->origin.y };
	if(ImGui::InputFloat2("Origin", tmpOrigin))
	{
		textureManager_.SetTextureOrigin(currentTextureId_, Vec2f(tmpOrigin[0], tmpOrigin[1]));
	}
	bool isSmooth = texture->texture.isSmooth();
	if(ImGui::Checkbox("Smooth Filter Mode", &isSmooth))
	{
		textureManager_.SetTextureSmooth(currentTextureId_, isSmooth);
	}
}

void TextureEditorSystem::OnSave()
{
	auto* texture = textureManager_.GetTexture(currentTextureId_);
	const auto textureName = textureManager_.GetTexturePath(currentTextureId_);
	const auto textureMetaName = textureName + sfml::TextureManager::GetMetaExtension().data();

	json textureMetaData;
	textureMetaData["origin"] = { texture->origin.x, texture->origin.y };
	textureMetaData["smooth"] = texture->texture.isSmooth();

	const std::string textureMetaStr = textureMetaData.dump(4);
	WriteStringToFile(textureMetaName, textureMetaStr);
	std::ostringstream oss;
	oss << "Saving texture meta data: " << textureMetaName;
	logDebug(oss.str());
}

void TextureEditorSystem::OnEvent(sf::Event& event)
{
	if (event.type == sf::Event::GainedFocus)
	{
		ReloadTextureSet();
	}
}

EditorSystemId TextureEditorSystem::GetEditorSystemId() const
{
	return currentTextureId_;
}

void TextureEditorSystem::SetCurrentTextureId(sfml::TextureId textureId)
{
	currentTextureId_ = textureId;
	auto* texture = textureManager_.GetTexture(currentTextureId_);
	screenSprite_.setTexture(texture->texture, true);

	screenRenderTexture_.create(texture->texture.getSize().x, texture->texture.getSize().y);

	resourcePath_ = textureManager_.GetTexturePath(textureId);
	editorSystemName_ = GetFilename(resourcePath_);

}

void TextureEditorSystem::ReloadTextureSet()
{
	logDebug("Reload textures set in Texture Editor System");
	neko::IterateDirectory(config_.dataRootPath, 
		[this](std::string_view path)
		{
			if (!neko::IsRegularFile(path))
				return;
			if (sfml::TextureManager::HasValidExtension(path))
			{
				const auto textureId = textureManager_.LoadTexture(path.data());
				textureSet_.emplace(textureId);
			}
		}, true);
}
}
