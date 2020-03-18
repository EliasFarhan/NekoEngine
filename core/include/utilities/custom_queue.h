#pragma once
#include <vector>

namespace neko
{
	template<typename T>
	class custom_queue
	{

	public:
		custom_queue();
		~custom_queue() = default;
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