#pragma once
#include <vector>
#include <array>
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
			start_ = 0;
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
		T Front()
		{
			return buffer_[start_];
		}
		T Back()
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
		DynamicQueue()= default;
		
		~DynamicQueue() = default;
		
		void PushBack(T element)
		{		
			if (Size() >= queueVector.capacity())
			{
				Rebase();
				queueVector.resize(Size());
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
			start_ = 0;
			end_ = 0;
		}
		T Pop()
		{
			T pop = queueVector[start_];
			start_++;
			if (start_ > queueVector.capacity())
			{
				start_ = 0;
			}
			return pop;
		}
		T Front()
		{
			return queueVector[start_];
		}
		T Back()
		{
			if (end_ != 0)
			{
				return queueVector[end_ - 1];
			}
			
			return queueVector[end_];
			
			
		}
		int Size()
		{
			if (end_ > start_)
			{
				return end_ - start_ + 1;
			}
			else {
				return   start_ - end_ + 1;
			}
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
	class DynamicQueueBasic
	{

	public:
		DynamicQueueBasic() = default;
		~DynamicQueueBasic() = default;
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
		T Front()
		{
			return queueVector[0];
		};
		T Back()
		{
			return queueVector[Size()-1];
		};
		int Size()
		{
			return queueVector.capacity();
		};
		T& operator[](size_t idx)
		{
			return queueVector[idx];
		};
	private:
		std::vector<T> queueVector;
	};
}
