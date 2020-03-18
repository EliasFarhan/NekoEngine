#pragma once

#include <engine/custom_allocator.h>
#include <xxhash.hpp>
#include <algorithm>

namespace neko
{

/*template<typename Key, typename Value, const size_t Size>
class FixedMap
{
    using Hash = xxh::hash_t<64>;
    using Pair = std::pair<Hash, Value>;

public:
    FixedMap(Allocator& allocator) : allocator_(allocator)
    {
        pairs_.fill(nullptr);
    }

    ~FixedMap()
    {
        std::for_each(pairs_.begin(), pairs_.end(), [this](Pair* p) {
            if (p != nullptr)
            {
                this->allocator_.Deallocate(p);
            }
        });
        pairs_.fill(nullptr);
    }

    void Append(const std::pair<Key, Value> pair)
    {
        const auto it = std::find_if(pairs_.begin(), pairs_.end(), [](const Pair* p) { return p == nullptr; });
        neko_assert(it != pairs_.end(), "neko::FixedMap<Key,Value,Size>::Append(Pair&): No more place in map.");
        (*it) = static_cast<Pair*>(allocator_.Allocate(sizeof(Pair), alignof(Pair)));
        *(*it) = Pair{
                xxh::xxhash<64>(&(pair.first), 1),
                pair.second};
    }

private:
    std::array<Pair*, Size> pairs_;
    Allocator& allocator_;
};*/

template<typename Key, typename Value, const size_t Size>
class FixedMap
{
public:
    using Hash = xxh::hash_t<64>;
    using Pair = std::pair<Hash, Value>;

    FixedMap() {
        pairs_.resize(Size); // Initializes the pairs to 0.
    }

    ~FixedMap() = default;

    [[nodiscard]] size_t Capacity() const {
        return pairs_.capacity();
    }

    Value& operator[](const Key key) {
        const Hash hash = xxh::xxhash<64>(&key, 1);
        auto it = std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair& p) { return p.first == hash; });

        neko_assert(it != pairs_.end(),
            "neko::FixedMap<Key,Value>::operator[](const Key): Key passed to operator not found.");
        return it->second;
    }

    bool Contains(const Key key) const {
        const Hash hash = xxh::xxhash<64>(&key, 1);
        return std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair p) { return p.first == hash; }) != pairs_.end();
    }

    void Append(const Key key, const Value value) {
        neko_assert(!Contains(key),
            "neko::FixedMap<Key,Value>::Append(const Key, const Value): Map already contains Key passed.");
        auto it = std::find_if(pairs_.begin(), pairs_.end(), [](Pair& p) { return p.first == 0; });
        neko_assert(it != pairs_.end(),
            "neko::FixedMap<Key,Value>::Append(const Key, const Value): No more free slots in map.")
            it->first = xxh::xxhash<64>(&key, 1);
        it->second = value;
    }

    void Remove(Key key)
    {
        neko_assert(Contains(key), "neko::FixedMap<Key, Value>::Remove(const Key): Key doesn't exist");
        const Hash hash = xxh::xxhash<64>(&key, 1);
        auto it = std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair p) { return p.first == hash; });
        it->first = 0;
        it->second = 0;
    }

    void Clear()
    {
        std::for_each(pairs_.rbegin(), pairs_.rend(), [](Pair& p) {p.first = 0; p.second = 0; return; });
    }

    typename std::vector<Pair>::iterator begin() {
        return pairs_.begin();
    }

    typename std::vector<Pair>::iterator end() {
        return pairs_.end();
    }

    typename std::vector<Pair>::const_iterator cbegin() const {
        return pairs_.cbegin();
    }

    typename std::vector<Pair>::const_iterator cend() const {
        return pairs_.cend();
    }

private:
    std::vector<Pair> pairs_; // 24 bytes // TODO: Replace this with neko's container when it's ready.
    // TODO: use Allocator& allocator_;
};

template<typename Key, typename Value, const size_t Size>
class SmallFixedMap
{
public:
    using Hash = xxh::hash_t<64>;
    using Pair = std::pair<Hash, Value>;

    SmallFixedMap()
    {
        pairs_.size(Size);
    }

    ~SmallFixedMap() = default;

    [[nodiscard]] size_t Capacity() const
    {
        return pairs_.size();
    }

    Value& operator[](Key key)
    {
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        auto it = std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair& p) { return p.first == hash; });

        neko_assert(it != pairs_.end(),
                    "neko::SmallFixedMap<Key,Value>::operator[](const Key): Key passed to operator not found.");
        return it->second;
    }

    bool Contains(Key key) const
    {
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        return std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair p) { return p.first == hash; }) != pairs_.end();
    }

    void Append(Key key, Value value)
    {
        neko_assert(!Contains(key),
                    "neko::SmallFixedMap<Key, Value>::Append(const Key, const Value): SmallFixedMap already contains Key passed.");
        auto it = std::find_if(pairs_.begin(), pairs_.end(), [](Pair& p) { return p.first == 0; });
        neko_assert(it != pairs_.end(),
                    "neko::SmallFixedMap<Key,Value>::Append(const Key, const Value): No more free slots in smallFixedMap.");
        it->first = xxh::xxhash<64>(&key, sizeof(Key));
        it->second = value;
    }

    void Remove(Key key)
    {
        neko_assert(Contains(key), "neko::SmallFixedMap<Key, Value>::Remove(const Key): Key doesn't exist");
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        auto it = std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair p) { return p.first == hash; });
        it->first = 0;
        it->second = 0;
    }

    void Clear()
    {
        std::for_each(pairs_.rbegin(), pairs_.rend(), [](Pair& p) {
            p.first = 0;
            p.second = 0;
            return;
        });
    }

    typename std::array<Pair, Size>::iterator begin()
    {
        return pairs_.begin();
    }

    typename std::array<Pair, Size>::iterator end()
    {
        return pairs_.end();
    }

    typename std::array<Pair, Size>::const_iterator cbegin() const
    {
        return pairs_.cbegin();
    }

    typename std::array<Pair, Size>::const_iterator cend() const
    {
        return pairs_.cend();
    }

private:
    std::array<Pair, 0> pairs_; // Oleg@Seb: wat? pq 0? deverait être Size du template

};

template<typename Key, typename Value, const size_t Size>
class DynamicMap
{
public:
    using Hash = xxh::hash_t<64>;
    using Pair = std::pair<Hash, Value>;

    DynamicMap()
    {
        pairs_.size() = 0;
    }

    ~DynamicMap() = default;

    Value& operator[](Key key)
    {
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        auto it = std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair& p) { return p.first == hash; });

        neko_assert(it != pairs_.end(),
                    "neko::DynamicMap<Key,Value>::operator[](const Key): Key passed to operator not found.");
        return it->second;
    }

    bool Contains(Key key) const
    {
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        return std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair p) { return p.first == hash; }) != pairs_.end();
    }

    void Append(Key key, Value value)
    {
        neko_assert(!Contains(key),
                    "neko::DynamicMap<Key,Value>::Append(const Key, const Value): DynamicMap already contains Key passed.");
        pairs_.push_back(Pair(key, value));
    }

    void Remove(Key key)
    {
        neko_assert(Contains(key), "neko::DynamicMap<Key, Value>::Remove(const Key): Key doesn't exist");
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        auto it = std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair p) { return p.first == hash; });
        pairs_.erase(it);
    }

    void Clear()
    {
        pairs_.clear();
    }

    typename std::vector<Pair>::iterator begin()
    {
        return pairs_.begin();
    }

    typename std::vector<Pair>::iterator end()
    {
        return pairs_.end();
    }

    typename std::vector<Pair>::const_iterator cbegin() const
    {
        return pairs_.cbegin();
    }

    typename std::vector<Pair>::const_iterator cend() const
    {
        return pairs_.cend();
    }

private:
    std::vector<Pair> pairs_;        // TODO: Replace this with neko's container when it's ready.
};

}// !neko