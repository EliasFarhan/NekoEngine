#pragma once

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

#include <memory>
#include "engine/assert.h"
#include <imgui.h>
#include <vector>

namespace neko
{

class Allocator
{
public:


	Allocator(size_t size, void* start)
	{
		start_ = start;
		size_ = size;
	}

	virtual ~Allocator()
	{
		neko_assert(numAllocations_ == 0 && usedMemory_ == 0, "Allocator should be emptied before destruction");
		start_ = nullptr;
		size_ = 0;
	}

	virtual void* Allocate(size_t allocatedSize, size_t alignment) = 0;

	virtual void Deallocate(void* p) = 0;

	inline static size_t CalculateAlignForwardAdjustment(const void* address, size_t alignment)
	{
		//Check if alignement is power of 2
		neko_assert((alignment & (alignment - 1)) == 0, "Alignement needs to be a power of two");
		const size_t adjustment = alignment - ((std::size_t) const_cast<void*>(address) & ((alignment - 1)));

		if (adjustment == alignment) return 0;
		return adjustment;
	}

	inline static size_t
		CalculateAlignForwardAdjustmentWithHeader(const void* address, size_t alignment, size_t headerSize)
	{
		auto adjustment = CalculateAlignForwardAdjustment(address, alignment);
		size_t neededSpace = headerSize;
		if (adjustment < neededSpace)
		{
			neededSpace -= adjustment;
			adjustment += alignment * (neededSpace / alignment);
			if (neededSpace % alignment > 0) adjustment += alignment;
		}
		return adjustment;
	}

	inline static void* AlignForward(void* address, size_t alignment)
	{
		return (void*)((std::uint64_t) address + CalculateAlignForwardAdjustment(address, alignment));
	}

	inline static void* AlignForwardWithHeader(void* address, size_t alignment, size_t headerSize)
	{
		return (void*)((std::uint64_t) address +
			CalculateAlignForwardAdjustmentWithHeader(address, alignment, headerSize));
	}

	size_t GetUsedMemory() const
	{
		return usedMemory_;
	}

	void* GetStart() const
	{
		return start_;
	}

	size_t GetSize() const
	{
		return size_;
	}


	virtual void Profile() = 0;

protected:
	void* start_ = nullptr;
	size_t size_ = 0;
	size_t usedMemory_ = 0;
	size_t numAllocations_ = 0;
};

class LinearAllocator : public Allocator
{
public:
	LinearAllocator(size_t size, void* start) : Allocator(size, start), currentPos_(start_)
	{
		neko_assert(size > 0, "Linear Allocator cannot be empty");
	}

	~LinearAllocator() override
	{
		currentPos_ = nullptr;
	}

	LinearAllocator(const LinearAllocator&) = delete;

	LinearAllocator& operator=(const LinearAllocator&) = delete;

	void* Allocate(size_t allocatedSize, size_t alignment) override;

	void Deallocate(void* p) override;

	void Clear();

	void Profile() override;
protected:
	void* currentPos_ = nullptr;
};

class StackAllocator : public Allocator
{
public:
	StackAllocator(size_t size, void* start) : Allocator(size, start), currentPos_(start)
	{
		neko_assert(size > 0, "Stack Allocator cannot be empty");
	}

	~StackAllocator() override
	{
		currentPos_ = nullptr;
#if defined(NEKO_ASSERT)
		prevPos_ = nullptr;
#endif
	}

	StackAllocator(const StackAllocator&) = delete;

	StackAllocator& operator=(const StackAllocator&) = delete;

	void* Allocate(size_t allocatedSize, size_t alignment) override;

	void Deallocate(void* p) override;

	void Profile() override;

	struct AllocationHeader
	{
#if defined(NEKO_ASSERT)
		void* prevPos = nullptr;
#endif
		std::uint8_t adjustment = 0;
	};

protected:
	void* currentPos_ = nullptr;
#if defined(NEKO_ASSERT)
	void* prevPos_ = nullptr;
#endif

};

class FreeListAllocator : public Allocator
{
public:
	FreeListAllocator(size_t size, void* start) : Allocator(size, start), freeBlocks_((FreeBlock*)start)
	{
		neko_assert(size > sizeof(FreeBlock), "Free List Allocator cannot be empty");
		freeBlocks_->size = size;
		freeBlocks_->next = nullptr;
	}

	~FreeListAllocator() override
	{
		freeBlocks_ = nullptr;
	}

	FreeListAllocator(const FreeListAllocator&) = delete;

	FreeListAllocator& operator=(const FreeListAllocator&) = delete;

	void* Allocate(size_t allocatedSize, size_t alignment) override;

	void Deallocate(void* p) override;


	void Profile() override;
protected:
	struct AllocationHeader
	{
		size_t size = 0;
		std::uint8_t adjustment = 0;
	};
	struct FreeBlock
	{
		size_t size = 0;
		FreeBlock* next = nullptr;
	};

	FreeBlock* freeBlocks_ = nullptr;
};

template<typename T>
class PoolAllocator : public Allocator
{
	static_assert(sizeof(T) >= sizeof(void*));
public:
	PoolAllocator(size_t size, void* mem);

	~PoolAllocator() override
	{
		freeBlocks_ = nullptr;
	}

	void* Allocate(size_t allocatedSize, size_t alignment) override;

	void Deallocate(void* p) override;

	void Profile() override;
protected:
	struct FreeBlock
	{
		FreeBlock* next = nullptr;
	};
	FreeBlock* freeBlocks_ = nullptr;
};

template<typename T>
PoolAllocator<T>::PoolAllocator(size_t size, void* mem) : Allocator(size, mem)
{
	const auto adjustment = CalculateAlignForwardAdjustment(mem, alignof(T));
	freeBlocks_ = (FreeBlock*)((std::uint64_t) mem + adjustment);
	FreeBlock* freeBlock = freeBlocks_;
	size_t numObjects = (size - adjustment) / sizeof(T);
	for (size_t i = 0; i < numObjects - 1; i++)
	{
		freeBlock->next = (FreeBlock*)((std::uint64_t) freeBlock + sizeof(T));
		freeBlock = freeBlock->next;
	}
	freeBlock->next = nullptr;
}

template<typename T>
void* PoolAllocator<T>::Allocate(size_t allocatedSize, [[maybe_unused]] size_t alignment)
{
	neko_assert(allocatedSize == sizeof(T) && alignment == alignof(T), "Pool Allocator can only allocate one Object pooled at once");
	if (freeBlocks_ == nullptr)
	{
		neko_assert(false, "Pool Allocator is full");
	}
	void* p = freeBlocks_;
	freeBlocks_ = freeBlocks_->next;
	usedMemory_ += allocatedSize;
	numAllocations_++;
	return p;
}

template<typename T>
void PoolAllocator<T>::Deallocate(void* p)
{
	FreeBlock* freeBlock = (FreeBlock*)p;
	freeBlock->next = freeBlocks_;
	freeBlocks_ = freeBlock;
	usedMemory_ -= sizeof(T);
	numAllocations_--;
}

template <typename T>
void PoolAllocator<T>::Profile()
{
	const float totalWidth = ImGui::GetContentRegionAvailWidth();
	ImGui::LabelText("Type", "Pool Allocator");
	const auto adjustment = CalculateAlignForwardAdjustment(start_, alignof(T));
	static std::vector<char> availablePool((size_ - adjustment) / sizeof(T));
	std::fill(availablePool.begin(), availablePool.end(), true);
	const T* firstBlock = (T*)((std::uint64_t)start_ + adjustment);
	FreeBlock* freeBlock = freeBlocks_;
	while (freeBlock != nullptr)
	{
		const size_t index = (T*)freeBlock - firstBlock;
		availablePool[index] = false;
		freeBlock = freeBlock->next;
	}
	bool selected = false;
	ImGui::Selectable("Adjustement", &selected, 0, ImVec2(totalWidth * adjustment / size_, 20));

	for (size_t i = 0; i < availablePool.size(); i++)
	{
		ImGui::SameLine();
		selected = availablePool[i];
		ImGui::PushID(i);
		ImGui::Selectable(selected ? "Used" : "Free", &selected, 0, ImVec2(totalWidth * sizeof(T) / size_, 0));
		ImGui::PopID();
	}
}

class ProxyAllocator : public Allocator
{
public:
	explicit ProxyAllocator(Allocator& allocator) :
		Allocator(allocator.GetSize(), allocator.GetStart()), allocator_(allocator)
	{

	}

	~ProxyAllocator() override = default;

	void* Allocate(size_t allocatedSize, size_t alignment) override;

	void Deallocate(void* p) override;

	void Profile() override;
protected:
	Allocator& allocator_;
};
}