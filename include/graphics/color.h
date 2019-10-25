#pragma once
#include <cstdint>
#include <ostream>

namespace neko
{

struct Color
{
	std::uint8_t r = 0;
	std::uint8_t g = 0;
	std::uint8_t b = 0;
	std::uint8_t a = 255;

	Color() = default;
	
	friend std::ostream& operator<<(std::ostream& os, const Color& color)
	{
		os << "Color(" << color.r << "," << color.g << "," << color.b << "," << color.a << ")";
		return os;
	}

	template <typename U>
	Color(const U& v);
	
	template <typename U>
	operator U() const;
};

}
