#include "allocator/linear_allocator.h"
#include <cassert>

namespace neko
{
void* LinearAllocator::Allocate(std::size_t allocatedSize, std::size_t alignment)
{
    std::lock_guard lock(mutex_);
    assert(allocatedSize != 0 && "Linear Allocator cannot allocated nothing");
    const auto adjustment = CalculateAlignForwardAdjustment(currentPos_, alignment);

    assert(usedMemory_ + adjustment + allocatedSize < size_ && "Linear Allocator has not enough space for this allocation");

    auto* alignedAddress = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(currentPos_) + adjustment);
    currentPos_ = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(alignedAddress) + allocatedSize);
    usedMemory_ += allocatedSize + adjustment;
    numAllocations_++;
    return alignedAddress;
}

void LinearAllocator::Deallocate(void* ptr)
{
}

void LinearAllocator::Clear()
{
    std::lock_guard lock(mutex_);
    numAllocations_ = 0;
    usedMemory_ = 0;
    currentPos_ = rootPtr_;
}

void LinearAllocator::Init(void* rootPtr, std::size_t size)
{
    CustomAllocator::Init(rootPtr, size);
    currentPos_ = rootPtr;
}
}
