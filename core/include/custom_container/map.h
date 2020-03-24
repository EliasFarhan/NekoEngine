#pragma once

#include <engine/custom_allocator.h>
#include <xxhash.hpp>
#include <algorithm>
#include <cmath>

// TODO: @Oleg: Calculate absolute best case possible for retrieval allowed by hardware.

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
    /*class Iterator
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
    };*/

    // Constructors / destructors / copy and move.
    FixedMap(LinearAllocator& allocator) : allocator_(allocator)
    {
        const float SIZE_OF_VALUE = sizeof(Value);
        const float SIZE_OF_HASH = sizeof(Hash);
        const float SIZE_OF_CACHE_LINE = 64;
        const size_t PAIRS_IN_CACHE_LINE = std::floor(SIZE_OF_CACHE_LINE / (SIZE_OF_HASH + SIZE_OF_VALUE));
        const size_t OCCUPIED_CACHE_LINE = 64 - (PAIRS_IN_CACHE_LINE * (SIZE_OF_HASH + SIZE_OF_VALUE));
        const size_t UNUSED_CACHE_LINE = 64 - OCCUPIED_CACHE_LINE;

        keysBegin_ = (Hash*) allocator_.Allocate((sizeof(Hash) + sizeof(Value)) * Capacity, 64);
    }

    /*FixedMap(LinearAllocator&& allocator) : allocator_(allocator)
    {
        keysBegin_ = (Hash*) allocator_.Allocate(sizeof(Hash) * Capacity, alignof(Hash));
        keysEnd_ = keysBegin_;
        valuesBegin_ = (Value*) allocator_.Allocate(sizeof(Value) * Capacity, alignof(Value));
        valuesEnd_ = valuesBegin_;
    }*/

    ~FixedMap()
    {
        // allocator_.Deallocate(begin_);
        allocator_.Clear();
    }

    FixedMap() = delete; // Allocator is mandatory.
    FixedMap(FixedMap<Key, Value, Capacity>& other) = delete; // No copying because map is using an allocator.
    FixedMap(FixedMap<Key, Value, Capacity>&& other) = delete; // No moving because map deallocates on destruction.
    FixedMap<Key, Value, Capacity>& operator=(const FixedMap<Key, Value, Capacity>& other) = delete;

    FixedMap<Key, Value, Capacity>& operator=(const FixedMap<Key, Value, Capacity>&& other) = delete;

    inline Value& operator[](const Key& key)
    {
        const auto hash = xxh::xxhash<HASH_SIZE>(&key, 1);
        const size_t size = Size();
        for (size_t i = 0; i < size; i++)
        {
            if (keysBegin_[i] == hash){
                return valuesBegin_[i];
            }
        }
        neko_assert(false, "neko::FixedMap<Key,Value,Capacity>::operator[](Key): Key not found.");
    }

    void Insert(const Pair& pair)
    {
        neko_assert(Size() < Capacity, "neko::FixedMap<Key,Value,Capacity>::insert(Pair&): Map is full.");

#ifdef NEKO_ASSERT
        const auto hash = xxh::xxhash<HASH_SIZE>(&pair.first, 1);
        const auto it = std::find(keysBegin_, keysEnd_, hash);
        neko_assert(it == keysEnd_, "neko::FixedMap<Key,Value,Capacity>::insert(Pair&): Map already contains key.");
#endif // !NEKO_ASSERT

        *keysEnd_ = xxh::xxhash<HASH_SIZE>(&(pair.first), 1); // InternalPair{xxh::xxhash<HASH_SIZE>(&(pair.first), 1), pair.second};
        *valuesEnd_ = pair.second;
        keysEnd_++;
        valuesEnd_++;
    }

    /*inline Iterator Begin() const
    {
        return Iterator{begin_};
    }

    inline Iterator End() const
    {
        return Iterator{end_};
    }*/

    void Clear()
    {
        keysEnd_ = keysBegin_;
        valuesEnd_ = valuesBegin_;
    }

    inline size_t Size() const
    {
        return (size_t) ((((std::uint64_t) keysEnd_) - ((std::uint64_t) keysBegin_)) / (std::uint64_t) sizeof(Hash));
    }

private:
    // InternalPair* begin_ = nullptr;
    // InternalPair* end_ = nullptr;
    LinearAllocator& allocator_;
    Hash* keysBegin_ = nullptr;
    Hash* keysEnd_ = nullptr;
    Value* valuesBegin_ = nullptr;
    Value* valuesEnd_ = nullptr;
};

#undef HASH_SIZE

}// !neko