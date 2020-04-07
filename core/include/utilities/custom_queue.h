#pragma once
#include <vector>

namespace neko
{
	template<typename T, size_t capacity>
	class SmallQueue
	{
	private:
		T* start_ = nullptr, end_ = nullptr;
		std::array<T, capacity> buffer_;
	};

	template<typename T, size_t capacity>
	class FixedQueue
	{
	public:
		FixedQueue();
		~FixedQueue() = default;
		void Push(T element); //need
		void Clear(); //need
		T Pop(); //need
		T Front(); //need
		T Back(); //need
		int Size(); //useful ?
	private:
		std::allocator<int> allocator;
	};
	//TODO Change name to DynamicQueue and use dynamic array
	template<typename T>
	class CustomQueue
	{
		
	public:
		CustomQueue();
		~CustomQueue() = default;
		void Push(T element);
		void Clear();
		T Pop();
		T Front();
		T Back();
		int Size();
	private:
		std::vector<T> queueVector;
	};

}