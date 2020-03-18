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
	void CustomQueue<T>::PushQueue(T element)
	{
		queueVector.push_back(element);
	}
	template<typename T>
	void CustomQueue<T>::DeleteQueue()
	{
		queueVector._Destroy();
	}
	template <typename T>
	T CustomQueue<T>::PopQueue()
	{
		T returnValue = queueVector[0];
		queueVector.erase(0);
		return returnValue;
	}

	template<typename T>
	T CustomQueue<T>::FrontQueue()
	{
		return queueVector.begin();
	}

	template<typename T>
	T CustomQueue<T>::BackQueue()
	{
		return queueVector.end();
	}

	template<typename T>
	int CustomQueue<T>::SizeQueue()
	{
		return queueVector.size();
	}


}
