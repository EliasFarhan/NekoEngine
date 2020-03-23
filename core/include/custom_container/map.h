#pragma once

#include <engine/custom_allocator.h>
#include <xxhash.hpp>
#include <algorithm>

namespace neko
{

template<typename Key, typename Value, const size_t Capacity>
class FixedMap
{
    using Hash = xxh::hash_t<64>;
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

    Value& operator[](const Key key)
    {
        const auto hash = xxh::xxhash<64>(&key, 1);
        auto it = std::find_if(Begin(), End(), [&hash](const InternalPair p) { return p.first == hash; });

        neko_assert(it != End(), "neko::FixedMap<Key,Value,Capacity>::operator[](Key): Key not found.");

        return it->second;
    }

    void Insert(const Pair& pair)
    {
        neko_assert(Size() < Capacity, "neko::FixedMap<Key,Value,Capacity>::insert(Pair&): Map is full.");

#ifdef NEKO_ASSERT
        const auto hash = xxh::xxhash<64>(&pair.first, 1);
        auto it = std::find_if(Begin(), End(), [&hash](const InternalPair p){return p.first == hash;});
        neko_assert(it == End(), "neko::FixedMap<Key,Value,Capacity>::insert(Pair&): Map already contains key.");
#endif // !NEKO_ASSERT

        *end_ = InternalPair{xxh::xxhash<64>(&(pair.first), 1), pair.second};
        end_++;
    }

    Iterator Begin() const
    {
        return Iterator{begin_};
    }

    Iterator End() const
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

}// !neko