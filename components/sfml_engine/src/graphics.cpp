//
// Created by efarhan on 10/14/19.
//

#include "sfml_engine/graphics.h"
#include "engine/globals.h"

namespace neko::sfml
{


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

void SfmlRenderCommand::Render(RenderTarget* renderTarget)
{
	auto* renderTargetPtr = static_cast<sf::RenderTarget*>(renderTarget->renderTargetPtr);
	if(drawable_ == nullptr || renderTargetPtr == nullptr)
		return;
	renderTargetPtr->draw(*drawable_, states_);
}
}
