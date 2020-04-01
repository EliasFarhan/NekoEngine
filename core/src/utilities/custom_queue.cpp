#include <vector>
#include "utilities/custom_queue.h"
#include <queue>

namespace neko
{

	template<typename T>
	DynamicQueue<T>::DynamicQueue()
	{
		queueVector.resize(0);
	}

	template<typename T>
	void DynamicQueue<T>::Push(T element)
	{
		queueVector.push_back(element);
	}
	template<typename T>
	void DynamicQueue<T>::Clear()
	{
		queueVector._Destroy();
	}
	template <typename T>
	T DynamicQueue<T>::Pop()
	{
		T returnValue = queueVector[0];
		queueVector.erase(0);
		return returnValue;
	}

	template<typename T>
	T DynamicQueue<T>::Front()
	{
		return queueVector.begin();
	}

	template<typename T>
	T DynamicQueue<T>::Back()
	{
		return queueVector.end();
	}

	template<typename T>
	int DynamicQueue<T>::Size()
	{
		return queueVector.size();
	}
	
	
}
