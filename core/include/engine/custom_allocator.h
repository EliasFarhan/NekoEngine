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

#include <fmt/format.h>
namespace neko
{

//#define NEKO_ALLOCATOR_LOG

class Allocator
{
public:

    Allocator() = delete;
    Allocator(size_t size, void* start);
    Allocator(const Allocator&);
    Allocator(Allocator&&) = default;
    Allocator& operator=(const Allocator&) = delete;
    Allocator& operator=(Allocator&&) = default;

    virtual ~Allocator() noexcept;

    virtual void* Allocate(size_t allocatedSize, size_t alignment) = 0;

    virtual void Deallocate(void* p) = 0;

    inline static size_t CalculateAlignForwardAdjustment(const void* address, size_t alignment)
    {
        //Check if alignement is power of 2
        neko_assert((alignment & (alignment - 1)) == 0, "Alignement needs to be a power of two");
        const size_t adjustment = alignment - ((std::size_t) const_cast<void*>(address) & ((alignment - 1)));

        if (adjustment == alignment) return 0;
        return adjustment;
    }

    inline static size_t
    CalculateAlignForwardAdjustmentWithHeader(const void* address, size_t alignment, size_t headerSize)
    {
        auto adjustment = CalculateAlignForwardAdjustment(address, alignment);
        size_t neededSpace = headerSize;
        if (adjustment < neededSpace)
        {
            neededSpace -= adjustment;
            adjustment += alignment * (neededSpace / alignment);
            if (neededSpace % alignment > 0) adjustment += alignment;
        }
        return adjustment;
    }

    inline static void* AlignForward(void* address, size_t alignment)
    {
        return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(address) + CalculateAlignForwardAdjustment(address, alignment));
    }

    inline static void* AlignForwardWithHeader(void* address, size_t alignment, size_t headerSize)
    {
        return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(address) +
                                       CalculateAlignForwardAdjustmentWithHeader(address, alignment, headerSize));
    }

    [[nodiscard]] size_t GetUsedMemory() const
    {
        return usedMemory_;
    }

    [[nodiscard]] void* GetStart() const
    {
        return start_;
    }

    [[nodiscard]] size_t GetSize() const
    {
        return size_;
    }

    virtual void Destroy();


protected:
    void* start_ = nullptr;
    size_t size_ = 0;
    size_t usedMemory_ = 0;
    size_t numAllocations_ = 0;
};

class LinearAllocator : public Allocator
{
public:
    LinearAllocator(size_t size, void* start) : Allocator(size, start), currentPos_(start_)
    {
        neko_assert(size > 0, "Linear Allocator cannot be empty");
    }

    ~LinearAllocator() override
    {
        currentPos_ = nullptr;
    }

    LinearAllocator(const LinearAllocator&) = delete;

    LinearAllocator& operator=(const LinearAllocator&) = delete;

    void* Allocate(size_t allocatedSize, size_t alignment) override;

    void Deallocate(void* p) override;

    void Clear();

protected:
    void* currentPos_ = nullptr;
};

class StackAllocator : public Allocator
{
public:
    StackAllocator(size_t size, void* start) : Allocator(size, start), currentPos_(start)
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

    void* Allocate(size_t allocatedSize, size_t alignment) override;

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

class FreeListAllocator : public Allocator
{
public:
    FreeListAllocator(size_t size, void* start) : Allocator(size, start), freeBlocks_((FreeBlock*)start)
    {
        neko_assert(size > sizeof(FreeBlock), "Free List Allocator cannot be empty");
        freeBlocks_->size = size;
        freeBlocks_->next = nullptr;
    }

    ~FreeListAllocator() override;

    FreeListAllocator(const FreeListAllocator&) = delete;

    FreeListAllocator& operator=(const FreeListAllocator&) = delete;

    void* Allocate(size_t allocatedSize, size_t alignment) override;

    void Deallocate(void* p) override;

protected:
    struct AllocationHeader
    {
        size_t size = 0;
        std::uint8_t adjustment = 0;
    };
    struct FreeBlock
    {
        size_t size = 0;
        FreeBlock* next = nullptr;
    };

    FreeBlock* freeBlocks_ = nullptr;
};

template<typename T>
class PoolAllocator : public Allocator
{
    static_assert(sizeof(T) >= sizeof(void*));
public:
    PoolAllocator(size_t size, void* mem);

    ~PoolAllocator() override
    {
        freeBlocks_ = nullptr;
    }

    void* Allocate(size_t allocatedSize, size_t alignment) override;

    void Deallocate(void* p) override;

protected:
    struct FreeBlock
    {
        FreeBlock* next = nullptr;
    };
    FreeBlock* freeBlocks_ = nullptr;
};

template<typename T>
PoolAllocator<T>::PoolAllocator(size_t size, void* mem) : Allocator(size, mem)
{
    const auto adjustment = CalculateAlignForwardAdjustment(mem, alignof(T));
    freeBlocks_ = reinterpret_cast<FreeBlock*>(reinterpret_cast<std::uintptr_t>(mem) + adjustment);
    FreeBlock* freeBlock = freeBlocks_;
    const size_t numObjects = (size - adjustment) / sizeof(T);
    for (size_t i = 0; i < numObjects - 1; i++)
    {
        freeBlock->next = reinterpret_cast<FreeBlock*>(reinterpret_cast<std::uintptr_t>(freeBlock) + sizeof(T));
        freeBlock = freeBlock->next;
    }
    freeBlock->next = nullptr;
}

template<typename T>
void* PoolAllocator<T>::Allocate(size_t allocatedSize, [[maybe_unused]] size_t alignment)
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

    void* Allocate(size_t allocatedSize, size_t alignment) override;

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
    using size_type = size_t;
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
