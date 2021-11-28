#pragma once
#include "allocator/allocator.h"

namespace neko
{

class ProxyAllocator : public AllocatorInterface
{
public:
    ProxyAllocator(AllocatorInterface& allocator);

    void* Allocate(std::size_t size, std::size_t alignement) override;
    void Deallocate(void* ptr) override;
private:
    AllocatorInterface& allocator_;
};


}