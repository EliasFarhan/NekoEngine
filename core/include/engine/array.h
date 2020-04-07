#pragma once
/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <iostream>
#include <engine/custom_allocator.h>
#include <engine/assert.h>

namespace neko
{

template<typename T>
class SmallVector
{
private:
	size_t length_{};
	T* data_{};

public:
	typedef T* iterator;
	typedef const T* const_iterator;

	SmallVector() = default;

	SmallVector(size_t length) : length_{ length }
	{
		neko_assert(length_ >= 0, "[Error] Small Vector initialized with negative size");

		if (length_ > 0)
			data_ = new T[length] {};
	}

	~SmallVector()
	{
		delete[] data_;
	}

	T& operator[](int index)
	{
		neko_assert(index >= 0 && index < length_, "[Error] Out of scope access");
		return data_[index];
	}

	int GetLength() const { return length_; }
};

template<typename T>
class DynArray
{
	FreeListAllocator& allocator_; //Please don't use Linear or Stack Allocator
	T* buffer_ = nullptr;

	// capacity is the total storage capacity
	size_t capacity = 0;

	// size is the current number of elements 
	size_t size = 0;

public:
    typedef T* iterator;
    typedef const T* const_iterator;

	DynArray(FreeListAllocator& allocator) : allocator_(allocator)
	{
		capacity = 0;
		size = 0;
	}
	~DynArray(){
	    allocator_.Deallocate(buffer_);
	}

    iterator begin(){
	    return (std::uint64_t)buffer_;
	}

    iterator end(){
        return buffer_ + static_cast<T*>(size);
    }

	T& operator[](size_t index)
	{
		neko_assert(index >= 0 && index < size, "[Error] Out of scope access");
		return buffer_[index];
	}
	void Push(T elem)
	{
	    if (buffer_ == nullptr){
	        capacity = 2;
	        buffer_ = static_cast<T*>(allocator_.Allocate(capacity, alignof(T)));
	        buffer_[0] = elem;
	        size++;
	    }else{
	        if (size + 1 > capacity){
                T temp[size];
                memcpy(temp, buffer_, size);

                allocator_.Deallocate(buffer_);
                capacity *= 2;
                allocator_.Allocate(capacity, alignof(T));
                for (int i = 0; i < size; ++i)
                {
					buffer_[i] = temp[i];
                }
                buffer_[size++] = elem;
            }else{
	            buffer_[size++] = elem;
	        }

	    }

		if (size == capacity) {
			T* temp = static_cast<T*>(allocator_.Allocate(2*capacity, alignof(T)));

			for (size_t i = 0; i < size; i++) {
				temp[i] = buffer_[i];
			}
			allocator_.Deallocate(buffer_);
			capacity *= 2;
			buffer_ = temp;
		}

		buffer_[size] = elem;
		size++;
	}

	void Insert(T data, size_t index)
	{
		if (index == capacity)
			Push(data);
		else
			buffer_[index] = data;
	}

	size_t Size() const
	{
		return size;
	}

	size_t Capacity() const
	{
		return capacity;
	}

};

}
