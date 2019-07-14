#include <gb/box_collision.h>

bool checkCollision1(const Box* box1, const Box* box2)
{
	if (box2->x >= box1->x + box1->w) return false;
	if (box2->x + box2->w <= box1->x) return false;
	if (box2->y <= box1->y - box1->h) return false;
	if (box2->y - box2->h >= box1->y) return false;
	return true;
}

bool checkCollision2(const Box* box1, const Box* box2)
{
	return !((box2->x >= box1->x + box1->w) ||
		(box2->x + box2->w <= box1->x) ||
		(box2->y <= box1->y - box1->h) ||
		(box2->y - box2->h >= box1->y));
}

bool checkCollision3(const Box* box1, const Box* box2)
{
	const auto a = (abs((box1->x + (box1->w >> 1)) - (box2->x + (box2->w >> 1))) - ((box1->w >> 1) + (box2->w >> 1)));
	if (a == 0 || abs(a) != a)
		return false;
	const auto b = (abs((box1->y - (box1->h >> 1)) - (box2->y - (box2->h >> 1))) - ((box1->h >> 1) + (box2->h >> 1)));
	if (b == 0 || abs(b) != b)
		return false;
	return true;
}
