#include <vector>
#include "utilities/custom_queue.h"
#include <queue>

namespace neko
{

	template<typename T>
	CustomQueue<T>::CustomQueue()
	{
		queueVector.resize(0);
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
