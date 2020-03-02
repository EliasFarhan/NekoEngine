//
// Created by efarhan on 01.03.20.
//

#include "engine/custom_allocator.h"

namespace neko
{

void* LinearAllocator::Allocate(size_t allocatedSize, size_t alignment)
{
    neko_assert(allocatedSize != 0, "Linear Allocator cannot allocated nothing");
    const auto adjustment = CalculateAlignForwardAdjustment(currentPos_, alignment);

    neko_assert(usedMemory_ + adjustment + allocatedSize > size_, "Linear Allocator has not enough space for this allocation");

    auto* alignedAddress = (void*) ((std::uint64_t) currentPos_ + adjustment);
    currentPos_ = (void*) ((std::uint64_t) alignedAddress + allocatedSize);
    usedMemory_ += allocatedSize + adjustment;
    numAllocations_++;
    return alignedAddress;
}

void LinearAllocator::Deallocate([[maybe_unused]]void* p)
{
    neko_assert(false,"Use clear() instead");
}

void LinearAllocator::Clear()
{
    numAllocations_ = 0;
    usedMemory_ = 0;
    currentPos_ = start_;
}

void* StackAllocator::Allocate(size_t allocatedSize, size_t alignment)
{
    neko_assert(allocatedSize != 0, "Stack Allocator cannot allocate nothing");
    const auto adjustment = CalculateAlignForwardAdjustmentWithHeader(currentPos_, alignment, sizeof(AllocationHeader));
    if (usedMemory_ + adjustment + allocatedSize > size_)
    {
        neko_assert(false, "StackAllocator has not enough space for this allocation");
    }

    void* alignedAddress = (void*) ((std::uint64_t) currentPos_ + adjustment);

    auto* header = (AllocationHeader*) ((std::uint64_t) alignedAddress - sizeof(AllocationHeader));
    header->adjustment = adjustment;

    currentPos_ = (void*) ((std::uint64_t) alignedAddress + allocatedSize);
    usedMemory_ += allocatedSize + adjustment;
    numAllocations_++;
    return alignedAddress;

}


void StackAllocator::Deallocate(void* p)
{
    neko_assert(p == prevPos_, "Stack allocator needs to deallocate from the top");
    //Access the AllocationHeader in the bytes before p
    auto* header = (AllocationHeader*) ((std::uint64_t) p - sizeof(AllocationHeader));
    usedMemory_ -= (std::uint64_t) currentPos_ - (std::uint64_t) p + header->adjustment;
    currentPos_ = (void*) ((std::uint64_t) p - header->adjustment);
    numAllocations_--;
}

void* FreeListAllocator::Allocate(size_t allocatedSize, size_t alignment)
{
    FreeBlock* prevFreeBlock = nullptr;
    FreeBlock* freeBlock = freeBlocks_;
    while (freeBlock != nullptr)
    {
        auto adjustment = CalculateAlignForwardAdjustmentWithHeader(freeBlock, alignment, sizeof(AllocationHeader));
        size_t totalSize = allocatedSize + adjustment;
        if (freeBlock->size < totalSize)
        {
            prevFreeBlock = freeBlock;
            freeBlock = freeBlock->next;
            continue;
        }
        static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

        if (freeBlock->size - totalSize <= sizeof(AllocationHeader))
        {
            totalSize = freeBlock->size;
            if (prevFreeBlock != nullptr)
            {
                prevFreeBlock->next = freeBlock->next;
            }
            else
            {
                freeBlocks_ = freeBlock->next;
            }
        }
        else
        {
            FreeBlock* nextBlock = (FreeBlock*) ((std::uint64_t) freeBlock + totalSize);
            nextBlock->size = freeBlock->size - totalSize;
            nextBlock->next = freeBlock->next;

            if (prevFreeBlock != nullptr)
            {
                prevFreeBlock->next = nextBlock;
            }
            else
            {
                freeBlocks_ = nextBlock;
            }
        }
        void* alignedAddress = (void*) ((std::uint64_t) freeBlock + adjustment);
        AllocationHeader* header = (AllocationHeader*) ((std::uint64_t) alignedAddress - sizeof(AllocationHeader));
        header->size = totalSize;
        header->adjustment = adjustment;
        usedMemory_ += totalSize;
        numAllocations_++;
        neko_assert(CalculateAlignForwardAdjustment(alignedAddress, alignment) == 0, "Free List Allocator: New generated block is not aligned");
        return alignedAddress;
    }
    neko_assert(false, "FreeList Allocator has not enough space for this allocation");
    return nullptr;
}

void FreeListAllocator::Deallocate(void* p)
{
    neko_assert(p != nullptr, "Free List cannot deallocate nullptr");
    AllocationHeader* header = (AllocationHeader*) ((std::uint64_t) p - sizeof(AllocationHeader));
    void* blockStart = (void*) ((std::uint64_t) p - header->adjustment);
    size_t blockSize = header->size;
    void* blockEnd = (void*) ((std::uint64_t) blockStart + blockSize);
    FreeBlock* prevFreeBlock = nullptr;
    FreeBlock* freeBlock = freeBlocks_;

    while (freeBlock != nullptr)
    {
        if (freeBlock >= blockEnd) break;
        prevFreeBlock = freeBlock;
        freeBlock = freeBlock->next;
    }
    if (prevFreeBlock == nullptr)
    {
        prevFreeBlock = (FreeBlock*) blockStart;
        prevFreeBlock->size = blockSize;
        prevFreeBlock->next = freeBlocks_;
        freeBlocks_ = prevFreeBlock;
    }
    else if (prevFreeBlock + prevFreeBlock->size == blockStart)
    {
        prevFreeBlock->size += blockSize;
    }
    else
    {
        FreeBlock* temp = (FreeBlock*) blockStart;
        temp->size = blockSize;
        temp->next = prevFreeBlock->next;
        prevFreeBlock->next = temp;
        prevFreeBlock = temp;
    }
    if (freeBlock != nullptr && freeBlock == blockEnd)
    {
        prevFreeBlock->size += freeBlock->size;
        prevFreeBlock->next = freeBlock->next;
    }
    numAllocations_--;
    usedMemory_ -= blockSize;
}

void* ProxyAllocator::Allocate(size_t allocatedSize, size_t alignment)
{
    numAllocations_++;
    size_t mem = allocator_.GetUsedMemory();
    void* p = allocator_.Allocate(allocatedSize, alignment);
    usedMemory_ += allocator_.GetUsedMemory() - mem;
    return p;
}

void ProxyAllocator::Deallocate(void* p)
{
    numAllocations_--;
    size_t mem = allocator_.GetUsedMemory();
    allocator_.Deallocate(p);
    usedMemory_ -= mem - allocator_.GetUsedMemory();
}
}