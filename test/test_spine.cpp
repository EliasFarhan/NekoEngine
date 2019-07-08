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

#include <gtest/gtest.h>
#include <spine/spine-sfml.h>
#include "graphics/spine.h"
#include "engine/engine.h"
#include <engine/log.h>

class SpineTestEngine : public neko::BasicEngine
{
public:
	void Init() override
	{
		neko::BasicEngine::Init();
		catBodyAtlas = Atlas_createFromFile("data/spine/cat_noarm.atlas.txt", 0);
		if(catBodyAtlas == nullptr)
		{
			logDebug("Error while trying to load spine atlas file");
			return;
		}
		catBodyData = neko::readSkeletonJsonData("data/spine/cat_noarm.json", catBodyAtlas, 1.0f);
		if(catBodyData == nullptr)
		{
			logDebug("Error while trying to load spine skeleton file");
			return;
		}


		logDebug("Skins: ");
		for(int i = 0; i < catBodyData->skinsCount;i++)
		{
			auto* skin = catBodyData->skins[i];
			logDebug(skin->name);
		}
		logDebug("Slots: ");
		for(int i = 0; i < catBodyData->slotsCount;i++)
		{
			auto* slot = catBodyData->slots[i];
			logDebug(slot->name);
		}
		logDebug("Animations: ");
		for(int i = 0; i< catBodyData->animationsCount;i++)
		{
			auto* anim = catBodyData->animations[i];
			logDebug(anim->name);
		}

		catBody = std::make_unique<spine::SkeletonDrawable>(catBodyData);
		catBody->timeScale = 1;
		Skeleton* skeleton = catBody->skeleton;
		skeleton->flipX = false;
		skeleton->flipY = false;
		Skeleton_setToSetupPose(skeleton);

		skeleton->x = float(config.screenSize.x/2);
		skeleton->y = float(config.screenSize.y);
		Skeleton_updateWorldTransform(skeleton);
		AnimationState_setAnimation(catBody->state, 0, catBodyData->animations[0], 1);
	}
	void Update(float dt) override
	{
		neko::BasicEngine::Update(dt);
		if (catBody != nullptr)
		{
			catBody->update(dt);
			renderWindow->draw(*catBody);
		}
	}
	void Destroy() override
	{
		SkeletonData_dispose(catBodyData);
		Atlas_dispose(catBodyAtlas);
	}
private:
	Atlas* catBodyAtlas = nullptr;
	SkeletonData* catBodyData = nullptr;
	std::unique_ptr<spine::SkeletonDrawable> catBody = nullptr;
};

TEST(Spine, Cat)
{
	SpineTestEngine engine;
	engine.Init();
	engine.EngineLoop();

}