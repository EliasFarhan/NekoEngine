#pragma once

#include <engine/custom_allocator.h>
#include <xxhash.hpp>
#include <algorithm>
#include <cmath>
#include <utility>

namespace neko
{

#define HASH_SIZE 32 // bits
template<typename Key, typename Value, const size_t Capacity>
class FixedMap // 32 bytes, 8 bytes aligned
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
        auto it = std::find_if(Begin(), End(), [hash](const InternalPair& p) { return p.first == hash; });
        neko_assert(it != End(), "neko::FixedMap<Key,Value,Capacity>::operator[](Key&): Key is not in map.");
#endif

        return BinarySearch(begin_, end_, hash);
    }

    void Insert(const Pair& pair)
    {

#ifdef NEKO_ASSERT
        {
            neko_assert(size_ < Capacity, "neko::FixedMap<Key,Value,Capacity>::Insert(Pair&): Map is full.")
            const auto hash = xxh::xxhash<HASH_SIZE>(&pair.first, 1);
            auto it = std::find_if(Begin(), End(), [hash](const InternalPair& p) { return p.first == hash; });
            neko_assert(it == End(), "neko::FixedMap<Key,Value,Capacity>::Insert(Pair&): Key already in map.");
        }
#endif

        const InternalPair newPair = InternalPair{xxh::xxhash<HASH_SIZE>(&pair.first, 1), pair.second};

        auto it = std::find_if(Begin(), End(), [newPair](const InternalPair& p){ return newPair.first < p.first;});

        char* left = (char*)&(*it);
        const char* right = (char*)end_;
        const size_t len = (size_t)(right - left);
        memmove((void*)(left + sizeof(InternalPair)), left, len);

        *((InternalPair*)left) = newPair;

        end_++;
        size_++;
/*
#ifdef NEKO_ASSERT
        for (size_t i = 0; i < size_ - 1; i++)
        {
            neko_assert(begin_[i].first < begin_[i + 1].first, "neko::FixedMap<Key,Value,Capacity>::Insert(Pair&): Map is not ordered.");
        }
#endif
*/
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

    inline Iterator Back() const
    {
        // neko_assert(size_ > 0, "neko::FixedMap<Key,Value,Capacity>::Back(): Map is empty.")
        return Iterator(end_ - 1);
    }
    inline Iterator Front() const
    {
        // neko_assert(size_ > 0, "neko::FixedMap<Key,Value,Capacity>::Front(): Map is empty.")
        return Iterator(begin_);
    }

private:
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

template<typename Key, typename Value>
class DynamicMap // 32 bytes, 8 bytes aligned
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
    DynamicMap(LinearAllocator& allocator) : allocator_(allocator)
    {
        begin_ = (InternalPair*) allocator_.Allocate(2 * sizeof(InternalPair), alignof(InternalPair));
        end_ = begin_;
        max_ = begin_ + 2;
    }

    ~DynamicMap()
    {
        allocator_.Clear();
    }

    DynamicMap() = delete; // Allocator is mandatory.
    DynamicMap(DynamicMap<Key, Value>& other) = delete; // No copying because map is using an allocator.
    DynamicMap(DynamicMap<Key, Value>&& other) = delete; // No moving because map deallocates on destruction.
    DynamicMap<Key, Value>& operator=(const DynamicMap<Key, Value>& other) = delete;

    DynamicMap<Key, Value>& operator=(const DynamicMap<Key, Value>&& other) = delete;

    inline Value& operator[](const Key& key)
    {
        const auto hash = xxh::xxhash<HASH_SIZE>(&key, 1);

#ifdef NEKO_ASSERT
        auto it = std::find_if(Begin(), End(), [hash](const InternalPair& p) { return p.first == hash; });
        neko_assert(it != End(), "neko::FixedMap<Key,Value,Capacity>::operator[](Key&): Key is not in map.");
#endif

        return BinarySearch(begin_, end_, hash);
    }

    void Insert(const Pair& pair)
    {
        if (end_ == max_)
        {
            char temp[Size() * sizeof(InternalPair)];
            memcpy(&temp, begin_, Size() * sizeof(InternalPair));
            const size_t tempSize = Size();

            allocator_.Clear();
            begin_ = (InternalPair*) allocator_.Allocate(2* tempSize * sizeof(InternalPair), alignof(InternalPair));
            end_ = begin_ + tempSize;
            max_ = begin_ + (tempSize * 2);

            memcpy(begin_, &temp, tempSize * sizeof(InternalPair));
        }

#ifdef NEKO_ASSERT
        {
            const auto hash = xxh::xxhash<HASH_SIZE>(&pair.first, 1);
            auto it = std::find_if(Begin(), End(), [hash](const InternalPair& p) { return p.first == hash; });
            neko_assert(it == End(), "neko::FixedMap<Key,Value,Capacity>::Insert(Pair&): Key already in map.");
        }
#endif

        const InternalPair newPair = InternalPair{xxh::xxhash<HASH_SIZE>(&pair.first, 1), pair.second};

        auto it = std::find_if(Begin(), End(), [newPair](const InternalPair& p){ return newPair.first < p.first;});

        char* left = (char*)&(*it);
        const char* right = (char*)end_;
        const size_t len = (size_t)(right - left);
        memmove((void*)(left + sizeof(InternalPair)), left, len);

        *((InternalPair*)left) = newPair;

        end_++;

#ifdef NEKO_ASSERT
        for (size_t i = 0; i < Size() - 1; i++)
        {
            neko_assert(begin_[i].first < begin_[i + 1].first, "neko::FixedMap<Key,Value,Capacity>::Insert(Pair&): Map is not ordered.");
        }
#endif

    }

    inline void Clear()
    {
        end_ = begin_;
    }

    inline size_t Size() const
    {
        return end_ - begin_;
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

    inline Iterator Back() const
    {
        neko_assert(Size() > 0, "neko::FixedMap<Key,Value,Capacity>::Back(): Map is empty.")
        return Iterator(end_ - 1);
    }
    inline Iterator Front() const
    {
        neko_assert(Size() > 0, "neko::FixedMap<Key,Value,Capacity>::Front(): Map is empty.")
        return Iterator(begin_);
    }

private:
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
    InternalPair* max_ = nullptr; // 8 bytes
};
#undef HASH_SIZE

template<typename Key, typename Value, const size_t Capacity>
class SmallMap
{
    using Pair = std::pair<Key, Value>;
public:

    class Iterator
    {
    public:
        typedef Iterator self_type;
        typedef Pair value_type;
        typedef Pair& reference;
        typedef Pair* pointer;
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

    SmallMap()
    {
        memset(pairs_, 0, Capacity * sizeof(Pair));
    }

    inline Value& operator[](const Key& key)
    {

#ifdef NEKO_ASSERT
        auto it = std::find_if(Begin(), End(), [key](const Pair p) { return p.first == key; });
        neko_assert(it != End(), "neko::FixedMap<Key,Value,Capacity>::operator[](Key&): Key is not in map.");
#endif

        return std::find_if(Begin(), End(), [key](const Pair p){ return p.first == key;})->second;
    }

    void Insert(const Pair& pair)
    {

#ifdef NEKO_ASSERT
        {
            neko_assert(pair.first != 0, "neko::FixedMap<Key,Value,Capacity>::Insert(Pair&): Key of 0 is reserved.")

            auto it = std::find_if(Begin(), End(), [](const Pair p){return p.first == 0;});
            neko_assert(it != End(), "neko::FixedMap<Key,Value,Capacity>::Insert(Pair&): Map is full.");

            it = std::find_if(Begin(), End(), [pair](const Pair p) { return p.first == pair.first; });
            neko_assert(it == End(), "neko::FixedMap<Key,Value,Capacity>::Insert(Pair&): Key already in map.");
        }
#endif

        auto it = std::find_if(Begin(), End(), [](const Pair p){ return p.first == 0;});
        *it = pair;
    }

    inline void Clear()
    {
        memset(pairs_, 0, Capacity * sizeof(Pair));
    }

    inline size_t Size() const
    {
        return Capacity;
    }

    inline Iterator Begin()
    {
        return Iterator(pairs_);
    }

    // Used by for auto
    inline Iterator begin() const
    {
        return Begin();
    }

    inline Iterator End()
    {
        return Iterator(pairs_ + Capacity);
    }

    // Used by for auto
    inline Iterator end() const
    {
        return End();
    }

private:
    Pair pairs_[Capacity];
};

}// !neko