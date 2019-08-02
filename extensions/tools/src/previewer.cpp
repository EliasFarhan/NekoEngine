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