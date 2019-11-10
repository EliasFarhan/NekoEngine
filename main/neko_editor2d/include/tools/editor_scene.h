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
#include <engine/scene.h>
#include <engine/component.h>
#include <sfml_engine/transform.h>
#include <sfml_engine/scene.h>

namespace neko
{
namespace box2d {
class BodyDef2dManager;
}

class Scale2dManager;
class PrefabManager;

namespace sfml
{
class Transform2dManager;
class SpriteManager;
class SpineManager;
}

class Rotation2dManager;
class Position2dManager;
}

namespace neko::editor
{
struct NekoEditorExport;
class EntityNameManager;
class EditorSceneManager : public sfml::SfmlBasicSceneManager
{
public:
	explicit EditorSceneManager(NekoEditorExport& editorExport);


    virtual json SerializeComponent(neko::Entity entity, neko::NekoComponentType componentType);
    virtual json SerializeEntity(neko::Entity entity);
    virtual json SerializeScene();


    void ClearScene() const;
    bool IsCurrentSceneTmp();
    void SaveCurrentScene();
    void LoadScene(std::string_view path);

    std::string_view GetSceneTmpPath();

    void ParseEntityJson(json& entityJson) override;
protected:

    void SaveScene(std::string_view path);

    std::map<NekoComponentType, std::function<json(Entity)>> componentSerializeFuncMap_;


    EntityNameManager& entityNameManager_;
    PrefabManager& prefabManager_;
	box2d::BodyDef2dManager& bodyDefManager_;
};


}
