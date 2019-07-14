#pragma once
#include <cstdint>
#include <cmath>

struct Box
{
	std::uint8_t x = 0;
	std::uint8_t y = 0;
	std::uint8_t w = 0;
	std::uint8_t h = 0;
};


bool checkCollision1(const Box* box1, const Box* box2);

bool checkCollision2(const Box* box1, const Box* box2);

bool checkCollision3(const Box* box1, const Box* box2);


