#pragma once
#include <vector>

namespace neko
{
	template<typename T>
	class CustomQueue
	{
		
	public:
		CustomQueue();
		~CustomQueue() = default;
		void PushQueue(T element);
		void DeleteQueue();
		T PopQueue();
		T FrontQueue();
		T BackQueue();
		int SizeQueue();
	private:
		std::vector<T> queueVector;
	};

}