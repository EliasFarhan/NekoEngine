#include <gb/box_collision.h>
#include <gtest/gtest.h>

#define BOX_LENGTH 12
Box boxes[BOX_LENGTH] =
{
	{2,2,3,3},
	{0,0,3,3},
	{3,0,1,3},
	{4,0,3,3},
	{0,3,3,1},
	{4,3,3,1},
	{0,4,3,3},
	{3,4,1,3},
	{4,4,3,3},
	{0,3,7,1},
	{3,0,1,7},
	{0,0,2,2}
};

TEST(GameBoy, BoxCollisions)
{
	for(int i = 1; i < BOX_LENGTH-1;i++)
	{
		EXPECT_TRUE(checkCollision1(&boxes[0], &boxes[i]));
		EXPECT_TRUE(checkCollision2(&boxes[0], &boxes[i]));
		EXPECT_TRUE(checkCollision3(&boxes[0], &boxes[i]));
	}
	EXPECT_FALSE(checkCollision1(&boxes[0], &boxes[BOX_LENGTH - 1]));
	EXPECT_FALSE(checkCollision2(&boxes[0], &boxes[BOX_LENGTH - 1]));
	EXPECT_FALSE(checkCollision2(&boxes[0], &boxes[BOX_LENGTH - 1]));
}