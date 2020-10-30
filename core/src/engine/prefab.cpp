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

#include <engine/prefab.h>

#include <engine/scene.h>

namespace neko
{
static const std::string_view prefabExtension = ".prefab";
PrefabManager::PrefabManager(EntityManager& entityManager, SceneManager& sceneManager) :
    ComponentManager(entityManager), sceneManager_(sceneManager)
{

}

PrefabId PrefabManager::LoadPrefab(const std::string_view prefabPath, bool forceReload)
{
	for(auto& prefabPair : prefabMap_)
	{
		if(prefabPair.second.prefabPath == prefabPath)
		{
			if(forceReload)
			{
				prefabPair.second.prefabJson = LoadJson(prefabPath);
			}
			return prefabPair.first;
		}
	}
	Prefab newPrefab;
	newPrefab.prefabPath = prefabPath;
	newPrefab.prefabJson = LoadJson(prefabPath);
	prefabMap_[newPrefab.id] = newPrefab;
	return newPrefab.id;
}



void PrefabManager::InstantiatePrefab(PrefabId prefabIndex, EntityManager& entityManager)
{
    auto prefabJson = prefabMap_[prefabIndex].prefabJson;
    const auto entityBase = entityManager.GetLastEntity() + 1;
    for (auto& entityJson: prefabJson["entities"])
    {
	    const Entity entity = entityJson["entity"];
        entityJson["entity"] = entity + entityBase;
	    const int parent = entityJson["parent"];
        if (parent != -1)
        {
            entityJson["parent"] = Entity(parent) + entityBase;
        }
        sceneManager_.ParseEntityJson(entityJson);
    }
	
}

std::string_view PrefabManager::GetExtension()
{
    return prefabExtension;
}

const Prefab& PrefabManager::GetPrefab(PrefabId prefabId)
{
    return prefabMap_[prefabId];
}

Prefab::Prefab() : id(sole::uuid0())
{

}
}