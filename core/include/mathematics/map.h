#pragma once
#include <engine/custom_allocator.h>
#include <vector>
#include <xxhash.hpp>

namespace neko{

template <typename Key, typename Value>
class Map{
public:
    Map(Allocator* allocator, const size_t sizeInBytes){
        allocator_ = allocator;
        count_ = 0;
        pairs_.reserve(sizeInBytes / sizeof(Value));
        pairs_.resize(sizeInBytes / sizeof(Value));
        for (auto& pair : pairs_)
        {
            pair = std::pair<xxh::hash_t<64>, Value>(0,Value());
        }
    }
    ~Map(){

    }

    // Copy assignment
    /*Map<Key,Value> operator=(const Map<Key,Value>& other){
        return Map<Key,Value>();
    }*/

    // Move assignment
    Map<Key,Value>& operator=(Map<Key,Value>&& other) noexcept{
        return *this;
    }

    bool IsEmpty(){
        return count_ != 0;
    }

    size_t Count(){
        return count_;
    }

    bool Add(Key key, Value value){
        xxh::hash_t<64> hash = xxh::xxhash<64>(&key, sizeof(key));
		for (int i = 0; i < pairs_.capacity(); i++)
		{
			if(pairs_[i].first == 0)
			{
                pairs_[i].first = hash;
                pairs_[i].second = value;
                return true;
			}
		}
        return false;
    }

    bool Remove(const Key key){
        xxh::hash_t<64> hash = xxh::xxhash<64>(&key, sizeof(key));
    	for(int i = 0; i < pairs_.capacity(); i++)
    	{
    		if (pairs_[i].first == hash)
    		{
                pairs_[i].first = 0;
                pairs_[i].second = Value();
                return true;
    		}
    	}
        return false;
    }

    bool Swap(const Key a, const Key b){
    	if (a == b)
    	{
            return false;
    	}
        xxh::hash_t<64> hashA = xxh::xxhash<64>(&a, sizeof(a));
        xxh::hash_t<64> hashB = xxh::xxhash<64>(&b, sizeof(b));
        int intA;
        int intB;
    	for(int i = 0; i <pairs_.size(); i++)
    	{
    		if (pairs_[i].first == hashA)
    		{
    			
    		}
    	}
        return false;
    }

    Value FindValue(const Key key){
        xxh::hash_t<64> hash = xxh::xxhash<64>(&key, sizeof(key));
	    for(int i = 0; i < pairs_.size(); i++)
	    {
            if (pairs_[i].first == hash)
            {
                return pairs_[i].second;
            }
	    }
        return Value();
    }

private:
    std::vector<std::pair<xxh::hash_t<64>, Value>> pairs_;
    Allocator* allocator_;
    size_t count_;
};

}// !neko