#include "container/array_list.h"
#include "gtest/gtest.h"

TEST(ArrayList, InitializerListTrivial)
{
    const auto args = { 1, 2, 3, 4, 5 };
    const neko::tl::ArrayList numbers = args;
    EXPECT_EQ(numbers.Size(), 5);
    EXPECT_EQ(numbers.Capacity(), 5);
    for (std::size_t i = 0; i < args.size(); i++)
    {
        EXPECT_EQ(numbers[i], args.begin()[i]);
    }
}

TEST(ArrayList, TrivialDefaultValue)
{
    const neko::tl::ArrayList<int> numbers(10);
    EXPECT_EQ(numbers.Size(), 10);
    EXPECT_EQ(numbers.Capacity(), 10);
    EXPECT_EQ(numbers.Front(), 0);
}

TEST(ArrayList, ReserveMore)
{
    neko::tl::ArrayList numbers = { 1, 2, 3, 4, 5 };
    EXPECT_EQ(numbers.Size(), 5);
    EXPECT_EQ(numbers.Capacity(), 5);
    numbers.Reserve(10);
    EXPECT_EQ(numbers.Size(), 5);
    EXPECT_EQ(numbers.Capacity(), 10);
}

TEST(ArrayList, ReserveLess)
{
    neko::tl::ArrayList numbers = { 1, 2, 3, 4, 5 };
    EXPECT_EQ(numbers.Size(), 5);
    EXPECT_EQ(numbers.Capacity(), 5);
    numbers.Reserve(4);
    //We do not change the capacity or size if smaller
    EXPECT_EQ(numbers.Size(), 5);
    EXPECT_EQ(numbers.Capacity(), 5);
}


TEST(ArrayList, ResizeMore)
{
    const auto args = { 1, 2, 3, 4, 5 };
    neko::tl::ArrayList numbers = args;
    EXPECT_EQ(numbers.Size(), 5);
    EXPECT_EQ(numbers.Capacity(), 5);
    numbers.Resize(10);
    EXPECT_EQ(numbers.Size(), 10);
    EXPECT_EQ(numbers.Capacity(), 10);
    for(std::size_t i = 0; i < args.size(); i++)
    {
        EXPECT_EQ(numbers[i],args.begin()[i]);
    }

}

TEST(ArrayList, ResizeLess)
{
    const auto args = { 1, 2, 3, 4, 5, 6 };
    neko::tl::ArrayList numbers = args;
    EXPECT_EQ(numbers.Size(), 6);
    EXPECT_EQ(numbers.Capacity(), 6);
    numbers.Resize(5);
    EXPECT_EQ(numbers.Size(), 5);
    EXPECT_EQ(numbers.Capacity(), 6);
}


TEST(ArrayList, EraseOne)
{
    const auto args = { 1, 2, 3, 4, 5 };
    neko::tl::ArrayList numbers = args;
    EXPECT_EQ(numbers.Size(), 5);
    EXPECT_EQ(numbers.Capacity(), 5);
    numbers.Erase(numbers.cbegin());
    EXPECT_EQ(numbers.Size(), 4);
    EXPECT_EQ(numbers.Capacity(), 5);
    EXPECT_EQ(numbers.Front(), 2);
}

TEST(ArrayList, EraseTwo)
{
    const auto args = { 1, 2, 3, 4, 5 };
    neko::tl::ArrayList numbers = args;
    EXPECT_EQ(numbers.Size(), 5);
    EXPECT_EQ(numbers.Capacity(), 5);
    numbers.Erase(numbers.cbegin(), numbers.cbegin()+2);
    EXPECT_EQ(numbers.Size(), 3);
    EXPECT_EQ(numbers.Capacity(), 5);
    EXPECT_EQ(numbers.Front(), 3);
}

TEST(ArrayList, EraseAll)
{
    const auto args = { 1, 2, 3, 4, 5 };
    neko::tl::ArrayList numbers = args;
    EXPECT_EQ(numbers.Size(), 5);
    EXPECT_EQ(numbers.Capacity(), 5);
    numbers.Erase(numbers.cbegin(), numbers.cend());
    EXPECT_EQ(numbers.Size(), 0);
    EXPECT_EQ(numbers.Capacity(), 5);
}

class MovableObject
{
public:
    MovableObject() = default;
    ~MovableObject() = default;
    MovableObject(const MovableObject&) = delete;
    MovableObject& operator=(const MovableObject&) = delete;
    MovableObject(MovableObject&& other) noexcept : countMoveConstruct(1 + other.countMoveConstruct)
    {
    }
    MovableObject& operator=(MovableObject&& other) noexcept
    {
        countMoveAssignment = other.countMoveAssignment + 1;
        countMoveConstruct = other.countMoveConstruct;
        return *this;
    }

    [[nodiscard]] int GetCountMoveConstruct() const { return countMoveConstruct; }
    [[nodiscard]] int GetCountMoveAssignment() const { return countMoveAssignment; }
private:
    int countMoveConstruct = 0;
    int countMoveAssignment = 0;

};


TEST(ArrayList, MovableOnlyObject)
{
    neko::tl::ArrayList<MovableObject> movable(10);
    EXPECT_EQ(movable.Size(), 10);
    EXPECT_EQ(movable.Capacity(), 10);
    auto newMovable(std::move(movable));
    EXPECT_EQ(movable.Capacity(), 0);
    EXPECT_EQ(movable.Size(), 0);
    //no move, only the pointer got moved
    EXPECT_EQ(newMovable.Front().GetCountMoveAssignment(), 0);
    EXPECT_EQ(newMovable.Front().GetCountMoveConstruct(), 0);
    EXPECT_EQ(newMovable.Capacity(), 10);
    EXPECT_EQ(newMovable.Size(), 10);
    newMovable.Reserve(20);
    //When reserving the movable object should have moved
    EXPECT_EQ(newMovable.Front().GetCountMoveAssignment(), 1);
    EXPECT_EQ(newMovable.Front().GetCountMoveConstruct(), 0);
    MovableObject m;
    newMovable.PushBack(std::move(m));
    EXPECT_EQ(newMovable.Size(), 11);
}
