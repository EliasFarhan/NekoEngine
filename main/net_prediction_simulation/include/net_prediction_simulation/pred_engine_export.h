#pragma once
namespace neko
{
class EntityManager;
class Position2dManager;
class Scale2dManager;
class Rotation2dManager;
}
namespace neko::sfml
{
class Transform2dManager;
class ConvexShapeManager;
}
namespace neko::net
{
class PredSimEngine;
class VelocityManager;

struct PredSimEngineExport
{
	EntityManager& entityManager;
	Position2dManager& position2dManager;
	Scale2dManager& scale2dManager;
	Rotation2dManager& rotation2dManager;
	sfml::Transform2dManager& transform2dManager;
	PredSimEngine& engine;
	VelocityManager& velocityManager;
	sfml::ConvexShapeManager& shapeManager;
};
}