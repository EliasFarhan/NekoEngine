#include <algorithm>
#include <custom_allocator_viewer.h>
#include <imgui.h>

namespace neko::editor
{
void CustomAllocatorViewer::DrawImGui()
{

	ImGui::Begin("Custom Allocator Profile");
	if (allocator_ != nullptr)
	{
		size_t usedMemory = allocator_->GetUsedMemory();
		ImGui::LabelText("Used Memory", "%zu B", usedMemory);
		size_t totalMemory = allocator_->GetSize();
		ImGui::LabelText("Total Memory", "%zu B", totalMemory);
		allocator_->Profile();
	}
	else
	{
		ImGui::TextColored(ImColor(255, 0, 0, 255), "No custom allocator is selected");
	}
	ImGui::End();
}

void CustomAllocatorViewer::SetAllocator(Allocator* allocator)
{
	allocator_ = allocator;
}

CustomAllocatorTester::CustomAllocatorTester(CustomAllocatorViewer& viewer) : viewer_(viewer)
{
	data_ = malloc(dataSize_);
}

void CustomAllocatorTester::DrawImGui()
{
	static const char* allocatorNames[(int)CustomAllocatorType::Length] =
	{
			"None",
			"Linear Allocator",
			"Stack Allocator",
			"FreeList Allocator",
			"Pool Allocator"
	};
	ImGui::Begin("Custom Allocator Tester");
	static int currentItem = 0;
	if (ImGui::Combo("Custom Allocator Type", &currentItem, allocatorNames, (int)CustomAllocatorType::Length))
	{
		switch (currentCustomAllocatorType)
		{
		case LinearAllocator:
		{
			linearAllocator_->Clear();
			linearAllocator_ = nullptr;
			break;
		}
		case StackAllocator:
		{
			std::for_each(allocatedPtr_.rbegin(), allocatedPtr_.rend(),
				[this](void* ptr) { stackAllocator_->Deallocate(ptr); });
			stackAllocator_ = nullptr;
			break;
		}
		case FreeListAllocator:
			std::for_each(allocatedPtr_.begin(), allocatedPtr_.end(),
				[this](void* ptr) { freeListAllocator_->Deallocate(ptr); });
			freeListAllocator_ = nullptr;
			break;
		case PoolAllocator:
			std::for_each(allocatedPtr_.begin(), allocatedPtr_.end(),
				[this](void* ptr) { poolAllocator_->Deallocate(ptr); });
			poolAllocator_ = nullptr;
			break;
		default:
			break;
		}
		allocatedPtr_.clear();
		currentCustomAllocatorType = (CustomAllocatorType)currentItem;
		switch (currentCustomAllocatorType)
		{
		case LinearAllocator:
		{
			linearAllocator_ = std::make_unique<neko::LinearAllocator>(dataSize_, data_);
			viewer_.SetAllocator(linearAllocator_.get());
			break;
		}
		case StackAllocator:
		{
			stackAllocator_ = std::make_unique<neko::StackAllocator>(dataSize_, data_);
			viewer_.SetAllocator(stackAllocator_.get());
			break;
		}
		case FreeListAllocator:
		{
			freeListAllocator_ = std::make_unique<neko::FreeListAllocator>(dataSize_, data_);
			viewer_.SetAllocator(freeListAllocator_.get());
			break;
		}
		case PoolAllocator:
		{
			poolAllocator_ = std::make_unique<neko::PoolAllocator<CustomData>>(dataSize_, data_);
			viewer_.SetAllocator(poolAllocator_.get());
			break;
		}
		case None:
		{
			viewer_.SetAllocator(nullptr);
			break;
		}
		default:
			break;
		}
	}
	static int allocationNmb = 1;
	ImGui::SliderInt("Allocation Nmb", &allocationNmb, 1, 10);
	if (ImGui::Button("Allocate"))
	{
		switch (currentCustomAllocatorType)
		{
		case LinearAllocator:
		{
			allocatedPtr_.push_back(
				linearAllocator_->Allocate(allocationNmb * sizeof(CustomData), alignof(CustomData)));
			break;
		}
		case StackAllocator:
		{
			allocatedPtr_.push_back(
				stackAllocator_->Allocate(allocationNmb * sizeof(CustomData), alignof(CustomData)));
			break;
		}
		case FreeListAllocator:
		{
			allocatedPtr_.push_back(
				freeListAllocator_->Allocate(allocationNmb * sizeof(CustomData), alignof(CustomData)));
			break;
		}
		case PoolAllocator:
		{
			for (int i = 0; i < allocationNmb; i++)
			{
				allocatedPtr_.push_back(poolAllocator_->Allocate(sizeof(CustomData), alignof(CustomData)));
			}
			break;
		}
		default:
			break;
		}
	}
	if (ImGui::Button("Deallocate"))
	{
		switch (currentCustomAllocatorType)
		{
		case StackAllocator:
		{
			for (int i = 0; i < allocationNmb; i++)
			{
				auto it = allocatedPtr_.rbegin();
				if (it == allocatedPtr_.rend())
					break;
				stackAllocator_->Deallocate(*it);
				allocatedPtr_.pop_back();
			}
			break;
		}
		case FreeListAllocator:
		{
			for (int i = 0; i < allocationNmb; i++)
			{
				if (freeListAllocator_->GetUsedMemory() != 0)
				{
					const size_t index = rand() % allocatedPtr_.size();
					freeListAllocator_->Deallocate(allocatedPtr_[index]);
					allocatedPtr_.erase(allocatedPtr_.cbegin() + index);
				}
				else
				{
					break;
				}
			}
			break;
		}
		case PoolAllocator:
		{
			for (int i = 0; i < allocationNmb; i++)
			{
				if (poolAllocator_->GetUsedMemory() != 0)
				{
					const size_t index = rand() % allocatedPtr_.size();
					poolAllocator_->Deallocate(allocatedPtr_[index]);
					allocatedPtr_.erase(allocatedPtr_.cbegin() + index);
				}
				else
				{
					break;
				}
			}
			break;
		}
		default:
			break;

		}
	}
	if (ImGui::Button("Clear"))
	{
		switch (currentCustomAllocatorType)
		{
		case LinearAllocator:
		{
			linearAllocator_->Clear();
			break;
		}
		case StackAllocator:
		{
			std::for_each(allocatedPtr_.rbegin(), allocatedPtr_.rend(),
				[this](void* ptr) { stackAllocator_->Deallocate(ptr); });
			break;
		}
		case FreeListAllocator:
			std::for_each(allocatedPtr_.begin(), allocatedPtr_.end(),
				[this](void* ptr) { freeListAllocator_->Deallocate(ptr); });
			break;
		case PoolAllocator:
			std::for_each(allocatedPtr_.begin(), allocatedPtr_.end(),
				[this](void* ptr) { poolAllocator_->Deallocate(ptr); });
			break;
		default:
			break;
		}
		allocatedPtr_.clear();
	}
	ImGui::End();

}

CustomAllocatorTester::~CustomAllocatorTester()
{
	switch (currentCustomAllocatorType)
	{
	case LinearAllocator:
	{
		linearAllocator_->Clear();
		linearAllocator_ = nullptr;
		break;
	}
	case StackAllocator:
	{
		std::for_each(allocatedPtr_.rbegin(), allocatedPtr_.rend(),
			[this](void* ptr) { stackAllocator_->Deallocate(ptr); });
		stackAllocator_ = nullptr;
		break;
	}
	case FreeListAllocator:
		std::for_each(allocatedPtr_.begin(), allocatedPtr_.end(),
			[this](void* ptr) { freeListAllocator_->Deallocate(ptr); });
		freeListAllocator_ = nullptr;
		break;
	case PoolAllocator:
		std::for_each(allocatedPtr_.begin(), allocatedPtr_.end(),
			[this](void* ptr) { poolAllocator_->Deallocate(ptr); });
		poolAllocator_ = nullptr;
		break;
	default:
		break;
	}
	free(data_);
}
}
