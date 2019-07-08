#include <graphics/spine_utility.h>
#include "engine/log.h"

namespace neko
{

SkeletonData* readSkeletonJsonData(const char* filename, Atlas* atlas, float scale) {
	SkeletonJson* json = SkeletonJson_create(atlas);
	json->scale = scale;
	SkeletonData* skeletonData = SkeletonJson_readSkeletonDataFile(json, filename);
	if (!skeletonData) 
	{
		logDebug(json->error);
		return nullptr;
	}
	SkeletonJson_dispose(json);
	return skeletonData;
}

SkeletonData* readSkeletonBinaryData(const char* filename, Atlas* atlas, float scale) {
	SkeletonBinary* binary = SkeletonBinary_create(atlas);
	binary->scale = scale;
	SkeletonData *skeletonData = SkeletonBinary_readSkeletonDataFile(binary, filename);
	if (!skeletonData) {
		logDebug(binary->error);
		return nullptr;
	}
	SkeletonBinary_dispose(binary);
	return skeletonData;
}
}
