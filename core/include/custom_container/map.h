#pragma once

#include <engine/custom_allocator.h>
#include <xxhash.hpp>
#include <algorithm>

namespace neko
{

#define HASH_SIZE 32 // 32 bits hash

template<typename Key, typename Value, const size_t Capacity>
class FixedMap
{
    using Hash = xxh::hash_t<HASH_SIZE>;
    using Pair = std::pair<Key, Value>;
    using InternalPair = std::pair<Hash, Value>;

public:
    class Iterator
    {
    public:
        typedef InternalPair value_type; // mandatory for std lib
        typedef InternalPair& reference; // mandatory for std lib
        typedef InternalPair* pointer; // mandatory for std lib
        typedef std::forward_iterator_tag iterator_category; // mandatory for std lib
        typedef int difference_type; // mandatory for std lib

        Iterator(InternalPair* ptr) : ptr_(ptr)
        {}

        Iterator operator++()
        {
            Iterator i = *this;
            ptr_++;
            return i;
        }

        Iterator operator++(int junk)
        {
            ptr_++;
            return *this;
        }

        InternalPair& operator*()
        { return *ptr_; }

        inline InternalPair* operator->()
        { return ptr_; }

        bool operator==(const Iterator& rhs)
        { return ptr_ == rhs.ptr_; }

        bool operator!=(const Iterator& rhs)
        { return ptr_ != rhs.ptr_; }

    private:
        InternalPair* ptr_;
    };

    // Constructors / destructors / copy and move.
    FixedMap(Allocator& allocator) : allocator_(allocator)
    {
        begin_ = (InternalPair*) allocator_.Allocate(sizeof(InternalPair) * Capacity, alignof(InternalPair));
        end_ = begin_;
    }

    FixedMap(Allocator&& allocator) : allocator_(allocator)
    {
        begin_ = (InternalPair*) allocator_.Allocate(sizeof(InternalPair) * Capacity, alignof(InternalPair));
        end_ = begin_;
    }

    ~FixedMap()
    {
        allocator_.Deallocate(begin_);
    }

    FixedMap() = delete; // Allocator is mandatory.
    FixedMap(FixedMap<Key, Value, Capacity>& other) = delete; // No copying because map is using an allocator.
    FixedMap(FixedMap<Key, Value, Capacity>&& other) = delete; // No moving because map deallocates on destruction.
    FixedMap<Key, Value, Capacity>& operator=(const FixedMap<Key, Value, Capacity>& other) = delete;

    FixedMap<Key, Value, Capacity>& operator=(const FixedMap<Key, Value, Capacity>&& other) = delete;

    inline Value& operator[](const Key& key)
    {
        const auto hash = xxh::xxhash<HASH_SIZE>(&key, 1);
        auto it = std::find_if(Begin(), End(), [hash](const InternalPair p) { return p.first == hash; }); // it cannot be made const because returning non const value

        neko_assert(it != End(), "neko::FixedMap<Key,Value,Capacity>::operator[](Key): Key not found.");

        return it->second;
    }

    void Insert(const Pair& pair)
    {
        neko_assert(Size() < Capacity, "neko::FixedMap<Key,Value,Capacity>::insert(Pair&): Map is full.");

#ifdef NEKO_ASSERT
        const auto hash = xxh::xxhash<HASH_SIZE>(&pair.first, 1);
        auto it = std::find_if(Begin(), End(), [&hash](const InternalPair p){return p.first == hash;});
        neko_assert(it == End(), "neko::FixedMap<Key,Value,Capacity>::insert(Pair&): Map already contains key.");
#endif // !NEKO_ASSERT

        *end_ = InternalPair{xxh::xxhash<HASH_SIZE>(&(pair.first), 1), pair.second};
        end_++;
    }

    inline Iterator Begin() const
    {
        return Iterator{begin_};
    }

    inline Iterator End() const
    {
        return Iterator{end_};
    }

    void Clear()
    {
        end_ = begin_;
    }

    inline size_t Size() const
    {
        return (size_t) ((((std::uint64_t) end_) - ((std::uint64_t) begin_)) / (std::uint64_t) sizeof(InternalPair));
    }

private:
    InternalPair* begin_ = nullptr;
    InternalPair* end_ = nullptr;
    Allocator& allocator_;
};

#undef HASH_SIZE

}// !neko