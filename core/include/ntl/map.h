#pragma once
#include <map>
#include <unordered_map>

#include "engine/custom_allocator.h"

namespace neko
{
template<typename Key, typename Value>
using SortedMap = std::map < Key, Value, std::less<Key>, StandardLibraryAllocator<std::pair<const Key, Value>>>;
template<typename Key, typename Value>
using UnsortedMap = std::unordered_map< Key, Value, std::less<Key>, StandardLibraryAllocator<std::pair<const Key, Value>>>;

template<typename Key, typename Value, size_t N>
class FixedMap
{
public:
    explicit FixedMap(Allocator& allocator);
    ~FixedMap();

    Value& operator[](Key key)
    {
        for(auto it = begin(); it != end(); ++it)
        {
            if((*it).first == key)
            {
                return (*it).second;
            }
        }
        neko_assert(last_ < end_, "Out of range");
        //Inserting new element
        last_->first = key;
        auto& value = last_->second;
        ++last_;
        return value;
    }
private:
    using Pair = std::pair<Key, Value>;
    //STL conventions
public:
    class iterator
    {
    public:
        // iterator traits
        using difference_type = ptrdiff_t;
        using value_type = Pair;
        using pointer = Pair*;
        using reference = Pair&;
        using iterator_category = std::random_access_iterator_tag;
        iterator(Pair* pair): ptr_(pair){}
        iterator& operator++() { ++ptr_; return *this; }
        iterator& operator--() { --ptr_; return *this; }
        iterator operator+(size_t v) { return iterator(ptr_ + v); }
        iterator operator-(size_t v) { return iterator(ptr_ - v); }
        bool operator==(iterator other) const { return ptr_ == other.ptr_; }
        bool operator!=(iterator other) const { return !(*this == other); }
        Pair& operator*() { return *ptr_; }

    private:
        Pair* ptr_ = nullptr;
    };
    iterator begin() { return iterator(start_); }
    iterator end() { return iterator(last_); }
    [[nodiscard]] size_t size() const { return static_cast<size_t>(last_ - start_); }
    [[nodiscard]] size_t capacity() const { return static_cast<size_t>(end_ - start_); }
private:
    friend class iterator;
    ProxyAllocator allocator_;

    std::pair<Key, Value>* start_= nullptr;
    std::pair<Key, Value>* last_ = nullptr;
    std::pair<Key, Value>* end_ = nullptr;
};

template <typename Key, typename Value, size_t N>
FixedMap<Key, Value, N>::FixedMap(Allocator& allocator) : allocator_(allocator)
{
    start_ = static_cast<Pair*>(allocator_.Allocate(N * sizeof(Pair), alignof(Pair)));
    end_ = start_ + N;
    last_ = start_;
}

template <typename Key, typename Value, size_t N>
FixedMap<Key, Value, N>::~FixedMap()
{
    allocator_.Deallocate(start_);
    start_ = nullptr;
    last_ = nullptr;
    end_ = nullptr;
}
}
