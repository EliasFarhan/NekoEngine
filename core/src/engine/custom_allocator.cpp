/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "engine/custom_allocator.h"


namespace neko
{
Allocator::Allocator(size_t size, void* start) : start_(start), size_(size)
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[Allocator] Construct with {}B memory and start pointer {} with pointer {}",
		size_,
		reinterpret_cast<std::uintptr_t>(start),
		reinterpret_cast<std::uintptr_t>(this)));
#endif
}

Allocator::Allocator(const Allocator& allocator)
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[Allocator] Copy construct with {}B memory from pointer {} with pointer {}", size_,
		reinterpret_cast<std::uintptr_t>(&allocator), reinterpret_cast<std::uintptr_t>(this)));
#endif
    start_ = allocator.start_;
    size_ = allocator.size_;
    numAllocations_ = allocator.numAllocations_;
    usedMemory_ = allocator.usedMemory_;
}

Allocator::~Allocator() noexcept
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[Allocator] Destructing with {} allocations and {}B memory, with pointer {}",
		numAllocations_, usedMemory_, reinterpret_cast<std::uintptr_t>(this)));
#endif
    //neko_assert(numAllocations_ == 0 && usedMemory_ == 0, "Allocator should be emptied before destructor");
}

void Allocator::Destroy()
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("Destroying Allocator with {} allocations and {}B memory, with pointer {}",
		numAllocations_, usedMemory_, reinterpret_cast<std::uintptr_t>(this)));
#endif
    neko_assert(numAllocations_ == 0 && usedMemory_ == 0, "Allocator should be emptied before destroy");
    start_ = nullptr;
    size_ = 0;
}

void* LinearAllocator::Allocate(size_t allocatedSize, size_t alignment)
{
    neko_assert(allocatedSize != 0, "Linear Allocator cannot allocated nothing");
    const auto adjustment = CalculateAlignForwardAdjustment(currentPos_, alignment);

    neko_assert(usedMemory_ + adjustment + allocatedSize < size_, "Linear Allocator has not enough space for this allocation");

    auto* alignedAddress = (void*)((std::uint64_t) currentPos_ + adjustment);
    currentPos_ = (void*)((std::uint64_t) alignedAddress + allocatedSize);
    usedMemory_ += allocatedSize + adjustment;
    numAllocations_++;
    return alignedAddress;
}

void LinearAllocator::Deallocate([[maybe_unused]] void* p)
{
    neko_assert(false, "Use clear() instead");
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

    void* alignedAddress = reinterpret_cast<void*>(reinterpret_cast<std::uint64_t>(currentPos_) + adjustment);

    auto* header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<std::uint64_t>(alignedAddress) - sizeof(AllocationHeader));
    header->adjustment = static_cast<std::uint8_t>(adjustment);
#if defined(NEKO_ASSERT)
    header->prevPos = prevPos_;
    prevPos_ = alignedAddress;
#endif
    currentPos_ = reinterpret_cast<void*>(reinterpret_cast<std::uint64_t>(alignedAddress) + allocatedSize);
    usedMemory_ += allocatedSize + adjustment;
    numAllocations_++;
    return alignedAddress;

}


void StackAllocator::Deallocate(void* p)
{
    neko_assert(p != nullptr, "Stack allocator requires valid pointer to deallocate");
    neko_assert(p == prevPos_, "Stack allocator needs to deallocate from the top");
    //Access the AllocationHeader in the bytes before p
    auto* header = (AllocationHeader*)((std::uint64_t) p - sizeof(AllocationHeader));
#if defined(NEKO_ASSERT)
    prevPos_ = header->prevPos;
#endif
    usedMemory_ -= (std::uint64_t) currentPos_ - (std::uint64_t) p + header->adjustment;
    currentPos_ = (void*)((std::uint64_t) p - header->adjustment);
    numAllocations_--;
}


FreeListAllocator::~FreeListAllocator()
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[FreeList Allocator] Destruction with {}B usedMemory, {} allocations",usedMemory_, numAllocations_));
#endif
    freeBlocks_ = nullptr;
}

void* FreeListAllocator::Allocate(size_t allocatedSize, size_t alignment)
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[FreeList Allocator] Allocate {}B, with pointer {}", allocatedSize,
		reinterpret_cast<std::uintptr_t>(this)));
#endif
    FreeBlock* prevFreeBlock = nullptr;
    FreeBlock* freeBlock = freeBlocks_;
    while (freeBlock != nullptr)
    {
        const std::uintptr_t adjustment = CalculateAlignForwardAdjustmentWithHeader(freeBlock, alignment, sizeof(AllocationHeader));
        size_t totalSize = allocatedSize + adjustment;
        if (freeBlock->size < totalSize)
        {
            prevFreeBlock = freeBlock;
            freeBlock = freeBlock->next;
            continue;
        }
        static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

        //If allocation takes the whole freeblock
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
            FreeBlock* nextBlock = reinterpret_cast<FreeBlock*>(reinterpret_cast<std::uintptr_t>(freeBlock) + totalSize);
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
        void* alignedAddress = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(freeBlock) + adjustment);
        auto* header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<std::uint64_t>(alignedAddress) - sizeof(AllocationHeader));
        header->size = totalSize;
        header->adjustment = static_cast<std::uint8_t>(adjustment);
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
#ifdef NEKO_ALLOCATOR_LOG
    logDebug("[FreeList Allocator] Deallocate");
#endif
    neko_assert(p != nullptr, "Free List cannot deallocate nullptr");
    AllocationHeader* header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<std::uintptr_t>(p) - sizeof(AllocationHeader));
    void* blockStart = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(p) - header->adjustment);
    size_t blockSize = header->size;
    void* blockEnd = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(blockStart) + blockSize);
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
        prevFreeBlock = static_cast<FreeBlock*>(blockStart);
        prevFreeBlock->size = blockSize;
        prevFreeBlock->next = freeBlocks_;
        freeBlocks_ = prevFreeBlock;
    }
    else if (reinterpret_cast<void*>(reinterpret_cast<size_t>(prevFreeBlock) + prevFreeBlock->size) == blockStart)
    {
        prevFreeBlock->size += blockSize;
    }
    else
    {
        auto* temp = static_cast<FreeBlock*>(blockStart);
        temp->size = blockSize;
        temp->next = prevFreeBlock->next;
        prevFreeBlock->next = temp;
        prevFreeBlock = temp;
    }
    if (freeBlock != nullptr && static_cast<void*>(freeBlock) == blockEnd)
    {
        prevFreeBlock->size += freeBlock->size;
        prevFreeBlock->next = freeBlock->next;
    }
    numAllocations_--;
    usedMemory_ -= blockSize;
}


ProxyAllocator::ProxyAllocator(Allocator& allocator):
        Allocator(allocator.GetSize(), allocator.GetStart()), allocator_(allocator)
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[Proxy Allocator] Construct with {}B memory from pointer {} with pointer {}",
		size_,
		reinterpret_cast<std::uintptr_t>(&allocator),
		reinterpret_cast<std::uintptr_t>(this)));
#endif
}

ProxyAllocator::ProxyAllocator(const ProxyAllocator& allocator) :
        Allocator(allocator.GetSize(), allocator.GetStart()),
        allocator_(allocator.allocator_)
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[Proxy Allocator] Copy construct size: {}B, start: {} from pointer {} with pointer {}",
		size_,
		reinterpret_cast<std::uintptr_t>(start_),
		reinterpret_cast<std::uintptr_t>(&allocator),
		reinterpret_cast<std::uintptr_t>(this)));
#endif
}

ProxyAllocator::~ProxyAllocator()
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[Proxy Allocator] Destruction with allocations nmb: {} and used memory {}", numAllocations_, usedMemory_));
#endif
}

void* ProxyAllocator::Allocate(size_t allocatedSize, size_t alignment)
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[Proxy Allocator] Allocate {}B with pointer {}", allocatedSize, reinterpret_cast<std::uintptr_t>(this)));
#endif
    numAllocations_++;
    const size_t mem = allocator_.GetUsedMemory();
    void* p = allocator_.Allocate(allocatedSize, alignment);
    usedMemory_ += allocator_.GetUsedMemory() - mem;
    return p;
}

void ProxyAllocator::Deallocate(void* p)
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug("[Proxy Allocator] Deallocate");
#endif
    if (p == nullptr)
        return;
    if (numAllocations_ != 0)
    {
        numAllocations_--;
    }
    const size_t mem = allocator_.GetUsedMemory();
    allocator_.Deallocate(p);
    if (usedMemory_ != 0)
    {
        usedMemory_ -= mem - allocator_.GetUsedMemory();
    }
}
}
