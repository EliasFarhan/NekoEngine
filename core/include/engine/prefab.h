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

#include <vector>
#include <engine/globals.h>
#include <utilities/json_utility.h>
#include "entity.h"
#include <sole.hpp>
#include "component.h"

namespace neko
{

class SceneManager;
using PrefabId = sole::uuid;
const PrefabId INVALID_PREFAB_ID = sole::uuid{};
struct Prefab
{
    Prefab();
    PrefabId id = INVALID_PREFAB_ID;
	std::string prefabPath = "";
    json prefabJson{};
};

class PrefabManager : public ComponentManager<PrefabId, EntityMask(ComponentType::PREFAB)>
{
public:
    explicit PrefabManager(EntityManager& entityManager, SceneManager& sceneManager);
    void InstantiatePrefab(PrefabId prefabIndex, EntityManager& entityManager);
    PrefabId LoadPrefab(std::string_view prefabPath, bool forceReload=false);
    const Prefab& GetPrefab(PrefabId prefabId);
	static std::string_view GetExtension();
protected:
    SceneManager& sceneManager_;
	std::unordered_map<PrefabId, Prefab> prefabMap_;
};
}
