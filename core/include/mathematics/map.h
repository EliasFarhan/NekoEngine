#pragma once
#include <engine/custom_allocator.h>
#include <vector>

namespace neko{

template <typename Key, typename Value>
class Map{
public:
    Map(const Allocator& allocator){

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
        return 0;
    }

    void Add(){

    }

    bool Remove(const Key key){
        return false;
    }

    bool Swap(const Key a, const Key b){
        return false;
    }

    void Clear(){

    }

    bool ContainsKey(const Key key){

    }

    bool ContainsValue(const Value value){

    }
private:
    std::vector<std::pair<Key, Value>> pairs_;
    Allocator& allocator_;
};

}// !neko