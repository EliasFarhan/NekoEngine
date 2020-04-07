#pragma once
#include <vector>

namespace neko
{
	template<typename T, size_t capacity>
	class FixedQueue
	{
		// Tristan
	};
	template<typename T, size_t capacity>
	class SmallQueue
	{
	public:

		SmallQueue();

		~SmallQueue();

		void Clear()
		{
			for (int i = 0; i <= capacity; i++)
			{
				buffer_[i] = nullptr;
			}

		};
		void PushBack(T element)
		{
			if (end_ <= capacity)
			{

				buffer_[end_] = element;

				if (end_ < capacity)
				{
					end_++;
				}
				else
				{
					if (start_ != 0)
					{
						end_ = 0;
					}
					else
					{
						end_ = -1;
					}
				}
			}

		};
		T Pop()
		{
			T pop = buffer_[start_];
			if (start_ < capacity)
			{
				start_++;
			}
			else
			{
				if (end_ != start_)
				{
					start_ = 0;
				}

			};
			return pop;
		};
		T Begin()
		{
			return buffer_[start_];
		};
		T End()
		{
			return buffer_[end_ - 1];
		};
		T& operator[](size_t idx)
		{
			return buffer_[idx];
		};

	private:
		size_t start_ = 0, end_ = 0;
		std::array<T, capacity> buffer_;
	};


	template<typename T>
	class DynamicQueue
	{

	public:
		DynamicQueue();
		~DynamicQueue() = default;
		void PushBack(T element)
		{
			queueVector[end_] = element;
			end_++;
		};
		void Clear()
		{
			queueVector.erase();
		};
		T Pop()
		{
			T pop = queueVector[start_];
			start_++;
			return pop;
		};
		T Begin()
		{
			return queueVector[start_];
		};
		T End()
		{
			return queueVector[end_ - 1];
		};
		int Size()
		{
			return end_ - start_ + 1;
		};
		void Resize()
		{
			for (int i = 0; i++; i < Size())
			{
				queueVector[i] = queueVector[start_ + i];
			}
			end_ = Size();
			start_ = 0;
		};
		T& operator[](size_t idx)
		{
			return queueVector[idx];
		};

	private:
		size_t start_ = 0, end_ = 0;
		std::vector<T> queueVector;
	};


	template<typename T>
	class DynamicQueue2
	{

	public:
		DynamicQueue2();
		~DynamicQueue2() = default;
		void PushBack(T element)
		{
			queueVector.push_back(element);
		};
		void Clear()
		{
			queueVector.erase();
		};
		T Pop()
		{
			T pop = queueVector[Size()];
			queueVector.erase(Size());
			return pop;
		};
		T Begin()
		{
			return queueVector[Size()];
		};
		T End()
		{
			return queueVector[0];
		};
		int Size()
		{
			return queueVector.size();
		};
		T& operator[](size_t idx)
		{
			return queueVector[idx];
		};
	private:
		std::vector<T> queueVector;
	};
}
