#include "gtest/gtest.h"
#include <engine/array.h>

namespace neko {

	TEST(Array, Iterators) {
		// TODO

		// std::find_if(arra.begin(), arra.end(). []{})
	}

	TEST(Array, DynArrays) {

		const size_t ALLOCATOR_HEADER_SIZE = 16;
		const size_t SIZE = 64;
		void* start = malloc(SIZE + ALLOCATOR_HEADER_SIZE);

		FreeListAllocator allocator(SIZE + ALLOCATOR_HEADER_SIZE, start);
		DynArray<double> doubleArray(allocator);


		doubleArray.Push(0.1 + 62);
		doubleArray.Push(0.1 + 52);
		doubleArray.Push(0.1 + 12);
		
		EXPECT_EQ(doubleArray[1], 0.1 + 52);
	}

	/*
	TEST(Array, SmallVectorOperator) {
		SmallVector<double> intVec(5);
		intVec[4] = 18.58484845;
		EXPECT_EQ(intVec[4], 18.58484845);

	}
	*/

}// !neko