#pragma once
#include <vector>
#include <algorithm>
#include "engine/custom_allocator.h"

namespace neko
{
template<typename T>
using ArrayList = std::vector<T, StandardLibraryAllocator<T>>;

template<typename T, size_t N>
class FixedArrayList
{
public:
    explicit FixedArrayList(Allocator& allocator);
    ~FixedArrayList();

    FixedArrayList(const FixedArrayList<T, N>& array);
    T& operator[](size_t index)
    {
        neko_assert(index < N, "Out of index");
        return start_[index];
    }
    T& at(size_t index)
    {
        neko_assert(index < N, "Out of index");
        return start_[index];
    }
    const T& at(size_t index) const
    {
        neko_assert(index < N, "Out of index");
        return start_[index];
    }
    //STL convention
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    class iterator
    {
    public:
        // iterator traits
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;
        explicit iterator(T* ptr) : ptr_(ptr) {}
        iterator& operator++() { ++ptr_; return *this; }
        iterator& operator--() { --ptr_; return *this; }
        iterator operator+(size_t v) { return iterator(ptr_ + v); }
        iterator operator-(size_t v) { return iterator(ptr_ - v); }
        difference_type operator-(iterator it) { return static_cast<difference_type>(ptr_ - it.ptr_); }
        bool operator==(iterator other) const { return ptr_ == other.ptr_; }
        bool operator!=(iterator other) const { return !(*this == other); }
        T& operator*() { return *ptr_; }

    private:
        T* ptr_ = nullptr;
    };
    class const_iterator
    {
    public:
        // iterator traits
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;
        explicit const_iterator(T* ptr) : ptr_(ptr) {}
        const_iterator& operator++() { ++ptr_; return *this; }
        const_iterator& operator--() { --ptr_; return *this; }
        const_iterator operator+(size_t v) { return const_iterator(ptr_ + v); }
        const_iterator operator-(size_t v) { return const_iterator(ptr_ - v); }
        difference_type operator-(const_iterator it) { return static_cast<difference_type>(ptr_ - it.ptr_); }
        bool operator==(const_iterator other) const { return ptr_ == other.ptr_; }
        bool operator!=(const_iterator other) const { return !(*this == other); }
        const T& operator*() { return *ptr_; }

    private:
        T* ptr_ = nullptr;
    };

    iterator begin() { return iterator(start_); }
    iterator end() { return iterator(last_); }
    const_iterator cbegin() { return const_iterator(start_); }
    const_iterator cend() { return const_iterator(end_); }
    size_t size() const { return static_cast<size_t>(last_ - start_); }
    size_t capacity() const { return static_cast<size_t>(end_ - start_); }
    void push_back(const value_type& val)
    {
        neko_assert(last_ < end_, "Out of range");
        *last_ = val;
        ++last_;
    }
    iterator insert(iterator position, const value_type& val)
    {
        neko_assert(last_ < end_, "Out of range");
        for (auto it = end(); it != position; --it)
        {
            *it = *(it - 1);
        }
        *position = val;
        ++last_;
        return position;
    }
    void clear()
    {
        if (!std::is_trivially_destructible<T>::value)
        {
            for (auto& v : *this)
            {
                v.~T();
            }
        }
        last_ = start_;
    }
    T& front() { return start_[0]; }
    [[nodiscard]] const T& front() const { return start_[0]; }
    T& back() { return *(last_ - 1); }
    [[nodiscard]] const T& back() const { return *(last_ - 1); }
    iterator erase(const_iterator position)
    {
        const auto index = std::distance(cbegin(), position);
        if (!std::is_trivially_destructible<T>::value)
        {
            (*this)[index].~T();
        }
        for (auto it = begin() + index; it != end(); ++it)
        {
            *it = *(it + 1);
        }
        --last_;
        return begin() + index;
    }
    iterator erase(const_iterator first, const_iterator last)
    {
        const auto index = std::distance(cbegin(), first);
        const auto length = std::distance(first, last);
        if (!std::is_trivially_destructible<T>::value)
        {
            for (auto it = begin() + index; it != begin() + index + length; ++it)
            {
                (*it).~T();
            }
        }
        for (auto it = begin() + index; it != end() - length; ++it)
        {
            *it = *(it + length);
        }
        last_ -= length;
        return begin() + index;
    }
private:
    friend class iterator;
    Allocator& allocator_;
    T* start_ = nullptr;
    T* last_ = nullptr;
    T* end_ = nullptr;
};


template <typename T, size_t N>
FixedArrayList<T, N>::FixedArrayList(Allocator& allocator) : allocator_(allocator)
{
    start_ = static_cast<T*>(allocator.Allocate(N * sizeof(T), alignof(T)));
    neko_assert(start_ != nullptr, "Failed to allocate");
    last_ = start_;
    end_ = start_ + N;
}

template <typename T, size_t N>
FixedArrayList<T, N>::~FixedArrayList()
{
    if (!std::is_trivially_destructible<T>::value)
    {
        for (auto& v : *this)
        {
            v.~T();
        }
    }
    allocator_.Deallocate(start_);
    last_ = nullptr;
    end_ = nullptr;
}

template <typename T, size_t N>
FixedArrayList<T, N>::FixedArrayList(const FixedArrayList<T, N>& array) : allocator_(array.allocator_)
{
    start_ = static_cast<T*>(allocator_.Allocate(N * sizeof(T), alignof(T)));
    end_ = start_ + N;
    neko_assert( array.size() < N, "Out of space" );
    const auto size = array.size();
    for(size_t i = 0; i < size; ++i)
    {
        (*this)[i] = array.at(i);
    }
    last_ = start_ + size;

}

template<typename T, size_t N>
class FixedArray
{
public:
    explicit FixedArray(Allocator& allocator);
    ~FixedArray();
    T& operator[](size_t index)
    {
        neko_assert(index < N, "Out of index");
        return start_[index];
    }
    //Following STL convention 
    class iterator
    {
    public:
        // iterator traits
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;
        explicit iterator(T* ptr) :ptr_(ptr) {}
        iterator& operator++() { ++ptr_; return *this; }
        bool operator==(iterator other) const { return ptr_ == other.ptr_; }
        bool operator!=(iterator other) const { return !(*this == other); }
        T& operator*() { return *ptr_; }

    private:
        T* ptr_ = nullptr;
    };
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;

    iterator begin() { return iterator(start_); }
    iterator end() { return iterator(end_); }
    size_t size() { return reinterpret_cast<size_t>(end_ - start_ - 1); }
private:
    friend class iterator;
    Allocator& allocator_;
    T* start_ = nullptr;
    T* end_ = nullptr;
};


template <typename T, size_t N>
FixedArray<T, N>::FixedArray(Allocator& allocator) : allocator_(allocator)
{
    start_ = static_cast<T*>(allocator_.Allocate(N * sizeof(T), alignof(T)));
    neko_assert(start_ != nullptr, "Failed to allocate memory for FixedArray");
    end_ = start_ + N;
}

template <typename T, size_t N>
FixedArray<T, N>::~FixedArray()
{
    allocator_.Deallocate(start_);
    start_ = nullptr;
    end_ = nullptr;
}
}
