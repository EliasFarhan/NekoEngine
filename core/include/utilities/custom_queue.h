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
		
	};
	//TODO Change name to DynamicQueue and use dynamic array
	template<typename T>
	class DynamicQueue
	{
		
	public:
		DynamicQueue();
		~DynamicQueue() = default;
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