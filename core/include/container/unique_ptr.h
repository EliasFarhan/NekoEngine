#pragma once

#include "container/custom_allocator.h"

namespace neko::tl
{

template<typename T>
class UniquePtr
{
public:
    UniquePtr() : allocator_(defaultAllocator_) { }
    explicit UniquePtr(T* ptr, Allocator& allocator = defaultAllocator_): ptr_(ptr), allocator_(allocator){}

    ~UniquePtr()
    {
        if(ptr_ != nullptr)
        {
            allocator_.Deallocate(ptr_);
        }
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_), allocator_(other.allocator_)
    {
        other.ptr_ = nullptr;
    }
    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        return *this;
    }
    [[nodiscard]] T* Get() noexcept { return ptr_; }

    /**
     * \brief Replaces the managed object
     */
    void Reset(T* newPtr)
    {
        auto* oldPtr = ptr_;
        ptr_ = newPtr;
        allocator_.Deallocate(oldPtr);
    }
    /**
     * \brief Swaps the managed objects
     */
    void Swap(UniquePtr<T>& other) noexcept
    {
        auto* tmpPtr = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = tmpPtr;
    }
    /**
     * \brief Checks if there is an associated managed object
     */
    explicit operator bool() const noexcept
    {
        return ptr_ != nullptr;
    }
    /**
     * \brief Indirection operator overload
     */
    T& operator*()
    {
        return *ptr_;
    }
    /**
     * \brief Member of a pointer operator overload
     */
    T* operator->()
    {
        return ptr_;
    }
private:
    T* ptr_ = nullptr;
    Allocator& allocator_;
    inline static DumbAllocator defaultAllocator_;
};

template<typename T>
class UniquePtrFactory
{
public:
    explicit UniquePtrFactory(Allocator& allocator = defaultAllocator_) : allocator_(allocator){}

    template<typename ...Args>
    UniquePtr<T> MakeUnique(Args ... args)
    {
        auto* ptr = static_cast<T*>(allocator_.Allocate(sizeof (T), alignof(T)));
        ::new (ptr) T(args...);
        return UniquePtr<T>(ptr);
    }
private:
    Allocator& allocator_;
    inline static DumbAllocator defaultAllocator_;
};

}