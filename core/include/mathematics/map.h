#pragma once

#include <engine/custom_allocator.h>
#include <vector> // TODO: replace this with neko's own container.
#include <xxhash.hpp>
#include <algorithm>

namespace neko
{

template<typename Key, typename Value, const size_t Size>
class FixedMap // 24 bytes
{
public:
    using Hash = xxh::hash_t<64>;
    using Pair = std::pair<Hash, Value>;

    FixedMap(){
        pairs_.resize(Size); // Initializes the pairs to 0.
    }

    ~FixedMap() = default;

    [[nodiscard]] size_t Capacity() const{
        return pairs_.capacity();
    }

    Value& operator[](Key key){
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        auto it = std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair& p) ->bool { return p.first == hash; });

        neko_assert(it != pairs_.end(),
                    "neko::Map<Key,Value>::operator[](const Key): Key passed to operator not found.");
        return it->second;
    }

    bool Contains(Key key) const{
        const Hash hash = xxh::xxhash<64>(&key, sizeof(Key));
        return std::find_if(pairs_.begin(), pairs_.end(), [hash](Pair p) ->bool { return p.first == hash; });
    }

    void Append(Key key, Value value){
        neko_assert(!Contains(key),
                    "neko::Map<Key,Value>::Append(const Key, const Value): Map already contains Key passed.");
        auto it = std::find_if(pairs_.begin(), pairs_.end(), [](Pair& p) ->bool { return p.first == 0; });
        neko_assert(it != pairs_.end(),
                    "neko::Map<Key,Value>::Append(const Key, const Value): No more free slots in map.")
        it->first = xxh::xxhash<64>(&key, sizeof(Key));
        it->second = value;
    }

    typename std::vector<Pair>::iterator begin(){
        return pairs_.begin();
    }

    typename std::vector<Pair>::iterator end(){
        return pairs_.end();
    }

    typename std::vector<Pair>::const_iterator cbegin() const{
        return pairs_.cbegin();
    }

    typename std::vector<Pair>::const_iterator cend() const{
        return pairs_.cend();
    }

private:
    std::vector<Pair> pairs_; // 24 bytes // TODO: Replace this with neko's container when it's ready.
    // Allocator& allocator_;
};

/*template<typename Key, typename Value, size_t Capacity>
class ArrayMap
{
public:
    // Constructor / destructor.
    ArrayMap(const size_t sizeInBytes)
    {
        allocator = nullptr; // TODO: Pass this allocator to neko's container.
        capacity = sizeInBytes / sizeof(Value);
        pairs_ = pairs_[capacity];
        for (auto& pair : pairs_)
        {
            pair = std::pair<xxh::hash_t<64>, Value>(0, Value());
        }
    }

    inline bool IsEmpty()
    {
        return count_ == 0;
    }

    inline size_t Count()
    {
        return count_;
    }

    inline size_t Capacity()
    {
        return capacity;
    }

    bool Add(const Key key, const Value value)
    {
        // TODO@Seb: check that key doesn't exist already in the map! Otherwise 1 key may have multiple values!

        const xxh::hash_t<64> hash = xxh::xxhash<64>(&key, sizeof(Key));
        const size_t len = capacity;
        for (size_t i = 0; i < len; i++)
        {
            if (pairs_[i].first == 0)
            {
                pairs_[i].first = hash;
                pairs_[i].second = value;
                count_++;
                return true;
            }
        }
        // assert("Could not add to map!"); // TEMPORARY for testing purposes!
        return false;
    }

    bool Remove(const Key key)
    {
        const xxh::hash_t<64> hash = xxh::xxhash<64>(&key, sizeof(Key));
        const size_t len = capa;
        for (int i = 0; i < len; i++)
        {
            if (pairs_[i].first == hash)
            {
                pairs_[i].first = 0;
                pairs_[i].second = Value();
                count_--;
                return true;
            }
        }
        return false;
    }

    void Clear()
    {
        for (auto& pair : pairs_)
        {
            pair = std::pair<xxh::hash_t<64>, Value>(0, Value());
        }
        count_ = 0;
    }

    bool Swap(const Key a, const Key b)
    {
        if (a == b || count_ < 2) return false;

        const xxh::hash_t<64> hash0 = xxh::xxhash<64>(&a, sizeof(Key));
        const xxh::hash_t<64> hash1 = xxh::xxhash<64>(&b, sizeof(Key));
        const size_t len = count_;

        size_t index0 = 0;
        size_t index1 = 0;
        size_t i = 0;
        bool firstMatch = false;

        do
        {
            auto hash = pairs_[i].first;
            if (hash == hash0 || hash == hash1)
            {
                if (!firstMatch)
                {
                    index0 = i;
                    firstMatch = true;
                }
                else
                {
                    index1 = i;
                    goto RETURN;
                }
            }
        } while (i++ < len);
        return false; // Traversed whole map without finding both keys.

        RETURN:
        const auto temp = pairs_[index0].second;
        pairs_[index0].second = pairs_[index1].second;
        pairs_[index1].second = temp;
        return true;
    }

    Value FindValue(const Key key) const
    {
        // TODO@Seb: Have to traverse whole vector regardless of count_ if there are holes in the memory layout!

        const xxh::hash_t<64> hash = xxh::xxhash<64>(&key, sizeof(Key));
        const size_t len = capacity; // TODO@Seb: this...       

        for (size_t i = 0; i < len; i++)        //
        {
            if (pairs_[i].first == hash)
            {
                return pairs_[i].second;
            }
        }
        // assert("Value not found!"); // TEMPORARY for testing purposes!
        return 0;
    }

private:
    std::array<std::pair<xxh::hash_t<64>, Value>, Capacity> pairs_;

    //std::vector<std::pair<xxh::hash_t<64>, Value>> pairs_; // TODO: Replace this with neko's container when it's ready.
    size_t count_ = 0;
};*/

}// !neko