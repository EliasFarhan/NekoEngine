#include "allocator/allocator.h"

#include <cassert>
#include <cstdlib>




namespace neko
{

    CustomAllocator::CustomAllocator(void* rootPtr, std::size_t size) : rootPtr_(rootPtr), size_(size)
    {
    }

    void CustomAllocator::Init(void* rootPtr, std::size_t size)
    {
        rootPtr_ = rootPtr;
        size_ = size;
    }

    std::size_t CustomAllocator::CalculateAlignForwardAdjustment(const void* address, std::size_t alignment)
    {
        //Check if alignement is power of 2
        assert((alignment & (alignment - 1)) == 0 && "Alignement needs to be a power of two");
        const std::size_t adjustment = alignment - (reinterpret_cast<std::uintptr_t>(address) & (alignment - 1));

        if (adjustment == alignment) return 0;
        return adjustment;
    }

    std::size_t CustomAllocator::CalculateAlignForwardAdjustmentWithHeader(const void* address, std::size_t alignment,
        std::size_t headerSize)
    {
        auto adjustment = CalculateAlignForwardAdjustment(address, alignment);
        std::size_t neededSpace = headerSize;
        if (adjustment < neededSpace)
        {
            neededSpace -= adjustment;
            adjustment += alignment * (neededSpace / alignment);
            if (neededSpace % alignment > 0) adjustment += alignment;
        }
        return adjustment;
    }

    void* CustomAllocator::AlignForward(void* address, std::size_t alignment)
    {
        return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(address) + CalculateAlignForwardAdjustment(address, alignment));

    }

    void* CustomAllocator::AlignForwardWithHeader(void* address, std::size_t alignment, std::size_t headerSize)
    {
        return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(address) + CalculateAlignForwardAdjustmentWithHeader(address, alignment, headerSize));
    }

    void* DumbAllocator::Allocate(std::size_t size, [[maybe_unused]] std::size_t alignement)
    {
        std::lock_guard lock(mutex_);
        return std::malloc(size);
    }

    void DumbAllocator::Deallocate(void* ptr)
    {
        std::lock_guard lock(mutex_);
        std::free(ptr);
    }
}
