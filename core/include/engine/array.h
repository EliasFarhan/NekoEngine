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
		size_t capacity_=0;
		size_t size_ = 0;
		T* data_ = nullptr;;

	public:
		
		typedef T* iterator;
		typedef const T* const_iterator;

		SmallVector() = default;

		SmallVector(size_t capacity) : capacity_{ capacity }{
			neko_assert(capacity >= 0, "[Error] Small Vector initialized with negative size");

			if (capacity_ > 0)
				data_ = new T[capacity_]{};
		}

		~SmallVector(){
			delete[] data_;
		}

		T& operator[](int index){
			neko_assert(index >= 0 && index < size_, "[Error] Out of scope access");
			return data_[index];
		}

		void Push(T elem) {
			if (size_ + 1 > capacity_) {
				neko_assert(size_ + 1 <= capacity_, "[Error] Out of scope access");
			}
			else {
				data_[size_] = elem;
				size_++;
			}	
		}
		
		size_t GetSize() const { return size_; }
	};

	template<typename T>
	class DynArray
	{
	private:
		FreeListAllocator& allocator_;
		size_t capacity_ = 0;
		size_t size_ = 0;
		T* data_ = nullptr;

	public:

		class iterator
		{
		public:
			typedef iterator self_type;
			typedef T value_type;
			typedef T& reference;
			typedef T* pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef int difference_type;
			iterator(pointer ptr) : ptr_(ptr) { }
			self_type operator++() { self_type i = *this; ptr_++; return i; }
			self_type operator++(int junk) { ptr_++; return *this; }
			reference operator*() { return *ptr_; }
			pointer operator->() { return ptr_; }
			bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
			bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
		private:
			pointer ptr_;
		};

		DynArray(FreeListAllocator& allocator) : allocator_(allocator) {
		}

		~DynArray() {
			allocator_.Deallocate(data_);
		}

		iterator begin() {
			return iterator(data_);
		}

		iterator end() {
			return iterator(data_ + size_);
		}


		T& operator[](size_t index) {
			neko_assert(index >= 0 && index < size_, "[Error] Out of scope access");
			return data_[index];
		}
		//Always use push to add new elements
		void Push(T elem) {
			if (data_ == nullptr) {
				capacity_ = 2;
				data_ = (T*)(allocator_.Allocate(sizeof(T) * capacity_, alignof(T)));
				data_[0] = elem;
				size_++;
			}
			else {
				if (size_ + 1 > capacity_) {

					allocator_.Deallocate(data_);
					capacity_ *= 2;
					data_ = (T*)allocator_.Allocate(sizeof(T) * capacity_, alignof(T));

					data_[size_] = elem;
					size_++;
				}
				else {
					data_[size_] = elem;
					size_++;
				}
			}
		}

		void Insert(T data, size_t index) {
			if (index == capacity_)
				Push(data);
			else {
				for (int i = size_; i == index; i--) {
					data_[i] = data_[i - 1];
				}
				data_[index] = data;
				size_++;
			}
		}

		size_t GetSize() const {return size_;}

		size_t GetCapacity() const {return capacity_;}
	};

	template<typename T>
	class FixedVector
	{
	private:
		size_t capacity_ = 0;
		size_t size_ = 0;
		FreeListAllocator& allocator_;
		T* data_ = nullptr;;

	public:

		typedef T* iterator;
		typedef const T* const_iterator;

		FixedVector(size_t capacity, FreeListAllocator& allocator) : capacity_{ capacity }, allocator_ {allocator}
		{
			neko_assert(capacity_ >= 0, "[Error] Fixed Vector initialized with negative size");

			if (capacity_ > 0)
				data_ = new T[capacity_]{};
		}

		~FixedVector()
		{
			delete[] data_;
		}

		T& operator[](int index)
		{
			neko_assert(index >= 0 && index < size_, "[Error] Out of scope access");
			return data_[index];
		}

		void Push(T elem) {
			if (data_ == nullptr) {

				data_ = (T*)(allocator_.Allocate(sizeof(T) * capacity_, alignof(T)));
				data_[0] = elem;
				size_++;
			}
			else {
				if (size_ + 1 > capacity_) {
					neko_assert(size_ + 1 <= capacity_, "[Error] Out of scope access");
				}
				else {
					data_[size_] = elem;
					size_++;
				}
			}
		}
		
		size_t GetCapacity() const {return capacity_;}

		size_t GetSize() const { return size_; }
	};
}