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
#include <memory>
#include <spine/spine-sfml.h>
#include "engine/entity.h"

namespace neko
{

struct BasicSpineDrawable : Component
{
	BasicSpineDrawable();
	virtual ~BasicSpineDrawable();
	Atlas* atlas = nullptr;
	SkeletonData* skeletonData = nullptr;
	std::unique_ptr<spine::SkeletonDrawable> skeletonDrawable = nullptr;
};

class SpineManager
{
public:
	void Init();
	void Update(EntityManager& entityManager, float dt);
	void Destroy();

	Entity AddSpineDrawable(Entity entity, const std::string_view atlasFilename, const std::string_view skeletonFilename);
private:
	std::vector<BasicSpineDrawable> spineDrawables_;
};


SkeletonData* readSkeletonJsonData(const char* filename, Atlas* atlas, float scale);
SkeletonData* readSkeletonBinaryData(const char* filename, Atlas* atlas, float scale);
}
