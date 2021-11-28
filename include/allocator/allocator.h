#pragma once

#include <cstddef>
#include <mutex>

namespace neko
{
    class AllocatorInterface
    {
    public:
        virtual ~AllocatorInterface() = default;
        virtual void* Allocate(std::size_t allocatedSize, std::size_t alignment) = 0;
        virtual void Deallocate(void* ptr) = 0;
    };

    class CustomAllocator : public AllocatorInterface
    {
    public:
        CustomAllocator() = default;
        CustomAllocator(void* rootPtr, std::size_t size);
        virtual void Init(void* rootPtr, std::size_t size);
    protected:
        static std::size_t CalculateAlignForwardAdjustment(const void* address, std::size_t alignment);
        static std::size_t CalculateAlignForwardAdjustmentWithHeader(const void* address, std::size_t alignment, std::size_t headerSize);
        static void* AlignForward(void* address, std::size_t alignment);
        static void* AlignForwardWithHeader(void* address, std::size_t alignment, std::size_t headerSize);
        void* rootPtr_ = nullptr;
        std::size_t size_ = 0;
        std::size_t usedMemory_ = 0;
        std::size_t numAllocations_ = 0;
    };

    class DumbAllocator : public AllocatorInterface
    {
    public:
        void* Allocate(std::size_t allocatedSize, std::size_t alignment) override;
        void Deallocate(void* ptr) override;
    private:
        mutable std::mutex mutex_;
    };

    /**
     * \brief Custom proxy allocator respecting allocator_traits
     */
    template<typename T>
    class StandardAllocator
    {
    public:
        typedef T value_type;
        StandardAllocator(AllocatorInterface& allocator);
        template <class U>
        StandardAllocator(const StandardAllocator<U>& allocator) noexcept : allocator_(allocator.GetAllocator()) {}
        T* allocate(std::size_t n);
        void deallocate(T* ptr, std::size_t n);
        [[nodiscard]] AllocatorInterface& GetAllocator() const { return allocator_; }
    protected:
        AllocatorInterface& allocator_;
    };


    template <class T, class U>
    constexpr bool operator== (const StandardAllocator<T>&, const StandardAllocator<U>&) noexcept
    {
        return true;
    }

    template <class T, class U>
    constexpr bool operator!= (const StandardAllocator<T>&, const StandardAllocator<U>&) noexcept
    {
        return false;
    }

    template <typename T>
    StandardAllocator<T>::StandardAllocator(AllocatorInterface& allocator) : allocator_(allocator)
    {
    }

    template <typename T>
    T* StandardAllocator<T>::allocate(std::size_t n)
    {
        return static_cast<T*>(allocator_.Allocate(n * sizeof(T), alignof(T)));
    }

    template <typename T>
    void StandardAllocator<T>::deallocate(T* ptr, std::size_t n)
    {
        allocator_.Deallocate(ptr);
    }
}
