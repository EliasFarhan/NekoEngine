#pragma once
/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

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

#include <utilities/json_utility.h>
#include "entity.h"
#include <engine/component.h>
#include "graphics/color.h"
#include <sole.hpp>

namespace neko
{

using SceneId = sole::uuid;
const SceneId INVALID_SCENE_ID = sole::uuid{};
struct Scene
{
    std::string sceneName = "New Scene";
    std::string scenePath = "";
	SceneId sceneId = INVALID_SCENE_ID;
};

class SceneManager 
{
public:
    explicit SceneManager(EntityManager& entityManager);
	virtual ~SceneManager() = default;
    virtual void ParseComponentJson(const json& componentJson, Entity entity) = 0;
    virtual void ParseEntityJson(const json& entityJson) = 0;
    virtual void ParseSceneJson(const json& sceneJson);

    const Scene& GetCurrentScene() const { return currentScene_;}
    void SetCurrentScene(const Scene& currentScene);
	static SceneId GenerateSceneId() { return sole::uuid0(); };
	static std::string_view GetExtension();
protected:
    std::map<ComponentType, std::function<void(Entity, const json&)>> componentParsingFuncMap_;
    Scene currentScene_;
    EntityManager& entityManager_;
};

}
