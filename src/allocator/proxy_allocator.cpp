#include "allocator/proxy_allocator.h"
#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif
namespace neko
{
ProxyAllocator::ProxyAllocator(AllocatorInterface& allocator): allocator_(allocator)
{
}

void* ProxyAllocator::Allocate(std::size_t size, std::size_t alignement)
{

    auto* ptr = allocator_.Allocate(size, alignement);
#ifdef TRACY_ENABLE
    TracyAlloc(ptr, size);
#endif
    return ptr;
}

void ProxyAllocator::Deallocate(void* ptr)
{
#ifdef TRACY_ENABLE
    TracyFree(ptr);
#endif
    allocator_.Deallocate(ptr);
}
}
