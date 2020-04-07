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
	size_t size_ = 0;
	T* data_ = nullptr;;

public:
	typedef T* iterator;
	typedef const T* const_iterator;

	SmallVector() = default;

	SmallVector(size_t size) : size_{ size }
	{
		neko_assert(size_ <= 0, "[Error] Small Vector initialized with negative size");

		if (size_ > 0)
			data_ = new T[size] {};
	}

	~SmallVector()
	{
		delete[] data_;
	}

	T& operator[](int index)
	{
		neko_assert(index >= 0 && index < size_, "[Error] Out of scope access");
		return data_[index];
	}

	int GetSize() const { return size_; }
};

template<typename T>
class DynArray
{
private:
	FreeListAllocator& allocator_ = FreeListAllocator();
	size_t capacity_ = 0;
	size_t size_ = 0;
	T* data_ = nullptr;

public:
    typedef T* iterator;
    typedef const T* const_iterator;

	DynArray(FreeListAllocator& allocator) : allocator_(allocator){
	}
	
	~DynArray(){
	    allocator_.Deallocate(data_);
	}

    iterator begin(){
	    return (std::uint64_t)data_;
	}
	
    iterator end(){
        return data_ + static_cast<T*>(size_);
    }
	

	T& operator[](size_t index){
		neko_assert(index >= 0 && index < size_, "[Error] Out of scope access");
		return data_[index];
	}
	
	void Push(T elem){
	    if (data_ == nullptr){
	        capacity_ = 2;
	        data_ = (T*)(allocator_.Allocate(sizeof(T) * capacity_, alignof(T)));
	        data_[0] = elem;
	        size_++;
	    }else{
			if (size_ + 1 > capacity_) {
				SmallVector<T> temp(size_);
				for (int i = 0; i < size_; ++i){
					temp[i] = data_[i];
				}

                allocator_.Deallocate(data_);
                capacity_ *= 2;
                data_ = (T*)allocator_.Allocate(sizeof(T)*capacity_, alignof(T));
	        	
				for (int i = 0; i < size_; ++i){
					data_[i] = temp[i];
				}
	        	
                data_[size_+1] = elem;
				size_++;
            }else{			
	            data_[size_+1] = elem;
				size_++;
	        }
	    }
	}

	void Insert(T data, size_t index){
		if (index == capacity_)
			Push(data);
		else{
			for (int i = size_; i == index; i--){
				data_[i] = data_[i - 1];
			}
			data_[index] = data;
			size_++;
		}
	}

	size_t Size() const{
		return size_;
	}

	size_t Capacity() const{
		return capacity_;
	}
};

}
