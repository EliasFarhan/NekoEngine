#pragma once

#include <graphics/graphics.h>
#include <SFML/Graphics/RenderTarget.hpp>

namespace neko::sfml
{

class SfmlRenderCommand : public RenderCommand
{
public:
    const sf::RenderStates& GetStates() const;
    void SetStates(const sf::RenderStates& states);
    const sf::Drawable* GetDrawable() const;
    void SetDrawable(sf::Drawable* drawable);
private:
    sf::Drawable* drawable_;
    sf::RenderStates states_;
};

class SfmlGraphicsManager : public GraphicsManager
{
public:
    void SetRenderTarget(sf::RenderTarget* renderTarget);
private:
    sf::RenderTarget* renderTarget_ = nullptr;

};

}
