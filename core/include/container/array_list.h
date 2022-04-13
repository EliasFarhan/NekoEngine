#pragma once

#include "container/custom_allocator.h"

#include <memory>
#include <functional>

namespace neko::tl
{

template<typename T>
class ArrayList
{
public:
    class iterator
    {
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;
        using self_type = iterator;

        iterator(T* ptr) : ptr_(ptr) {}
        self_type operator++() { self_type i = *this; ++ptr_; return i; }
        reference operator*() { return *ptr_; }
        pointer operator->() { return ptr_; }
        bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
    private:
        friend class const_iterator;
        T* ptr_ = nullptr;
    };

    class const_iterator
    {
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;
        using self_type = const_iterator;

        const_iterator(iterator it) : ptr_(it.ptr_){}
        const_iterator(T* ptr) : ptr_(ptr) {}
        self_type operator++() { self_type i = *this; ++ptr_; return i; }
        const reference operator*() { return *ptr_; }
        const pointer operator->() { return ptr_; }
        bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_; }
        auto operator+(std::size_t i) const { return self_type{ ptr_ + i }; }
        auto operator-(self_type it) const{ return  ptr_ - it.ptr_; }
    private:
        T* ptr_ = nullptr;
    };

    iterator begin() { return { beginPtr_ }; }
    iterator end() { return { endPtr_ }; }
    [[nodiscard]] const_iterator cbegin() const { return { beginPtr_ }; }
    [[nodiscard]] const_iterator cend() const { return { endPtr_ }; }

    ArrayList() : allocator_(defaultAllocator_)
    {
        
    }

    explicit ArrayList(std::size_t count, Allocator& allocator = defaultAllocator_) :
        beginPtr_(static_cast<T*>(allocator.Allocate(sizeof(T) * count, alignof(T)))),
        endPtr_(beginPtr_ + count),
        capacity_(endPtr_ - beginPtr_),
        allocator_(allocator)
    {
        for (auto* ptr = beginPtr_; ptr != endPtr_; ++ptr)
        {
            ::new(ptr) T();
        }
        
    }

    explicit ArrayList(std::size_t count, const T& value, Allocator& allocator = defaultAllocator_) :
            beginPtr_(static_cast<T*>(allocator.Allocate(sizeof(T) * count, alignof(T)))),
            endPtr_(beginPtr_ + count),
            capacity_(endPtr_ - beginPtr_),
            allocator_(allocator)
    {
        for (auto* ptr = beginPtr_; ptr != endPtr_; ++ptr)
        {
            *ptr = value;
        }

    }

    ArrayList(std::initializer_list<T> init, Allocator& allocator = defaultAllocator_) :
        beginPtr_(static_cast<T*>(allocator.Allocate(init.size() * sizeof(T), alignof(T)))), allocator_(allocator)
    {
        const auto newSize = init.size();
        auto initPtr = init.begin();
        for (std::size_t i = 0; i < newSize; i++)
        {
            beginPtr_[i] = initPtr[i];
        }
        endPtr_ = beginPtr_ + newSize;
        capacity_ = endPtr_ - beginPtr_;
    }

    ArrayList(const ArrayList& other) : allocator_(other.allocator_)
    {
        const auto otherSize = other.Size();
        beginPtr_ = static_cast<T*>(allocator_.Allocate(otherSize * sizeof(T), alignof(T)));
        endPtr_ = beginPtr_ + otherSize;
        capacity_ = otherSize;
        std::memcpy(beginPtr_, other.beginPtr_, otherSize * sizeof(T));
    }

    ArrayList& operator=(const ArrayList& other)
    {
        if (this == &other)
        {
            return *this;
        }
        const auto otherSize = other.Size();
        if (beginPtr_ != nullptr)
        {
            const auto beginIt = begin();
            const auto endIt = end();
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                for (auto it = beginPtr_; it != endPtr_; ++it)
                {
                    it->~T();
                }
            }
            if(capacity_ < otherSize)
            {
                allocator_.Deallocate(beginPtr_);
                beginPtr_ = allocator_.Allocate(otherSize * sizeof(T), alignof(T));
            }
        }
        else
        {
            beginPtr_ = allocator_.Allocate(otherSize * sizeof(T), alignof(T));
        }
        endPtr_ = beginPtr_ + otherSize;
        capacity_ = otherSize;
        std::memcpy(beginPtr_, other.beginPtr_, sizeof(T) * otherSize);
        return *this;
    }

    ArrayList(ArrayList&& other) noexcept : beginPtr_(other.beginPtr_), endPtr_(other.endPtr_),
                                            capacity_(other.capacity_), allocator_(other.allocator_)
    {
        other.beginPtr_ = nullptr;
        other.endPtr_ = nullptr;
        other.capacity_ = 0;
    }

    ArrayList& operator=(ArrayList&& other) noexcept
    {
        if(beginPtr_ != nullptr)
        {
            ~ArrayList();
        }
        beginPtr_ = other.beginPtr_;
        endPtr_ = other.endPtr_;
        capacity_ = other.capacity_;

        return *this;
    }

    ~ArrayList()
    {
        if (beginPtr_ == nullptr)
        {
            return;
        }
        const auto beginIt = begin();
        const auto endIt = end();
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            for (auto it = beginPtr_; it != endPtr_; ++it)
            {
                it->~T();
            }
        }
        allocator_.Deallocate(beginPtr_);
        beginPtr_ = nullptr;
        endPtr_ = nullptr;
        capacity_ = 0;
        
    }

    [[nodiscard]] std::size_t Size() const { return std::distance(beginPtr_, endPtr_); }
    [[nodiscard]] std::size_t Capacity() const { return capacity_; }
    void Reserve(std::size_t newCapacity)
    {
        if (newCapacity < capacity_)
            return;
        const auto oldSize = Size();
        //TODO implement using realloc as well
        T* newBeginPtr = static_cast<T*>(allocator_.Allocate(sizeof(T) * newCapacity, alignof(T)));
        std::memset(newBeginPtr, 0, sizeof(T) * newCapacity);
        for(std::size_t i = 0; i < oldSize; i++)
        {
            newBeginPtr[i] = std::move(beginPtr_[i]);
        }
        allocator_.Deallocate(beginPtr_);
        beginPtr_ = newBeginPtr;
        endPtr_ = beginPtr_ + oldSize;
        capacity_ = newCapacity;
    }

    void Resize(std::size_t newSize)
    {
        const auto oldSize = Size();
        if(newSize < oldSize)
        {
            if constexpr (!std::is_trivially_constructible_v<T>)
            {
                for (auto it = beginPtr_ + newSize; it != endPtr_; ++it)
                {
                    it->~T();
                }
            }
        }
        if(newSize > capacity_)
        {
            Reserve(newSize);
        }
        for(std::size_t i = oldSize; i < newSize; ++i)
        {
            T* ptr = &beginPtr_[i];
            ::new(ptr) T();
        }
        endPtr_ = beginPtr_ + newSize;
    }

    void PushBack(const T& val)
    {
        const auto oldCapacity = capacity_;
        const auto newSize = Size() + 1;
        if (capacity_ == 0)
        {
            Reserve(1);
        }
        else if (newSize > oldCapacity)
        {
            auto newCapacity = oldCapacity;
            while (newCapacity < newSize)
            {
                newCapacity = defaultReallocSizeFunc(newCapacity);
            }
            Reserve(newCapacity);
        }
        
        beginPtr_[newSize - 1] = val;
        ++endPtr_;
    }
    void PushBack(T&& val)
    {
        const auto oldCapacity =  capacity_;
        const auto newSize = Size() + 1;
        if(oldCapacity == 0)
        {
            Reserve(1);
        }
        else
        {
            if (newSize > oldCapacity)
            {
                auto newCapacity = oldCapacity;
                while (newCapacity < newSize)
                {
                    newCapacity = defaultReallocSizeFunc(newCapacity);
                }
                Reserve(newCapacity);
            }
        }

        beginPtr_[newSize - 1] = std::move(val);

        ++endPtr_;
    }

    template<typename... Args>
    void EmplaceBack(Args... args)
    {
        const auto oldCapacity = capacity_;
        const auto newSize = Size() + 1;
        if (oldCapacity == 0)
        {
            Reserve(1);
        }
        else if (newSize > oldCapacity)
        {
            auto newCapacity = oldCapacity;
            while (newCapacity < newSize)
            {
                newCapacity = defaultReallocSizeFunc(newCapacity);
            }
            Reserve(newCapacity);
        }
        ::new (beginPtr_+newSize-1) T(args...);
        ++endPtr_;
    }

    void PopBack()
    {
        if constexpr (!std::is_trivially_constructible_v<T>)
        {
            beginPtr_[Size() - 1].~T();
        }
        --endPtr_;
    }

    void Erase(const_iterator it)
    {
        auto ptr = &(*it);
        if constexpr (std::is_trivially_destructible_v<T>)
        {
            ptr->~T();
        }
        const auto lastElementsCount = std::distance(it, cend())-1;
        if (lastElementsCount != 0)
        {
            std::memmove(ptr, ptr + 1, lastElementsCount*sizeof(T));
        }
        --endPtr_;
    }

    void Erase(const_iterator beginIt, const_iterator endIt)
    {
        auto beginPtr = &(*beginIt);
        auto endPtr = &(*endIt);
        const auto elementCount = std::distance(beginPtr, endPtr);
        if constexpr (std::is_trivially_destructible_v<T>)
        {
            for (auto it = beginPtr; it != endPtr; ++it)
            {
                it->~T();
            }
        }
        const auto lastElementsCount = std::distance(beginPtr, endPtr_) - elementCount;
        if (lastElementsCount != 0)
        {
            std::memmove(beginPtr, beginPtr + elementCount, lastElementsCount*sizeof(T));
        }
        endPtr_ -= elementCount;

    }

    [[nodiscard]] T& Front() { return *beginPtr_; }
    [[nodiscard]] const T& Front() const { return *beginPtr_; }

    [[nodiscard]] T& Back() { return *(beginPtr_+Size()-1); }
    [[nodiscard]] const T& Back() const { return *(beginPtr_+Size()-1); }
    [[nodiscard]] T& operator[](std::size_t index) { return beginPtr_[index]; }
    [[nodiscard]] const T& operator[](std::size_t index) const { return beginPtr_[index]; }

    inline static constexpr auto defaultReallocSizeFunc = [](std::size_t n) { return n * 2; };
private:
    T* beginPtr_ = nullptr;
    T* endPtr_ = nullptr;
    std::size_t capacity_ = 0;
    inline static DumbAllocator defaultAllocator_;
    Allocator& allocator_;
};
}