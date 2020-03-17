#include <vector>
#include "utilities/custom_queue.h"
#include <queue>

namespace neko
{
	template<typename T>
	std::vector<T> custom_queue<T>::CreateQueue()
	{
		return std::vector<T>();
	}
	template<typename T>
	void custom_queue<T>::PushQueue(std::vector<T> queue, T element)
	{
		queue.push_back(element);
	}
	template<typename T>
	void custom_queue<T>::DeleteQueue(std::vector<T> queue)
	{
		queue._Destroy();
	}
	template <typename T>
	T custom_queue<T>::PopQueue(std::vector<T> queue)
	{
		//todo
	}

	template<typename T>
	T custom_queue<T>::FrontQueue(std::vector<T> queue)
	{
		return queue.begin();
	}

	template<typename T>
	T custom_queue<T>::BackQueue(std::vector<T> queue)
	{
		return queue.end();
	}

	template<typename T>
	int custom_queue<T>::SizeQueue(std::vector<T> queue)
	{
		return queue.size();
	}
	
	
}
