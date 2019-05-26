#include <engine/transform.h>
#include "engine/globals.h"

namespace neko
{

Transform2dManager::Transform2dManager()
{
	positions.reserve(InitEntityNmb);
	scales.reserve(InitEntityNmb);
	angles.reserve(InitEntityNmb);
}
}
