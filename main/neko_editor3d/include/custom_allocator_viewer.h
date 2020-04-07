#pragma once

#include <vector>
#include <engine/system.h>
#include "engine/custom_allocator.h"

namespace neko::editor
{

class CustomAllocatorViewer;

class CustomAllocatorTester : public DrawImGuiInterface
{
public:
    explicit CustomAllocatorTester(CustomAllocatorViewer& viewer);
    ~CustomAllocatorTester();
    void DrawImGui() override;

    struct CustomData
    {
        char customData[16];
    };
    enum CustomAllocatorType
    {
        None,
        LinearAllocator,
        StackAllocator,
        FreeListAllocator,
        PoolAllocator,
        Length
    };
private:
    void* data_ = nullptr;
    CustomAllocatorType currentCustomAllocatorType = CustomAllocatorType::None;
    std::unique_ptr<neko::LinearAllocator> linearAllocator_ {nullptr};
    std::unique_ptr<neko::StackAllocator> stackAllocator_ {nullptr};
    std::unique_ptr<neko::FreeListAllocator> freeListAllocator_ {nullptr};
    std::unique_ptr<neko::PoolAllocator<CustomData>> poolAllocator_ {nullptr};
    CustomAllocatorViewer& viewer_;
    std::vector<void*> allocatedPtr_;
    const size_t dataSize_ = 1024;
};

class CustomAllocatorViewer : public DrawImGuiInterface
{
public:
	void DrawImGui() override;
	void SetAllocator(Allocator* allocator);
private:
	Allocator* allocator_ = nullptr;
};

}
