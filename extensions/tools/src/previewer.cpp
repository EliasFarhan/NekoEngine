
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
#include <graphics/spine.h>
#include <imgui.h>
#include <imgui-SFML.h>

namespace editor
{

void Previewer::Update(float dt)
{
    if(previewTexture_.getSize() == sf::Vector2u(0,0))
    {
        const auto imWindowSize = ImGui::GetWindowSize();
        previewTexture_.create(imWindowSize.x, imWindowSize.y);
    }
    switch (status_)
    {
        case PreviewStatus::SpineAnimation:
        {
            previewTexture_.clear();
            if(spineDrawable_.skeletonDrawable != nullptr)
            {
                spineDrawable_.SetPosition(sf::Vector2f(previewTexture_.getSize())/2.0f);
                previewTexture_.draw(*spineDrawable_.skeletonDrawable);
            }
            previewTexture_.display();
            ImGui::Image(previewTexture_.getTexture());
            break;
        }
        default:
            break;
    }
}

void Previewer::SetSpineAnimation(neko::BasicSpineDrawable* spineDrawablePtr)
{
    spineDrawable_.atlas = spineDrawablePtr->atlas;
    spineDrawable_.skeletonData = spineDrawablePtr->skeletonData;
    spineDrawable_.skeletonDrawable = std::make_shared<spine::SkeletonDrawable>(
            spineDrawable_.skeletonData);
    spineDrawable_.skeletonDrawable->update(0.0f);

    auto rect = spineDrawable_.skeletonDrawable->vertexArray->getBounds();

    status_ = PreviewStatus::SpineAnimation;

}

Previewer::~Previewer()
{
    spineDrawable_.atlas = nullptr;
    spineDrawable_.skeletonData = nullptr;
}
}