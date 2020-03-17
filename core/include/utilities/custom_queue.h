#include <vector>

namespace neko
{
	template<class ... T>
	class custom_queue
	{
	public:
		std::vector<T> CreateQueue();
		void PushQueue(std::vector<T> queue, T element);
		void DeleteQueue(std::vector<T> queue);
		T PopQueue(std::vector<T> queue);
		T FrontQueue(std::vector<T> queue);
		T BackQueue(std::vector<T> queue);
		int SizeQueue(std::vector<T> queue);
	private:
		custom_queue MyQueue;
	};

}