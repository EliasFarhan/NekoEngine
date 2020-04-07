#include <vector>
#include "utilities/custom_queue.h"
#include <queue>

namespace neko
{
	template<typename T>
	void FixedQueue<T>::Push(T element)
	{
		allocator.push_back(element);
	}
	template<typename T>
	void FixedQueue<T>::Clear()
	{
		allocator._Destroy();
	}
	template <typename T>
	T FixedQueue<T>::Pop()
	{
		T returnValue = queueVector[0];
		allocator.erase(0);
		return returnValue;
	}

	template<typename T>
	T FixedQueue<T>::Front()
	{
		return allocator.begin();
	}

	template<typename T>
	T FixedQueue<T>::Back()
	{
		return allocator.end();
	}

	template<typename T>
	int FixedQueue<T>::Size()
	{
		return allocator.size();
	}
	
	template<typename T>
	void CustomQueue<T>::Push(T element)
	{
		queueVector.push_back(element);
	}
	template<typename T>
	void CustomQueue<T>::Clear()
	{
		queueVector._Destroy();
	}
	template <typename T>
	T CustomQueue<T>::Pop()
	{
		T returnValue = queueVector[0];
		queueVector.erase(0);
		return returnValue;
	}

	template<typename T>
	T CustomQueue<T>::Front()
	{
		return queueVector.begin();
	}

	template<typename T>
	T CustomQueue<T>::Back()
	{
		return queueVector.end();
	}

	template<typename T>
	int CustomQueue<T>::Size()
	{
		return queueVector.size();
	}


}
