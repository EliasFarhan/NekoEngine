#pragma once

#include "allocator/allocator.h"

namespace neko
{
    class LinearAllocator : public CustomAllocator
    {
    public:
        void* Allocate(std::size_t allocatedSize, std::size_t alignment) override;
        void Deallocate(void* ptr) override;
        void Clear();
        void Init(void* rootPtr, std::size_t size) override;
    private:
        mutable std::mutex mutex_;
        void* currentPos_ = nullptr;
    };
}