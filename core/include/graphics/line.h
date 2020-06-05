#pragma once


#include <vector>
#include "color.h"
#include "graphics.h"
#include "shape.h"
#include "graphics/line.h"
#include "mathematics/vector.h"
#include "graphics/graphics.h"

namespace neko
{

struct Line
{
	Vec3f origin_;
	Vec3f end_;
	Color3 lineColor_ = Vec3f(1,0,0);

	explicit Line(Vec3f origin, Vec3f end, Color3 lineColor = Color3(1,0,0)) :
	origin_(origin), end_(end), lineColor_(lineColor)
	{
		
	}
	
	explicit Line(Vec3f origin, Vec3f dir, float length, Color3 lineColor = Color3(1, 0, 0)) :
	origin_(origin), lineColor_(lineColor)
	{
		end_ = origin_ + dir * length;
	}
};

struct LineArray
{
	std::vector<Vec3f> positions_;
	std::vector<Color3> colors_;
};
	
/**
 * \brief Double buffered render system that draws lines
 */
class LineRenderer : public RenderProgram, public SyncBuffersInterface
{
public:
	void Init() override
	{
		
	}
	void Update([[maybe_unused]] seconds dt) override 
	{
		auto& renderer = RendererLocator::get();
		renderer.Render(this);
	}
	void Destroy() override
	{
		
	}
	/**
	 * \brief DrawLine in Normal Device Coordinate [-1,1]
	 */
	void DrawLine(const Line& line)
	{
		currentBuffer_.positions_.push_back(line.origin_);
		currentBuffer_.positions_.push_back(line.end_);
		currentBuffer_.colors_.push_back(line.lineColor_);
		currentBuffer_.colors_.push_back(line.lineColor_);
	}
	/**
	 * \brief Should be called at the beginning of the frame
	 * before all the DrawLine calls
	 */
	void SyncBuffers() override
	{
		std::swap(previousBuffer_.positions_,currentBuffer_.positions_);
		std::swap(previousBuffer_.colors_, currentBuffer_.colors_);
		currentBuffer_.positions_.clear();
		currentBuffer_.colors_.clear();
	}
protected:
	LineArray currentBuffer_;
	LineArray previousBuffer_;
};
}
