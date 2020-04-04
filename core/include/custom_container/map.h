#pragma once

#include <engine/custom_allocator.h>
#include <xxhash.hpp>
#include <algorithm>
#include <cmath>
#include <utility>

// TODO: @Oleg: Calculate absolute best case possible for retrieval allowed by hardware.

namespace neko
{

static const size_t HASH_SIZE = 32; // bits

template<typename Key, typename Value, const size_t Capacity>
class FixedMap // 32 bytes size, 8 bytes aligned
{
    using Hash = xxh::hash_t<HASH_SIZE>;
    using Pair = std::pair<Key, Value>;
    using InternalPair = std::pair<Hash, Value>;

public:
    class Iterator
    {
    public:
        typedef Iterator self_type;
        typedef InternalPair value_type;
        typedef InternalPair& reference;
        typedef InternalPair* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;

        Iterator(pointer ptr) : ptr_(ptr)
        {}

        self_type operator++()
        {
            self_type i = *this;
            ptr_++;
            return i;
        }

        self_type operator++(int junk)
        {
            ptr_++;
            return *this;
        }

        reference operator*()
        { return *ptr_; }

        pointer operator->()
        { return ptr_; }

        bool operator==(const self_type& rhs)
        { return ptr_ == rhs.ptr_; }

        bool operator!=(const self_type& rhs)
        { return ptr_ != rhs.ptr_; }

    private:
        pointer ptr_;
    };

    // Constructors / destructors / copy and move.
    FixedMap(LinearAllocator& allocator) : allocator_(allocator)
    {
        begin_ = (InternalPair*) allocator_.Allocate(Capacity * sizeof(InternalPair), alignof(InternalPair));
        end_ = begin_;
    }

    ~FixedMap()
    {
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

#ifdef NEKO_ASSERT
        for (size_t i = 0; i < size_ - 1; i++)
        {
            if (begin_[i] >= begin_[i + 1])
            {
                neko_assert(false,
                            "neko::FixedMap<Key,Value,Capacity>::operator[](Key&): Map is not sorted. Use Rearrange() before trying to retrieve anything.");
            }
        }

        auto it = std::find_if(Iterator{begin_}, Iterator{end_},
                               [hash](const InternalPair& p) { return p.first == hash; });
        neko_assert(it != Iterator{end_}, "neko::FixedMap<Key,Value,Capacity>::operator[](Key&): Key is not in map.");
#endif

        return BinarySearch(begin_, end_, hash);
    }

    void Insert(const Pair& pair)
    {

#ifdef NEKO_ASSERT
        neko_assert(size_ < Capacity, "neko::FixedMap<Key,Value,Capacity>::operator[](Key&): Map is full.")
        const auto hash = xxh::xxhash<HASH_SIZE>(&pair.first, 1);
        auto it = std::find_if(Iterator{begin_}, Iterator{end_},
                               [hash](const InternalPair& p) { return p.first == hash; });
        neko_assert(it == Iterator{end_}, "neko::FixedMap<Key,Value,Capacity>::operator[](Key&): Key already in map.");
#endif

        const InternalPair newPair = InternalPair{xxh::xxhash<HASH_SIZE>(&pair.first, 1), pair.second};
        if (begin_ == end_){
            *end_ = newPair;
            end_++;
            return;
        }

        InternalPair* back = end_ - 1;

        if (newPair.first >= back->first) // Current pair should go after the one with currently biggest hash.
        {
            *back = newPair;
            end_++;
        }else // Current pair should go before the one with the currently biggest hash.
        {
            auto it = std::find_if(Iterator{begin_}, Iterator{end_}, [newPair](const InternalPair p){return p.first > newPair.first;}); // Find insertion point.

            InternalPair* source = &(*it);
            InternalPair* destination = source + 1;
            memmove(destination, source, (end_ - source) * sizeof(InternalPair)); // Move right the part of the memory to make room for the new element.
            end_++;

            *source = newPair; // Assign new element.
        }

        size_++;
    }

    inline void Clear()
    {
        end_ = begin_;
        size_ = 0;
    }

    inline size_t Size() const
    {
        return size_;
    }

    inline Iterator Begin() const
    {
        return Iterator{begin_};
    }

    // Used by for auto
    inline Iterator begin() const
    {
        return Begin();
    }

    inline Iterator End() const
    {
        return Iterator{end_};
    }

    // Used by for auto
    inline Iterator end() const
    {
        return End();
    }

private:
    // Sort the map elements in ascending order by key.
    void Rearrange()
    {
        std::vector<InternalPair> temp(begin_, end_);
        std::sort(temp.begin(), temp.end(),
                  [](const InternalPair& a, const InternalPair& b) { return a.first < b.first; });
        const size_t len = size_;
        for (size_t i = 0; i < len; i++)
        {
            begin_[i] = temp[i];
        }
    }

    Value& BinarySearch(InternalPair* begin, InternalPair* end, const Hash lookingFor) const
    {
        InternalPair* middle = begin + ((end - begin) >> 1);

        if (middle->first == lookingFor)
        {
            return middle->second;
        }

        if (middle->first > lookingFor)
        {
            return BinarySearch(begin, middle - 1, lookingFor);
        }
        else
        {
            return BinarySearch(middle + 1, end, lookingFor);
        }
    }

    InternalPair* begin_ = nullptr; // 8 bytes
    InternalPair* end_ = nullptr; // 8 bytes
    LinearAllocator& allocator_; // 8 bytes
    size_t size_ = 0; // 8 bytes
};

#undef HASH_SIZE

}// !neko