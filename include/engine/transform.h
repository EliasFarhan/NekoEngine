#pragma once

/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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

#include <vector>
#include "SFML/System/Vector2.hpp"
#include "globals.h"
#include <engine/entity.h>

namespace neko
{
class Physics2dManager;

/**
 * \brief manage the graphic transform of any kind of objects in a SOA fashions
 */
class Transform2dManager
{
public:
    Transform2dManager();

    Index AddPosition(sf::Vector2f position, Entity entity = INVALID_ENTITY);
    Index AddScale(sf::Vector2f scale, Entity entity = INVALID_ENTITY);
    Index AddAngle(float angle, Entity entity = INVALID_ENTITY);

    sf::Vector2f GetPosition(Index i) const;
	void SetPosition(const sf::Vector2f& position, Index i);

private:
    friend class SpriteManager;
    friend class ShapeManager;

    std::vector<sf::Vector2f> positions_;
    std::vector<sf::Vector2f> scales_;
    std::vector<float> angles_;
};
}
