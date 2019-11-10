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
#include <engine/entity.h>
#include <engine/engine.h>


namespace neko {
namespace box2d {
class BodyDef2dManager;
}

class Rotation2dManager;
class Scale2dManager;
class Position2dManager;
}

namespace neko::sfml {
class SpineManager;
class TextureManager;
class SpriteManager;
class Transform2dManager;
}


namespace neko::editor
{
class CircleColliderDefManager;
class PolygonColldierDefManager;
class BoxColliderDefManager;
class EditorSceneManager;
struct NekoEditorExport;
class NekoEditor;
class Inspector
{
public:
	explicit Inspector(NekoEditorExport& nekoEditorExport);
    void ShowEntityInfo(neko::Entity entity) const;
private:
    sfml::Transform2dManager& transformManager_;
	EditorSceneManager& sceneManager_;
	EntityManager& entityManager_;
	Position2dManager& positionManager_;
	Scale2dManager& scaleManager_;
	Rotation2dManager& rotationManager_;
	sfml::SpriteManager& spriteManager_;
	sfml::TextureManager& textureManager_;
	sfml::SpineManager& spineManager_;
	Configuration& config_;
	box2d::BodyDef2dManager& bodyDefManager_;
	BoxColliderDefManager& boxColliderDefManager_;
	CircleColliderDefManager& circleColliderDefManager_;
	PolygonColldierDefManager& polygonColliderDefManager_;
	NekoEditor& editor_;
};
}
