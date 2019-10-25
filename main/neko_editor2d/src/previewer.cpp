
/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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
#include <tools/previewer.h>
#include <sfml_engine/spine.h>
#include <imgui.h>
#include <imgui-SFML.h>

namespace neko::editor
{

void Previewer::Update(float dt)
{


	switch (status_)
	{
	case PreviewStatus::SpineAnimation:
	{
	    if(spineDrawable_.skeletonData != nullptr)
        {
	        auto animations = spineDrawable_.skeletonData->animations;
            static const char* currentAnim = spineDrawable_.skeletonData->animations[0]->name;            // Here our selection is a single pointer stored outside the object.
            if (ImGui::BeginCombo("Animation", currentAnim)) // The second parameter is the label previewed before opening the combo.
            {
                for (int n = 0; n < spineDrawable_.skeletonData->animationsCount; n++)
                {
                    bool isSelected = (currentAnim == animations[n]->name);
                    if (ImGui::Selectable(animations[n]->name, isSelected))
                    {
                        currentAnim = animations[n]->name;
                        if (spineDrawable_.skeletonDrawable != nullptr)
                        {
                            spineDrawable_.SetAnimationByName(animations[n]->name);
                        }
                    };
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
                }
                ImGui::EndCombo();
            }
            auto skins = spineDrawable_.skeletonData->skins;
            static const char* currentSkin = spineDrawable_.skeletonData->skins[0]->name;            // Here our selection is a single pointer stored outside the object.
            if (ImGui::BeginCombo("Skin", currentSkin)) // The second parameter is the label previewed before opening the combo.
            {
                for (int n = 0; n < spineDrawable_.skeletonData->skinsCount; n++)
                {
                    bool isSelected = (currentSkin == skins[n]->name);
                    if (ImGui::Selectable(skins[n]->name, isSelected))
                    {
                        currentSkin = skins[n]->name;
                        if (spineDrawable_.skeletonDrawable != nullptr)
                        {
                            spineDrawable_.SetSkinByName(skins[n]->name);
                        }
                    };
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
                }
                ImGui::EndCombo();
            }
        }
        if (previewTexture_.getSize() == sf::Vector2u(0, 0))
        {
            const auto imWindowSize = ImGui::GetContentRegionAvail();
            previewTexture_.create(unsigned(imWindowSize.x), unsigned(imWindowSize.y));
        }
		previewTexture_.clear();
		if (spineDrawable_.skeletonDrawable != nullptr)
		{
		    spineDrawable_.skeletonDrawable->update(dt);
			spineDrawable_.SetPosition(sf::Vector2f(previewTexture_.getSize()) / 2.0f);
			previewTexture_.draw(*spineDrawable_.skeletonDrawable);
			auto view = previewTexture_.getView();
			const auto skeletonBound = spineDrawable_.skeletonDrawable->vertexArray->getBounds();
			const sf::Vector2f ratio = sf::Vector2f(float(skeletonBound.width) * 2.0f / view.getSize().x, float(skeletonBound.height) * 2.0f / view.getSize().y);
			view.setSize(view.getSize() * (ratio.x > ratio.y ? ratio.x : ratio.y));
			previewTexture_.setView(view);
		}
		previewTexture_.display();
		ImGui::Image(previewTexture_.getTexture());
		break;
	}
	default:
		break;
	}
}

void Previewer::SetSpineAnimation(const sfml::BasicSpineDrawable* spineDrawablePtr)
{
	spineDrawable_.atlas = spineDrawablePtr->atlas;
	spineDrawable_.skeletonData = spineDrawablePtr->skeletonData;
	spineDrawable_.skeletonDrawable = std::make_shared<spine::SkeletonDrawable>(
		spineDrawable_.skeletonData);
	spineDrawable_.skeletonDrawable->update(0.0f);
	status_ = PreviewStatus::SpineAnimation;

}

Previewer::~Previewer()
{
	spineDrawable_.atlas = nullptr;
	spineDrawable_.skeletonData = nullptr;
}
}