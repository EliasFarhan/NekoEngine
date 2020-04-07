#pragma once
#include <vector>
#include "engine/assert.h"

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

		SmallQueue() = default;

		~SmallQueue() = default;

		void Clear()
		{
			
			end_ = 0;
		}

		void PushBack(T element)
		{
			neko_assert(end_ <= capacity, "end_ index is not possible")
			neko_assert(end_ >= 0, "")

			buffer_[end_] = element;

			if (end_ < capacity)
			{
				if (end_ + 1 != start_)
				{
					end_++;
				}
				else
				{
					neko_assert(end_ + 1 != start_, "end_ idx cant be equale to start_ idx")
				}
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
		T Pop()
		{
			T pop = buffer_[start_];
			if (start_ < capacity)
			{
				start_++;
			}
			else
			{
				start_ = 0;
			};
			return pop;
		}
		T Begin()
		{
			return buffer_[start_];
		}
		T End()
		{
			return buffer_[end_ - 1];
		}
		T& operator[](size_t idx)
		{
			idx += start_;
			if (idx < capacity) 
			{
				return buffer_[idx];
			}
			else
			{
				 idx -= capacity;
				 return buffer_[idx];
			}
		}

	private:
		size_t start_ = 0, end_ = 0;
		std::array< T, capacity> buffer_;
	};


	template<typename T>
	class DynamicQueue
	{

	public:
		DynamicQueue()
		{
			queueVector.resize(1);
		}
		~DynamicQueue() = default;
		void PushBack(T element)
		{
			/*if (end_ >= queueVector.size())
			{
				queueVector.resize(queueVector.size()*2);
			}

			queueVector[end_] = element;
			end_++;*/
			if (start_ - end_ + 1 >= queueVector.size)
			{
				queueVector.resize(start_ - end_ + 1);
			}
			if (end_ - start_ + 1 >= queueVector.size)
			{
				queueVector.resize(end_ - start_ + 1);
			}
			queueVector[end_] = element;
			if (end_ + 1 != start_)
			{
				end_++;
			}
			
			
		}
		void Clear()
		{
			queueVector.clear();
		}
		T Pop()
		{
			T pop = queueVector[start_];
			start_++;
			if (start_ > queueVector.size)
			{
				start_ = 0;
			}
			return pop;
		}
		T Begin()
		{
			return queueVector[start_];
		}
		T End()
		{
			return queueVector[end_ - 1];
		}
		int Size()
		{
			return end_ - start_ + 1;
		}
		void Rebase()
		{
			for (int i = 0; i < Size(); i++)
			{
				queueVector[i] = queueVector[start_ + i];
			}
			end_ = Size();
			queueVector.resize(end_);
			start_ = 0;
		}
		T& operator[](size_t idx)
		{
			idx += start_;
			return queueVector[idx];
		}

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
			queueVector.clear();
		};
		T Pop()
		{
			T pop = queueVector[0];
			for (int i = 0; i < Size(); i++)
			{
				queueVector[i] = queueVector[i+1];
			}
			
			return pop;
		};
		T Begin()
		{
			return queueVector[0];
		};
		T End()
		{
			return queueVector[Size()];
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
