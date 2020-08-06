/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "engine/custom_allocator.h"

namespace neko
{

void* LinearAllocator::Allocate(size_t allocatedSize, size_t alignment)
{
	neko_assert(allocatedSize != 0, "Linear Allocator cannot allocated nothing");
	const auto adjustment = CalculateAlignForwardAdjustment(currentPos_, alignment);

	neko_assert(usedMemory_ + adjustment + allocatedSize < size_, "Linear Allocator has not enough space for this allocation");

	auto* alignedAddress = (void*)((std::uint64_t) currentPos_ + adjustment);
	currentPos_ = (void*)((std::uint64_t) alignedAddress + allocatedSize);
	usedMemory_ += allocatedSize + adjustment;
	numAllocations_++;
	return alignedAddress;
}

void LinearAllocator::Deallocate([[maybe_unused]] void* p)
{
	neko_assert(false, "Use clear() instead");
}

void LinearAllocator::Clear()
{
	numAllocations_ = 0;
	usedMemory_ = 0;
	currentPos_ = start_;
}

void LinearAllocator::Profile()
{
	const float totalWidth = ImGui::GetContentRegionAvailWidth();
	ImGui::LabelText("Type", "Linear Allocator");

	bool selected = true;
	if (usedMemory_ != 0)
	{
		const float usedWidth = usedMemory_ * totalWidth / size_;
		ImGui::Selectable("Used Memory", &selected, 0, ImVec2(usedWidth, 20));
		ImGui::SameLine();
	}
	selected = false;
	const float availableWidth = (size_ - usedMemory_) * totalWidth / size_;
	ImGui::Selectable("Available Memory", &selected, 0, ImVec2(availableWidth, 20));

}

void* StackAllocator::Allocate(size_t allocatedSize, size_t alignment)
{
	neko_assert(allocatedSize != 0, "Stack Allocator cannot allocate nothing");
	const auto adjustment = CalculateAlignForwardAdjustmentWithHeader(currentPos_, alignment, sizeof(AllocationHeader));
	if (usedMemory_ + adjustment + allocatedSize > size_)
	{
		neko_assert(false, "StackAllocator has not enough space for this allocation");
	}

	void* alignedAddress = (void*)((std::uint64_t) currentPos_ + adjustment);

	auto* header = reinterpret_cast<AllocationHeader*>(std::uint64_t(alignedAddress) - sizeof(AllocationHeader));
	header->adjustment = static_cast<std::uint8_t>(adjustment);
#if defined(NEKO_ASSERT)
	header->prevPos = prevPos_;
	prevPos_ = alignedAddress;
#endif
	currentPos_ = (void*)((std::uint64_t) alignedAddress + allocatedSize);
	usedMemory_ += allocatedSize + adjustment;
	numAllocations_++;
	return alignedAddress;

}


void StackAllocator::Deallocate(void* p)
{
	neko_assert(p != nullptr, "Stack allocator requires valid pointer to deallocate");
	neko_assert(p == prevPos_, "Stack allocator needs to deallocate from the top");
	//Access the AllocationHeader in the bytes before p
	auto* header = (AllocationHeader*)((std::uint64_t) p - sizeof(AllocationHeader));
#if defined(NEKO_ASSERT)
	prevPos_ = header->prevPos;
#endif
	usedMemory_ -= (std::uint64_t) currentPos_ - (std::uint64_t) p + header->adjustment;
	currentPos_ = (void*)((std::uint64_t) p - header->adjustment);
	numAllocations_--;
}

void StackAllocator::Profile()
{
	ImGui::LabelText("Type", "Stack Allocator");
	const float totalWidth = ImGui::GetContentRegionAvailWidth();
	ImGui::LabelText("Type", "Linear Allocator");

	bool selected = true;
	if (usedMemory_ != 0)
	{
		const float usedWidth = usedMemory_ * totalWidth / size_;
		ImGui::Selectable("Used Memory", &selected, 0, ImVec2(usedWidth, 20));
		ImGui::SameLine();
	}
	selected = false;
	const float availableWidth = (size_ - usedMemory_) * totalWidth / size_;
	ImGui::Selectable("Available Memory", &selected, 0, ImVec2(availableWidth, 20));
}

void* FreeListAllocator::Allocate(size_t allocatedSize, size_t alignment)
{
	FreeBlock* prevFreeBlock = nullptr;
	FreeBlock* freeBlock = freeBlocks_;
	while (freeBlock != nullptr)
	{
		auto adjustment = CalculateAlignForwardAdjustmentWithHeader(freeBlock, alignment, sizeof(AllocationHeader));
		size_t totalSize = allocatedSize + adjustment;
		if (freeBlock->size < totalSize)
		{
			prevFreeBlock = freeBlock;
			freeBlock = freeBlock->next;
			continue;
		}
		static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

		//If allocation takes the whole freeblock
		if (freeBlock->size - totalSize <= sizeof(AllocationHeader))
		{
			totalSize = freeBlock->size;
			if (prevFreeBlock != nullptr)
			{
				prevFreeBlock->next = freeBlock->next;
			}
			else
			{
				freeBlocks_ = freeBlock->next;
			}
		}
		else
		{
			FreeBlock* nextBlock = (FreeBlock*)((std::uint64_t) freeBlock + totalSize);
			nextBlock->size = freeBlock->size - totalSize;
			nextBlock->next = freeBlock->next;

			if (prevFreeBlock != nullptr)
			{
				prevFreeBlock->next = nextBlock;
			}
			else
			{
				freeBlocks_ = nextBlock;
			}
		}
		void* alignedAddress = (void*)((std::uint64_t) freeBlock + adjustment);
		AllocationHeader* header = (AllocationHeader*)((std::uint64_t) alignedAddress - sizeof(AllocationHeader));
		header->size = totalSize;
		header->adjustment = adjustment;
		usedMemory_ += totalSize;
		numAllocations_++;
		neko_assert(CalculateAlignForwardAdjustment(alignedAddress, alignment) == 0, "Free List Allocator: New generated block is not aligned");
		return alignedAddress;
	}
	neko_assert(false, "FreeList Allocator has not enough space for this allocation");
	return nullptr;
}

void FreeListAllocator::Deallocate(void* p)
{
	neko_assert(p != nullptr, "Free List cannot deallocate nullptr");
	AllocationHeader* header = (AllocationHeader*)((std::uint64_t) p - sizeof(AllocationHeader));
	void* blockStart = (void*)((std::uint64_t) p - header->adjustment);
	size_t blockSize = header->size;
	void* blockEnd = (void*)((std::uint64_t) blockStart + blockSize);
	FreeBlock* prevFreeBlock = nullptr;
	FreeBlock* freeBlock = freeBlocks_;

	while (freeBlock != nullptr)
	{
		if (freeBlock >= blockEnd) break;
		prevFreeBlock = freeBlock;
		freeBlock = freeBlock->next;
	}
	if (prevFreeBlock == nullptr)
	{
		prevFreeBlock = (FreeBlock*)blockStart;
		prevFreeBlock->size = blockSize;
		prevFreeBlock->next = freeBlocks_;
		freeBlocks_ = prevFreeBlock;
	}
	else if ((void*)((size_t)prevFreeBlock + prevFreeBlock->size) == blockStart)
	{
		prevFreeBlock->size += blockSize;
	}
	else
	{
		FreeBlock* temp = (FreeBlock*)blockStart;
		temp->size = blockSize;
		temp->next = prevFreeBlock->next;
		prevFreeBlock->next = temp;
		prevFreeBlock = temp;
	}
	if (freeBlock != nullptr && (void*)freeBlock == blockEnd)
	{
		prevFreeBlock->size += freeBlock->size;
		prevFreeBlock->next = freeBlock->next;
	}
	numAllocations_--;
	usedMemory_ -= blockSize;
}

void FreeListAllocator::Profile()
{
	ImGui::LabelText("Type", "FreeList Allocator");


	const float totalWidth = ImGui::GetContentRegionAvailWidth();
	const auto start = (size_t)start_;
	const auto end = ((size_t)start_ + size_);
	FreeBlock* freeBlock = freeBlocks_;

	while (freeBlock != nullptr)
	{
		bool selected = false;
		const size_t blockBegin = (size_t)freeBlock;
		const size_t blockEnd = blockBegin + freeBlock->size;
		const size_t blockSize = blockEnd - blockBegin;
		if (freeBlock == freeBlocks_ && freeBlock != start_)
		{
			selected = true;
			ImGui::PushID(blockBegin-1);
			ImGui::Selectable("Used Block", &selected, 0, ImVec2(float(blockBegin-start) * totalWidth / size_, 20));
			ImGui::PopID();
			ImGui::SameLine();
		}
		selected = false;
		ImGui::PushID(blockBegin);
		ImGui::Selectable("Free Block", &selected, 0, ImVec2(blockSize * totalWidth / size_, 20));
		ImGui::PopID();


		if (freeBlock->next != nullptr)
		{
			const size_t nextFreeBlock = (size_t)freeBlock->next;
			ImGui::SameLine();
			selected = true;
			ImGui::PushID(blockBegin);
			ImGui::Selectable("Used Block", &selected, 0, ImVec2(totalWidth * float(nextFreeBlock - blockEnd) / size_, 20));
			ImGui::PopID();
			ImGui::SameLine();
		}
		else
		{
			if (end != blockEnd)
			{
				//Adding used block at the end
				ImGui::SameLine();
				selected = true;
				ImGui::PushID(blockBegin);
				ImGui::Selectable("Used Block", &selected, 0, ImVec2(float(end - blockEnd) * totalWidth / size_, 20));
				ImGui::PopID();
			}
		}
		freeBlock = freeBlock->next;
	}
}

void* ProxyAllocator::Allocate(size_t allocatedSize, size_t alignment)
{
	numAllocations_++;
	size_t mem = allocator_.GetUsedMemory();
	void* p = allocator_.Allocate(allocatedSize, alignment);
	usedMemory_ += allocator_.GetUsedMemory() - mem;
	return p;
}

void ProxyAllocator::Deallocate(void* p)
{
	numAllocations_--;
	size_t mem = allocator_.GetUsedMemory();
	allocator_.Deallocate(p);
	usedMemory_ -= mem - allocator_.GetUsedMemory();
}

void ProxyAllocator::Profile()
{
	ImGui::LabelText("Type", "Proxy Allocator");
}
}
