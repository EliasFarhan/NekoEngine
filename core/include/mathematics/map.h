#pragma once
#include <engine/custom_allocator.h>
#include <vector>
#include <xxhash.hpp>

namespace neko{

template <typename Key, typename Value>
class Map{
public:
    Map(const Allocator& allocator){
        allocator_ = allocator;
    }
    ~Map(){

    }

    // Copy assignment
    Map<Key,Value> operator=(const Map<Key,Value>& other){
        return Map<Key,Value>();
    }

    // Move assignment
    Map<Key,Value>& operator=(Map<Key,Value>&& other) noexcept{
        return *this;
    }

    bool IsEmpty(){
        return false;
    }

    size_t Count(){
        return pairs_.count;
    }

    bool Add(Key key, Value value){
        xxh::hash_t<64> hash = xxh::xxhash<64>(&key, sizeof(key));
		for (int i = 0; i < pairs_.size(); i++)
		{
			if(pairs_[i].first == nullptr)
			{
                pairs_[i].first == hash;
                pairs_[i].second == value;
                return true;
			}
		}
        return false;
    }

    bool Remove(const Key key){
        xxh::hash_t<64> hash = xxh::xxhash<64>(&key, sizeof(key));
    	for(int i = 0; i < pairs_.size(); i++)
    	{
    		if (pairs_[i].first == hash)
    		{
                pairs_[i].first = nullptr;
                pairs_[i].second = nullptr;
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
        xxh::hash_t<64> hashA = xxh::xxhash<64>(&key, sizeof(key));
        xxh::hash_t<64> hashB = xxh::xxhash<64>(&key, sizeof(key));
        int intA;
        int intB;
        int check = 0;              //Check increases each time he find a value
    	for(int i = 0; i <pairs_.size(); i++)
    	{
    		if (pairs_[i].first == hashA)
    		{
                intA = i;
                check++;
    			if(check >=2)
    			{
                    break;
    			}
    		}
            else if (pairs_[i].first == hashB)
            {
                intB = i;
                check++;
            	if(check >=2)
            	{
                    break;
            	}
            }
    	}
    	if (check < 2)
    	{
            return false;
    	}
        Value value = pairs_[intA].second;
        pairs_[intA].second = pairs_[intB].second;
        pairs_[intB].second = value;
        return true;
    }

    void Clear(){
        
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
    Allocator& allocator_;
};

}// !neko