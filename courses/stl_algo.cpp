#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <numeric>

int main(int argc, char** argv)
{
	std::vector<int> numbers = { 1,2,3,4 };
	std::vector<int> numbersPermutation = { 4,3,2,1 };
	std::vector<int> otherNumbers = { 5,7,9,11 };

	//Is any number even
	const auto isEvenFunc = [](int n) { return n % 2 == 0; };
	std::cout << "Is any of numbers even: " <<
		std::any_of(numbers.begin(), numbers.end(), isEvenFunc) << "\n";

	std::cout << "Is any other numbers even: " <<
		std::any_of(otherNumbers.begin(), otherNumbers.end(), isEvenFunc) << "\n";

	//All_of
	std::cout << "Are all of numbers even: " <<
		std::all_of(numbers.begin(), numbers.end(), isEvenFunc) << "\n";
	std::cout << "Are all of other numbers even: " <<
		std::all_of(otherNumbers.begin(), otherNumbers.end(), isEvenFunc) << "\n";

	//None_of
	std::cout << "Are none of numbers even: " <<
		std::none_of(numbers.begin(), numbers.end(), isEvenFunc) << "\n";
	std::cout << "Are none of other numbers even: " <<
		std::none_of(otherNumbers.begin(), otherNumbers.end(), isEvenFunc) << "\n";

	//Is equal
	std::cout << "Is numbers equal to numbers permutation: " <<
		std::equal(numbers.begin(), numbers.end(), numbersPermutation.begin(), numbersPermutation.end()) << "\n";

	//Is permutation
	std::cout << "Is numbers a permutation of numbers permutation: " <<
		std::is_permutation(numbers.begin(), numbers.end(), numbersPermutation.begin(), numbersPermutation.end()) << "\n";
	std::cout << "Is numbers a permutation of other numbers: " <<
		std::is_permutation(numbers.begin(), numbers.end(), otherNumbers.begin(), otherNumbers.end()) << "\n";

	//count the number of occurences 
	std::cout << "There is exactly: " << std::count(numbers.begin(), numbers.end(), 3) << " number 3\n";
	std::cout << "There are exactly " << std::count_if(numbers.begin(), numbers.end(), isEvenFunc) << " even numbers\n";

	//inner product
	std::cout << "The dot product of numbers and other numbers is: " <<
		std::inner_product(numbers.begin(), numbers.end(), otherNumbers.begin(), 0) << "\n";
	//Shuffle
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(numbers.begin(), numbers.end(), g);
	std::cout << "After shuffle: ";
	std::for_each(numbers.begin(), numbers.end(), [](int n) {std::cout << n << ' '; });
	std::cout << "\n";
	//Sort
	std::sort(numbers.begin(), numbers.end());
	std::cout << "After sort: ";
	std::for_each(numbers.begin(), numbers.end(), [](int n) {std::cout << n << ' '; });
	std::cout << "\n";

	//accumulate
	std::cout << "The sum of all values in numbers is: " << std::accumulate(numbers.begin(), numbers.end(), 0) << "\n";
	numbers.resize(10);
	//Fill with a value
	std::fill(numbers.begin(), numbers.end(), 42);
	std::cout << "After fill: ";
	std::for_each(numbers.begin(), numbers.end(), [](int n) {std::cout << n << ' '; });
	std::cout << "\n";
	//Fill with iota
	std::iota(numbers.begin(), numbers.end(), 1);
	std::cout << "After iota: ";
	std::for_each(numbers.begin(), numbers.end(), [](int n) {std::cout << n << ' '; });
	std::cout << "\n";

	//partial sort
	std::shuffle(numbers.begin(), numbers.end(), g);
	std::cout << "After shuffle: ";
	std::for_each(numbers.begin(), numbers.end(), [](int n) {std::cout << n << ' '; });
	std::cout << "\n";
	std::partial_sort(numbers.begin(), numbers.begin() + 5, numbers.end());
	std::cout << "After partial sort: ";
	std::for_each(numbers.begin(), numbers.end(), [](int n) {std::cout << n << ' '; });
	std::cout << "\n";

	//partitioning
	std::sort(numbers.begin(), numbers.end());
	std::partition(numbers.begin(), numbers.end(), isEvenFunc);
	std::cout << "After partitioning even numbers: ";
	std::for_each(numbers.begin(), numbers.end(), [](int n) {std::cout << n << ' '; });
	std::cout << "\n";

	//stable partitioning
	std::sort(numbers.begin(), numbers.end());
	std::stable_partition(numbers.begin(), numbers.end(), isEvenFunc);
	std::cout << "After stable partitioning even numbers: ";
	std::for_each(numbers.begin(), numbers.end(), [](int n) {std::cout << n << ' '; });
	std::cout << "\n";
	//Min Max
	std::shuffle(numbers.begin(), numbers.end(), g);
	std::cout << "After shuffle: ";
	std::for_each(numbers.begin(), numbers.end(), [](int n) {std::cout << n << ' '; });
	std::cout << "\n";
	std::cout << "The max element is at: " <<
		(std::max_element(numbers.begin(), numbers.end()) - numbers.begin()) <<
		" and the min element is at: " <<
		(std::min_element(numbers.begin(), numbers.end()) - numbers.begin()) << "\n";
	//Find the even numbers
	std::cout << "The first even number is at: " <<
		(std::find_if(numbers.begin(), numbers.end(), isEvenFunc) - numbers.begin()) << " the last one at: " <<
		(std::find_if(numbers.rbegin(), numbers.rend(), isEvenFunc) - numbers.rbegin())<<"\n";
	//Reverse
	std::sort(numbers.begin(), numbers.end());
	std::reverse(numbers.begin(), numbers.end());
	std::cout << "Reversing: ";
	std::for_each(numbers.begin(), numbers.end(), [](int n) {std::cout << n << ' '; });
	std::cout << "\n";
	//Rotate
	std::sort(numbers.begin(), numbers.end());
	std::rotate(numbers.begin(), numbers.begin() + 3, numbers.end());
	std::cout << "Rotate 3 numbers: ";
	std::for_each(numbers.begin(), numbers.end(), [](int n) {std::cout << n << ' '; });
	std::cout << "\n";
	//Remove all the even numbers
	std::sort(numbers.begin(), numbers.end());
	numbers.erase(std::remove_if(numbers.begin(), numbers.end(), isEvenFunc), numbers.end());
	std::cout << "After erase and remove: ";
	std::for_each(numbers.begin(), numbers.end(), [](int n) {std::cout << n << ' '; });
	std::cout << "\n";
	return EXIT_SUCCESS;
}
