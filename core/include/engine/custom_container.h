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
#include "FreeListAllocator.h" 

class IntArray
{
private:
	int m_length{};
	int* m_data{};

public:
	IntArray() = default;

	IntArray(int length) :
		m_length{ length }
	{
		assert(length >= 0);

		if (length > 0)
			m_data = new int[length] {};
	}

	~IntArray()
	{
		delete[] m_data;
		// we don't need to set m_data to null or m_length to 0 here, since the object will be destroyed immediately after this function anyway
	}

	void erase()
	{
		delete[] m_data;
		// We need to make sure we set m_data to nullptr here, otherwise it will
		// be left pointing at deallocated memory!
		m_data = nullptr;
		m_length = 0;
	}

	int& operator[](int index)
	{
		assert(index >= 0 && index < m_length);
		return m_data[index];
	}

	int getLength() const { return m_length; }

	// reallocate resizes the array.  Any existing elements will be destroyed.  This function operates quickly.
	void reallocate(int newLength)
	{
		// First we delete any existing elements
		erase();

		// If our array is going to be empty now, return here
		if (newLength <= 0)
			return;

		// Then we have to allocate new elements
		m_data = new int[newLength];
		m_length = newLength;
	}

	// resize resizes the array.  Any existing elements will be kept.  This function operates slowly.
	void resize(int newLength)
	{
		// if the array is already the right length, we're done
		if (newLength == m_length)
			return;

		// If we are resizing to an empty array, do that and return
		if (newLength <= 0)
		{
			erase();
			return;
		}

		// Now we can assume newLength is at least 1 element.  This algorithm
		// works as follows: First we are going to allocate a new array.  Then we
		// are going to copy elements from the existing array to the new array.
		// Once that is done, we can destroy the old array, and make m_data
		// point to the new array.

		// First we have to allocate a new array
		int* data{ new int[newLength] };

		// Then we have to figure out how many elements to copy from the existing
		// array to the new array.  We want to copy as many elements as there are
		// in the smaller of the two arrays.
		if (m_length > 0)
		{
			int elementsToCopy{ (newLength > m_length) ? m_length : newLength };

			// Now copy the elements one by one
			for (int index{ 0 }; index < elementsToCopy; ++index)
				data[index] = m_data[index];
		}

		// Now we can delete the old array because we don't need it any more
		delete[] m_data;

		// And use the new array instead!  Note that this simply makes m_data point
		// to the same address as the new array we dynamically allocated.  Because
		// data was dynamically allocated, it won't be destroyed when it goes out of scope.
		m_data = data;
		m_length = newLength;
	}

}


class DynArray {
	int* arr;

	// capacity is the total storage capacity
	int capacity;

	// size is the current number of elements 
	int size;

public:

	DynArray()
	{
		arr = new int[1];
		capacity = 1;
		size = 0;
	}

	void Push(int data)
	{
		if (size == capacity) {
			int* temp = new int[2 * capacity];

			for (int i = 0; i < capacity; i++) {
				temp[i] = arr[i];
			}

			delete[] arr;
			capacity *= 2;
			arr = temp;
		}

		arr[size] = data;
		size++;
	}

	void Push(int data, int index)
	{
		if (index == capacity)
			Push(data);
		else
			arr[index] = data;
	}

	int Get(int index)
	{
		if (index < size)
			return arr[index];
	}

	int Size()
	{
		return size;
	}

	int GetCapacity()
	{
		return capacity;
	}

	void Print()
	{
		for (int i = 0; i < size; i++) {
			cout << arr[i] << " ";
		}
		cout << endl;
	}
};