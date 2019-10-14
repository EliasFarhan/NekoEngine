#pragma once

#include <mathematics/vector.h>
#include <SFML/System/Vector2.hpp>
#include <Box2D/Common/b2Math.h>

namespace neko
{
template<>
Vec2f::Vec2f(const sf::Vector2f& v)
{
    this->x = v.x;
    this->y = v.y;
}
template <>
Vec2f::operator sf::Vector2f() const
{
    return sf::Vector2f(x, y);
}
template<>
Vec2f::Vec2f(const b2Vec2& v)
{
    this->x = v.x;
    this->y = v.y;
}
template <>
Vec2f::operator b2Vec2() const
{
    return b2Vec2(x, y);
}
}