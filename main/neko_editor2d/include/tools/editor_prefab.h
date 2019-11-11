#pragma once

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

#include <engine/prefab.h>
#include "SFML/Graphics/Rect.hpp"

namespace neko {
class Position2dManager;
}

namespace neko::sfml
{
class SpineManager;
class Transform2dManager;
class SpriteManager;
}
namespace neko::editor
{
class CircleColliderDefManager;
class BoxColliderDefManager;
struct NekoEditorExport;

class EditorPrefabManager : public neko::PrefabManager
{
public:
    explicit EditorPrefabManager(NekoEditorExport& editorExport);
    const std::string& GetCurrentPrefabPath() const;
    void SetCurrentPrefabPath(const std::string& currentPrefabPath);


    void SaveCurrentPrefab();
    neko::Index CreatePrefabFromEntity(neko::Entity entity);
    sf::FloatRect CalculatePrefabBound();
protected:
    void SavePrefab(const std::string_view path);

    Prefab* currentPrefab_ = nullptr;

    EntityManager& entityManager_;
	sfml::Transform2dManager& transformManager_;
	Position2dManager& position2dManager_;
	sfml::SpriteManager& spriteManager_;

	BoxColliderDefManager& boxColliderDefManager_;
	CircleColliderDefManager& circleColliderDefManager_;
	sfml::SpineManager& spineManager_;
};



}
