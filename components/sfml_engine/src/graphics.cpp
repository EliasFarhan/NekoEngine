//
// Created by efarhan on 10/14/19.
//

#include "sfml_engine/graphics.h"

namespace neko::sfml
{
void SfmlGraphicsManager::SetRenderTarget(sf::RenderTarget* renderTarget)
{
    renderTarget_ = renderTarget;

}

void SfmlGraphicsManager::Render(RenderCommand* command)
{
    if(command == nullptr or renderTarget_ == nullptr)
        return;
    auto* sfmlRenderCommand = dynamic_cast<SfmlRenderCommand*>(command);
    if(sfmlRenderCommand)
    {
        auto* drawable = sfmlRenderCommand->GetDrawable();
        if(drawable == nullptr)
            return;
        renderTarget_->draw(*drawable, sfmlRenderCommand->GetStates());
    }
}


const sf::RenderStates& SfmlRenderCommand::GetStates() const
{
    return states_;
}

void SfmlRenderCommand::SetStates(const sf::RenderStates& states)
{
    states_ = states;
}

const sf::Drawable* SfmlRenderCommand::GetDrawable() const
{
    return drawable_;
}

void SfmlRenderCommand::SetDrawable(sf::Drawable* drawable)
{
    drawable_ = drawable;
}
}
