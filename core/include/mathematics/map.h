#pragma once

#include <engine/custom_allocator.h>
#include <vector> // TODO: replace this with neko's own container.
#include <xxhash.hpp>
#include <array>

namespace neko
{

template<typename Key, typename Value>
class Map
{
public:
    // Constructor / destructor.
    Map(Allocator* allocator, const size_t sizeInBytes)
    {
        allocator = nullptr; // TODO: Pass this allocator to neko's container.

        pairs_.resize(sizeInBytes / sizeof(Value));
        for (auto& pair : pairs_)
        {
            pair = std::pair<xxh::hash_t<64>, Value>(0, Value());
        }
    }

    // Overloads.
    inline Map<Key, Value>& operator=(Map<Key, Value>&& other) noexcept
    {
        return *this;
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
        return pairs_.capacity();
    }

    bool Add(const Key key, const Value value)
    {
        // TODO@Seb: check that key doesn't exist already in the map! Otherwise 1 key may have multiple values!

        const xxh::hash_t<64> hash = xxh::xxhash<64>(&key, sizeof(Key));
        const size_t len = pairs_.capacity();
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
        const size_t len = pairs_.capacity();
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
        const size_t len = pairs_.size(); // TODO@Seb: this...

        for (size_t i = 0; i < len; i++)
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
    std::vector<std::pair<xxh::hash_t<64>, Value>> pairs_; // TODO: Replace this with neko's container when it's ready.
    size_t count_ = 0;
};

/*template <typename Key, typename Value>
class NonVectMap{
public:
    // Constructor / destructor.
    NonVectMap(Allocator* allocator, const size_t sizeInBytes)
    {
        allocator_ = allocator;
        sizeInBytes_ = sizeInBytes;

        for (size_t i = 0; i < sizeInBytes / sizeof(Value) ; i++)
        {
            pairs_[i] = static_cast<std::pair<xxh::hash_t<64>, Value>*>(allocator_->Allocate(sizeof(std::pair<xxh::hash_t<64>, Value>), alignof(std::pair<xxh::hash_t<64>, Value>)));
        }
    }

    // Overloads.
    inline NonVectMap<Key, Value>& operator=(Map<Key, Value>&& other) noexcept
    {
        return *this;
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
        return sizeInBytes_ / sizeof(std::pair<xxh::hash_t<64>, Value>);
    }

    bool Add(const Key key, const Value value)
    {
        // TODO@Seb: check that key doesn't exist already in the map! Otherwise 1 key may have multiple values!

        const xxh::hash_t<64> hash = xxh::xxhash<64>(&key, sizeof(Key));
        const size_t len = Capacity();
        for (size_t i = 0; i < len; i++)
        {
            if (pairs_[i]->first == 0)
            {
                pairs_[i]->first = hash;
                pairs_[i]->second = value;
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
        const size_t len = Capacity();
        for (int i = 0; i < len; i++)
        {
            if (pairs_[i]->first == hash)
            {
                pairs_[i]->first = 0;
                pairs_[i]->second = Value();
                count_--;
                return true;
            }
        }
        return false;
    }

    void Clear()
    {
        const size_t len = Capacity();
        for (size_t i = 0; i < len; i++)
        {
            pairs_[i]->first = 0;
            pairs_[i]->second = Value();
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
            auto hash = pairs_[i]->first;
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
        const auto temp = pairs_[index0]->second;
        pairs_[index0]->second = pairs_[index1]->second;
        pairs_[index1]->second = temp;
        return true;
    }

    Value FindValue(const Key key)
    {
        // TODO@Seb: Have to traverse whole vector regardless of count_ if there are holes in the memory layout!

        const xxh::hash_t<64> hash = xxh::xxhash<64>(&key, sizeof(Key));
        const size_t len = Capacity();

        for (size_t i = 0; i < len; i++)
        {
            if (pairs_[i]->first == hash)
            {
                return pairs_[i]->second;
            }
        }
        // assert("Value not found!"); // TEMPORARY for testing purposes!
        return 0;
    }

private:
    size_t count_ = 0;
    size_t sizeInBytes_ = 0;
    Allocator* allocator_ = nullptr;
    std::pair<xxh::hash_t<64>, Value>* pairs_[];
};*/
/*
template <typename Key, typename Value>
class MinimalMap {
    using Hash = xxh::hash_t<64>;
    using Pair = std::pair<Hash, Value>;

public:
    MinimalMap(const size_t sizeInNumberOfCacheLines = 1){

        sizeInNumverOfCacheLines_ = sizeInNumberOfCacheLines;
        assert(64 * sizeInNumverOfCacheLines_ >= sizeof(Value)); // Assume a single pair to fit inside a cache line.

        pairs_ = static_cast<Pair*>(malloc(64 * sizeInNumverOfCacheLines_));
        const int len = (64 * sizeInNumverOfCacheLines_) / sizeof(Pair);
        for (int i = 0; i < len; ++i)
        {
            *(pairs_ + (i * sizeof(Pair))) = Pair(Hash(), Value()); // Set memory to 0.
        }
    }

    inline bool Add(const Key key, const Value value)
    {
        for (int i = 0; i < (64 * sizeInNumverOfCacheLines_) / sizeof(Pair); ++i) // Vectorizable.
        {
            if ((pairs_ + (i * sizeof(Pair)))->first == 0){
                (pairs_ + (i * sizeof(Pair)))->first = xxh::xxhash<64>(&key, sizeof(Key)); // Q: sizeof(Key) is const at runtime?
                (pairs_ + (i * sizeof(Pair)))->second = value;
                return true; // Early exit.
            }
        }
        return false;
    }

     inline bool Remove(const Key key){
        auto hash = xxh::xxhash<64>(&key, sizeof(Key));
        for (int i = 0; i < (64 * sizeInNumverOfCacheLines_) / sizeof(Pair); ++i)
        {
            if ((pairs_ + (i * sizeof(Pair)))->first == hash){
                (pairs_ + (i * sizeof(Pair)))->first = 0;
                (pairs_ + (i * sizeof(Pair)))->second = Value();
                return true; // Early exit.
            }
        }
        return false;
    }

    inline Value Retrieve(const Key key) const
    {
        auto hash = xxh::xxhash<64>(&key, sizeof(Key));
        for (int i = 0; i < (64 * sizeInNumverOfCacheLines_) / sizeof(Pair); ++i)
        {
            if ((pairs_ + (i * sizeof(Pair)))->first == hash){
                return (pairs_ + (i * sizeof(Pair)))->second;
            }
        }
        return Value();
    }

    ~MinimalMap(){
        free(pairs_);
    }
private:
    size_t sizeInNumverOfCacheLines_;
    Pair* pairs_; // Actual values are on heap, ptrs on stack.
};*/

template <typename Key, typename Value>
class PoolAllocatorMap{
    using Hash = xxh::hash_t<64>;
    using Pair = std::pair<Hash, Value>;

public:
    PoolAllocatorMap(const uint8_t sizeInCacheLines){
        mem_ = malloc((64 * sizeInCacheLines) + 1);
        allocator_ = PoolAllocator<Pair>((64 * sizeInCacheLines) + 1, mem_);
        pairs_.resize(64 * sizeInCacheLines);
    }

    void Add(const Key key, const Value value){
        Pair* pair = static_cast<Pair*>(allocator_.Allocate(sizeof(Pair), alignof(Pair)));
        pair->first = xxh::xxhash<64>(&key, sizeof(Key));
        pair->second = value;
        pairs_[pairs_.size()] = pair;
    }

    Value Retrieve(const Key key){
        const auto hash = xxh::xxhash<64>(&key, sizeof(key));
        for (Pair* pair : pairs_){
            if (pair->first == hash){
                return pair->second;
            }
        }
        return Value();
    }

    void Remove(const Key key){
        const auto hash = xxh::xxhash<64>(&key, sizeof(key));
        for (Pair* pair : pairs_){
            if (pair->first == hash){
                pair->first = 0;
                pair->second = Value();
            }
        }
    }

    ~PoolAllocatorMap(){
        free(mem_);
    }
private:
    void* mem_ = nullptr; // 8 bytes
    PoolAllocator<Pair> allocator_ = nullptr; // 40 bytes
public: // FOR TESTING PURPOSES
    std::vector<Pair*> pairs_; // 24 bytes?
};

}// !neko