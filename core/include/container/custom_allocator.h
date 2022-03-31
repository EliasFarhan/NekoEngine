#pragma once

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

#include <memory>
#include "engine/assert.h"
#include <vector>

#include "engine/log.h"

#include <fmt/core.h>
namespace neko
{

//#define NEKO_ALLOCATOR_LOG

class Allocator
{
public:

    Allocator() = default;
    Allocator(std::size_t size, void* start);
    Allocator(const Allocator&);
    Allocator(Allocator&&) = default;
    Allocator& operator=(const Allocator&) = delete;
    Allocator& operator=(Allocator&&) = default;

    virtual ~Allocator() noexcept;

    virtual void* Allocate(std::size_t allocatedSize, std::size_t alignment) = 0;

    virtual void Deallocate(void* p) = 0;

    inline static std::uintptr_t CalculateAlignForwardAdjustment(void* address, std::size_t alignment)
    {
        //Check if alignement is power of 2
        neko_assert((alignment & (alignment - 1)) == 0, "Alignement needs to be a power of two");
        const auto adjustment = alignment - (reinterpret_cast<std::uintptr_t>(address) & (alignment - 1));

        if (adjustment == alignment) return 0;
        return adjustment;
    }

    inline static std::uintptr_t CalculateAlignForwardAdjustmentWithHeader(void* address, std::size_t alignment, std::size_t headerSize)
    {
        auto adjustment = CalculateAlignForwardAdjustment(address, alignment);
        auto neededSpace = headerSize;
        if (adjustment < neededSpace)
        {
            neededSpace -= adjustment;
            adjustment += alignment * (neededSpace / alignment);
            if (neededSpace % alignment > 0) adjustment += alignment;
        }
        return adjustment;
    }

    inline static void* AlignForward(void* address, std::uintptr_t alignment)
    {
        return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(address) + CalculateAlignForwardAdjustment(address, alignment));
    }

    inline static void* AlignForwardWithHeader(void* address, std::uintptr_t alignment, std::uintptr_t headerSize)
    {
        return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(address) +
                                       CalculateAlignForwardAdjustmentWithHeader(address, alignment, headerSize));
    }

    [[nodiscard]] std::size_t GetUsedMemory() const
    {
        return usedMemory_;
    }

    [[nodiscard]] void* GetStart() const
    {
        return start_;
    }

    [[nodiscard]] std::size_t GetSize() const
    {
        return size_;
    }

    virtual void Destroy();


protected:
    void* start_ = nullptr;
    std::size_t size_ = 0;
    std::size_t usedMemory_ = 0;
    std::size_t numAllocations_ = 0;
};

class DumbAllocator final : public Allocator
{
public:
    void* Allocate(std::size_t allocatedSize, std::size_t alignment) override;
    void Deallocate(void* p) override;
};

class LinearAllocator final : public Allocator
{
public:
    LinearAllocator(std::size_t size, void* start) : Allocator(size, start), currentPos_(start_)
    {
        neko_assert(size > 0, "Linear Allocator cannot be empty");
    }

    ~LinearAllocator() override
    {
        currentPos_ = nullptr;
    }

    LinearAllocator(const LinearAllocator&) = delete;

    LinearAllocator& operator=(const LinearAllocator&) = delete;

    void* Allocate(std::size_t allocatedSize, std::size_t alignment) override;

    void Deallocate(void* p) override;

    void Clear();

protected:
    void* currentPos_ = nullptr;
};

class StackAllocator final : public Allocator
{
public:
    StackAllocator(std::size_t size, void* start) : Allocator(size, start), currentPos_(start)
    {
        neko_assert(size > 0, "Stack Allocator cannot be empty");
    }

    ~StackAllocator() override
    {
        currentPos_ = nullptr;
#if defined(NEKO_ASSERT)
        prevPos_ = nullptr;
#endif
    }

    StackAllocator(const StackAllocator&) = delete;

    StackAllocator& operator=(const StackAllocator&) = delete;

    void* Allocate(std::size_t allocatedSize, std::size_t alignment) override;

    void Deallocate(void* p) override;


    struct AllocationHeader
    {
#if defined(NEKO_ASSERT)
        void* prevPos = nullptr;
#endif
        std::uint8_t adjustment = 0;
    };

protected:
    void* currentPos_ = nullptr;
#if defined(NEKO_ASSERT)
    void* prevPos_ = nullptr;
#endif

};

class FreeListAllocator final : public Allocator
{
public:
    FreeListAllocator(std::size_t size, void* start) : Allocator(size, start), freeBlocks_((FreeBlock*)start)
    {
        neko_assert(size > sizeof(FreeBlock), "Free List Allocator cannot be empty");
        freeBlocks_->size = size;
        freeBlocks_->next = nullptr;
    }

    ~FreeListAllocator() override;

    FreeListAllocator(const FreeListAllocator&) = delete;

    FreeListAllocator& operator=(const FreeListAllocator&) = delete;

    void* Allocate(std::size_t allocatedSize, std::size_t alignment) override;

    void Deallocate(void* p) override;

protected:
    struct AllocationHeader
    {
        std::size_t size = 0;
        std::uint8_t adjustment = 0;
    };
    struct FreeBlock
    {
        std::size_t size = 0;
        FreeBlock* next = nullptr;
    };

    FreeBlock* freeBlocks_ = nullptr;
};

template<typename T>
class PoolAllocator final : public Allocator
{
    static_assert(sizeof(T) >= sizeof(void*));
public:
    PoolAllocator(std::size_t size, void* mem);

    ~PoolAllocator() override
    {
        freeBlocks_ = nullptr;
    }

    void* Allocate(std::size_t allocatedSize, std::size_t alignment) override;

    void Deallocate(void* p) override;

protected:
    struct FreeBlock
    {
        FreeBlock* next = nullptr;
    };
    FreeBlock* freeBlocks_ = nullptr;
};

template<typename T>
PoolAllocator<T>::PoolAllocator(std::size_t size, void* mem) : Allocator(size, mem)
{
    const auto adjustment = CalculateAlignForwardAdjustment(mem, alignof(T));
    freeBlocks_ = reinterpret_cast<FreeBlock*>(reinterpret_cast<std::uintptr_t>(mem) + adjustment);
    FreeBlock* freeBlock = freeBlocks_;
    const std::size_t numObjects = (size - adjustment) / sizeof(T);
    for (std::size_t i = 0; i < numObjects - 1; i++)
    {
        freeBlock->next = reinterpret_cast<FreeBlock*>(reinterpret_cast<std::uintptr_t>(freeBlock) + sizeof(T));
        freeBlock = freeBlock->next;
    }
    freeBlock->next = nullptr;
}

template<typename T>
void* PoolAllocator<T>::Allocate(std::size_t allocatedSize, [[maybe_unused]] std::size_t alignment)
{
    neko_assert(allocatedSize == sizeof(T) && alignment == alignof(T), "Pool Allocator can only allocate one Object pooled at once");
    if (freeBlocks_ == nullptr)
    {
        neko_assert(false, "Pool Allocator is full");
    }
    void* p = freeBlocks_;
    freeBlocks_ = freeBlocks_->next;
    usedMemory_ += allocatedSize;
    numAllocations_++;
    return p;
}

template<typename T>
void PoolAllocator<T>::Deallocate(void* p)
{
    auto* freeBlock = static_cast<FreeBlock*>(p);
    freeBlock->next = freeBlocks_;
    freeBlocks_ = freeBlock;
    usedMemory_ -= sizeof(T);
    numAllocations_--;
}


class ProxyAllocator : public Allocator
{
public:
    ProxyAllocator() = delete;
    explicit ProxyAllocator(Allocator& allocator);
    ProxyAllocator(const ProxyAllocator&);
    ProxyAllocator& operator=(const ProxyAllocator&) = delete;
    ~ProxyAllocator() override;

    void* Allocate(std::size_t allocatedSize, std::size_t alignment) override;

    void Deallocate(void* p) override;

protected:
    Allocator& allocator_;
};

template <typename  T>
class StandardLibraryAllocator : public ProxyAllocator
{
public:
    explicit StandardLibraryAllocator(Allocator& allocator);
    StandardLibraryAllocator(const StandardLibraryAllocator&);
    template <class U>
    explicit StandardLibraryAllocator(const StandardLibraryAllocator<U>&) noexcept;

    StandardLibraryAllocator& operator=(StandardLibraryAllocator&) = delete;
    StandardLibraryAllocator(StandardLibraryAllocator&&) noexcept;
    StandardLibraryAllocator& operator=(StandardLibraryAllocator&&) = delete;
    ~StandardLibraryAllocator();

    using value_type = T;
    using size_type = std::size_t;
    using pointer = T*;
    using const_pointer = const T*;


    [[nodiscard]] pointer allocate(size_type n)
    {
#ifdef NEKO_ALLOCATOR_LOG
        logDebug(fmt::format("[SL Allocator] Allocate {} objects", n));
#endif
        return static_cast<T*>(Allocate(sizeof(T) * n, alignof(T)));
    }
    void deallocate(pointer p, size_type n)
    {
        if (p == nullptr || n == 0)
            return;
#ifdef NEKO_ALLOCATOR_LOG
        logDebug(fmt::format("[SL Allocator] Deallocate {}B with pointer {}", n, reinterpret_cast<std::uintptr_t>(p)));
#endif
        Deallocate(p);
    }
private:
    template <typename U>
    friend class StandardLibraryAllocator;

};

template< typename T, typename U >
bool operator==(const StandardLibraryAllocator<T>& a,
                const StandardLibraryAllocator<U>& b)
{
    return &a.allocator_ == &b.allocator;
}

template <class T, class U>
bool operator!=(const StandardLibraryAllocator<T>& a, const StandardLibraryAllocator<U>& b)
{
    return &a.allocator_ != &b.allocator;
}


template <typename T>
StandardLibraryAllocator<T>::StandardLibraryAllocator(Allocator& allocator) :
        ProxyAllocator(allocator)
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[SL Allocator] Construct with {}B memory with pointer {}", size_,
		reinterpret_cast<std::uintptr_t>(this)));
#endif

}

template <typename T>
StandardLibraryAllocator<T>::StandardLibraryAllocator(const StandardLibraryAllocator& allocator) :
        ProxyAllocator(allocator.allocator_)
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[SL Allocator] Copy construct with {}B memory with pointer {}", size_,
		reinterpret_cast<std::uintptr_t>(this)));
#endif

}

template <typename T>
StandardLibraryAllocator<T>::StandardLibraryAllocator(StandardLibraryAllocator&& allocator) noexcept :
        ProxyAllocator(allocator.allocator_)
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[SL Allocator] Move construct with {}B memory with pointer {}", size_,
		reinterpret_cast<std::uintptr_t>(this)));
#endif

}

template <typename T>
template <class U>
StandardLibraryAllocator<T>::StandardLibraryAllocator(const StandardLibraryAllocator<U>& allocator) noexcept :
        ProxyAllocator(allocator.allocator_)
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[SL Allocator] Template Copy construct with {}B memory with pointer {}", size_,
		reinterpret_cast<std::uintptr_t>(this)));
#endif

}

template <typename T>
StandardLibraryAllocator<T>::~StandardLibraryAllocator()
{
#ifdef NEKO_ALLOCATOR_LOG
    logDebug(fmt::format("[SL Allocator] Destruction with {}B used memory {} allocations with pointer {}", usedMemory_,
		numAllocations_, reinterpret_cast<std::uintptr_t>(this)));
#endif

}
}
