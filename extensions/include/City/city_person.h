#pragma once
#include "engine/entity.h"
#include "engine/system.h"
#include "utilities/json_utility.h"

namespace neko
{


class CityPeopleManager : public System
{
public:
	void Init() override;
	void Update() override;
	void Destroy() override;
protected:
	json personBehaviorTree_;
	std::vector<Entity> people_;
};
}
