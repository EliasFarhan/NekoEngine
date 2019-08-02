#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

#include <graphics/spine.h>


namespace neko
{
class BasicSpineDrawable;
}
namespace editor
{
enum class PreviewStatus
{
    None,
    SpineAnimation,

};
class Previewer
{
public:
    void Update(float dt);

    void SetSpineAnimation(neko::BasicSpineDrawable* spineDrawablePtr);

    virtual ~Previewer();

private:
    neko::BasicSpineDrawable spineDrawable_;
    PreviewStatus status_ = PreviewStatus::None;
    sf::RenderTexture previewTexture_;
};

}