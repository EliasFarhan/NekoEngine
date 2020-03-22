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
    class iterator
    {
    public:
        typedef iterator self_type;
        typedef InternalPair value_type;
        typedef InternalPair& reference;
        typedef InternalPair* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;

        iterator(pointer ptr) : ptr_(ptr)
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

    class const_iterator
    {
    public:
        typedef const_iterator self_type;
        typedef InternalPair value_type;
        typedef InternalPair& reference;
        typedef InternalPair* pointer;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;

        const_iterator(pointer ptr) : ptr_(ptr)
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

        const pointer operator->()
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
        auto it = std::find_if(begin(), end(), [&hash](const InternalPair p) { return p.first == hash; });

        neko_assert(it != end(), "neko::FixedMap<Key,Value,Capacity>::operator[](Key): Key not found.");

        return *it;
    }

    void insert(const Pair& pair)
    {
        neko_assert(size() < Capacity, "neko::FixedMap<Key,Value,Capacity>::insert(Pair&): Map is full.");

#ifdef NEKO_ASSERT
        const auto hash = xxh::xxhash<64>(&pair.first, 1);
        auto it = std::find_if(begin(), end(), [&hash](const InternalPair p){return p.first == hash;});
        neko_assert(it == end(), "neko::FixedMap<Key,Value,Capacity>::insert(Pair&): Map already contains key.");
#endif NEKO_ASSERT

        *end_ = InternalPair{xxh::xxhash<64>(&(pair.first), 1), pair.second};
        end_++;
    }

    iterator begin() const
    {
        return iterator{begin_};
    }

    iterator end() const
    {
        return iterator{end_};
    }

    const_iterator cbegin() const
    {
        return const_iterator{begin_};
    }

    const_iterator cend() const
    {
        return const_iterator{end_};
    }

    void clear()
    {
        end_ = begin_;
    }

    inline size_t size() const
    {
        return (size_t) ((((std::uint64_t) end_) - ((std::uint64_t) begin_)) / (std::uint64_t) sizeof(InternalPair));
    }

private:
    InternalPair* begin_ = nullptr;
    InternalPair* end_ = nullptr;
    Allocator& allocator_;
};
/*
template<typename Key, typename Value, const size_t Size>
class FixedMap
{
public:
    using Hash = xxh::hash_t<64>;
    using Pair = std::pair<Hash, Value>;

    FixedMap() {
        begin_.resize(Size); // Initializes the pairs to 0.
    }

    ~FixedMap() = default;

    [[nodiscard]] size_t Capacity() const {
        return begin_.capacity();
    }

    Value& operator[](const Key key) {
        const Hash hash = xxh::xxhash<64>(&key, 1);
        auto it = std::find_if(begin_.begin(), begin_.end(), [hash](Pair& p) { return p.first == hash; });

        neko_assert(it != begin_.end(),
            "neko::FixedMap<Key,Value>::operator[](const Key): Key passed to operator not found.");
        return it->second;
    }

    bool Contains(const Key key) const {
        const Hash hash = xxh::xxhash<64>(&key, 1);
        return std::find_if(begin_.begin(), begin_.end(), [hash](Pair p) { return p.first == hash; }) != begin_.end();
    }

    void Append(const Key key, const Value value) {
        neko_assert(!Contains(key),
            "neko::FixedMap<Key,Value>::Append(const Key, const Value): Map already contains Key passed.");
        auto it = std::find_if(begin_.begin(), begin_.end(), [](Pair& p) { return p.first == 0; });
        neko_assert(it != begin_.end(),
            "neko::FixedMap<Key,Value>::Append(const Key, const Value): No more free slots in map.");
            it->first = xxh::xxhash<64>(&key, 1);
        it->second = value;
    }

    void Remove(Key key)
    {
        neko_assert(Contains(key), "neko::FixedMap<Key, Value>::Remove(const Key): Key doesn't exist");
        const Hash hash = xxh::xxhash<64>(&key, 1);
        auto it = std::find_if(begin_.begin(), begin_.end(), [hash](Pair p) { return p.first == hash; });
        it->first = 0;
        it->second = 0;
    }

    void Clear()
    {
        std::for_each(begin_.rbegin(), begin_.rend(), [](Pair& p) {p.first = 0; p.second = 0; return; });
    }

    typename std::vector<Pair>::iterator begin() {
        return begin_.begin();
    }

    typename std::vector<Pair>::iterator end() {
        return begin_.end();
    }

    typename std::vector<Pair>::const_iterator cbegin() const {
        return begin_.cbegin();
    }

    typename std::vector<Pair>::const_iterator cend() const {
        return begin_.cend();
    }

private:
    std::vector<Pair> begin_; // 24 bytes // TODO: Replace this with neko's container when it's ready.
    // TODO: use Allocator& allocator_;
};
*/
/*template<typename Key, typename Value, const size_t Capacity>
class SmallMap
{
public:
    using Hash = xxh::hash_t<64>;
    using Pair = std::pair<Hash, Value>;

    SmallMap()
    {
        begin_.size(Capacity);
    }

    ~SmallMap() = default;

    Value& operator[](Key key)
    {
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        auto it = std::find_if(begin_.begin(), begin_.end(), [hash](Pair& p) { return p.first == hash; });

        neko_assert(it != begin_.end(),
                    "neko::SmallFixedMap<Key,Value>::operator[](const Key): Key passed to operator not found.");
        return it->second;
    }

    bool Contains(Key key) const
    {
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        return std::find_if(begin_.begin(), begin_.end(), [hash](Pair p) { return p.first == hash; }) != begin_.end();
    }

    void Append(Key key, Value value)
    {
        neko_assert(!Contains(key),
                    "neko::SmallFixedMap<Key, Value>::Append(const Key, const Value): SmallFixedMap already contains Key passed.");
        auto it = std::find_if(begin_.begin(), begin_.end(), [](Pair& p) { return p.first == 0; });
        neko_assert(it != begin_.end(),
                    "neko::SmallFixedMap<Key,Value>::Append(const Key, const Value): No more free slots in smallFixedMap.");
        it->first = xxh::xxhash<64>(&key, sizeof(Key));
        it->second = value;
    }

    void Remove(Key key)
    {
        neko_assert(Contains(key), "neko::SmallFixedMap<Key, Value>::Remove(const Key): Key doesn't exist");
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        auto it = std::find_if(begin_.begin(), begin_.end(), [hash](Pair p) { return p.first == hash; });
        it->first = 0;
        it->second = 0;
    }

    void Clear()
    {
        std::for_each(begin_.begin(), begin_.end(), [](Pair& p) {
            p.first = 0;
            p.second = 0;
            return;
        });
    }

private:
    std::array<Pair, Capacity> begin_; // Oleg@Seb: wat? pq 0? deverait être Size du template
    size_t size_ = 0;
};*/

/*template<typename Key, typename Value, const size_t Size>
class DynamicMap
{
public:
    using Hash = xxh::hash_t<64>;
    using Pair = std::pair<Hash, Value>;

    DynamicMap()
    {
        begin_.size() = 0;
    }

    ~DynamicMap() = default;

    Value& operator[](Key key)
    {
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        auto it = std::find_if(begin_.begin(), begin_.end(), [hash](Pair& p) { return p.first == hash; });

        neko_assert(it != begin_.end(),
                    "neko::DynamicMap<Key,Value>::operator[](const Key): Key passed to operator not found.");
        return it->second;
    }

    bool Contains(Key key) const
    {
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        return std::find_if(begin_.begin(), begin_.end(), [hash](Pair p) { return p.first == hash; }) != begin_.end();
    }

    void Append(Key key, Value value)
    {
        neko_assert(!Contains(key),
                    "neko::DynamicMap<Key,Value>::Append(const Key, const Value): DynamicMap already contains Key passed.");
        begin_.push_back(Pair(key, value));
    }

    void Remove(Key key)
    {
        neko_assert(Contains(key), "neko::DynamicMap<Key, Value>::Remove(const Key): Key doesn't exist");
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        auto it = std::find_if(begin_.begin(), begin_.end(), [hash](Pair p) { return p.first == hash; });
        begin_.erase(it);
    }

    void Clear()
    {
        begin_.clear();
    }

private:
    std::vector<Pair> begin_;        // TODO: Replace this with neko's container when it's ready.
};*/

}// !neko