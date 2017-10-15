#include <iostream>
#include <ctime>
#include <cmath>
#include <functional>
#include <utility>
#include <type_traits>
#include <vector>
#include <string>

constexpr static size_t N = 10;
constexpr static int RANGE = 100;

template<typename T>
void PrintArray(const T array[], size_t size = N)
{
	for (size_t i = 0; i < size; ++i)
		std::cout << array[i] << " ";
	std::cout << std::endl;
}

// bubble sort
template<typename T>
void BubbleSort(T array[])
{
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < N - i - 1; ++j)
			if (array[j] < array[j + 1])
				std::swap(array[j], array[j + 1]);
}

// cocktail sort
template<typename T>
void CocktailSort(T array[])
{
	size_t left = 0, right = N - 1;
	while (left < right)
	{
		for (size_t i = left; i < right; ++i)
			if (array[i] < array[i + 1])
				std::swap(array[i], array[i + 1]);
		--right;

		for (size_t i = right; i > left; --i)
			if (array[i - 1] < array[i])
				std::swap(array[i - 1], array[i]);
		++left;
	}

}

// insertion sort
template<typename T>
void InsertionSort(T array[])
{
	for (size_t i = 1; i < N; ++i)
		for (size_t j = i; j > 0 && array[j - 1] < array[j]; --j)
			std::swap(array[j - 1], array[j]);
}

// gnome sort
template<typename T>
void GnomeSort(T array[])
{
	size_t i = 1, j = i;
	while(i < N)
	{
		if (i != 0 && array[i - 1] < array[i])
		{
			std::swap(array[i - 1], array[i]);
			--i;
		}
		else
			i = ++j;
	}
}

// merge sort
template<typename T>
void MergeSort(T array[])
{
	std::function<T*(T[], size_t)> merge_sort = [&merge_sort](T array[], size_t size) -> T*
	{
		if (size == 1)
			return new T(*array);

		size_t size1 = std::floor(size / 2);
		size_t size2 = size - size1;
		T* array1 = merge_sort(array, size1);
		T* array2 = merge_sort(array + size1, size2);

		T* tmp_array = new T[size];
		size_t l1 = 0;
		size_t l2 = 0;
		for(size_t i = 0; i < size; ++i)
		{
			if (l1 == size1)
			{
				for (size_t j = l2; j < size2; ++j)
					tmp_array[i++] = array2[j];
				break;
			}

			if (l2 == size2)
			{
				for (size_t j = l1; j < size1; ++j)
					tmp_array[i++] = array1[j];
				break;
			}

			if (array1[l1] > array2[l2])
			{
				tmp_array[i] = array1[l1];
				++l1;
			}
			else
			{
				tmp_array[i] = array2[l2];
				++l2;
			}
		}

		delete[] array1, array2;
		return tmp_array;
	};
	
	auto tmp = merge_sort(array, N);
	std::copy(tmp, tmp + N, array);
	delete[] tmp;
}

template<typename T>
T FindMax(const T array[])
{
	T max = array[0];
	for (size_t i = 1; i < N; ++i)
		if (array[i] > max)
			max = array[i];
	return max;
}

template<typename T>
T FindMin(const T array[])
{
	T min = array[0];
	for (size_t i = 1; i < N; ++i)
		if (array[i] < min)
			min = array[i];
	return min;
}

// counting sort
template<typename T>
void CountingSort(T array[])
{
	static_assert(std::is_integral<T>::value, "Only integral types");

	const T min = FindMin(array);
	const T max = FindMax(array) - min + 1;
	int* counting = new int[max] {};
	for (size_t i = 0; i < N; ++i)
		++counting[array[i] - min];
	
	int pos = 0;
	for (int i = max - 1; i >= 0; --i) // suddenly, size_t doesn't work in this case
		while(counting[i]-- > 0)
			array[pos++] = i + min;
	
	delete[] counting;
}

template<typename T>
int GetDigit(T number, int place)
{
	std::string tmp = std::to_string(number);
	if (tmp.size() < place)
		return static_cast<int>(tmp[place]);
	return -1;
	//return std::floor( number % static_cast<int>(std::pow(10, place)) / 10 );
}

// radix sort
template<typename T>
void RadixSort(T array[])
{
	static_assert(std::is_integral<T>::value, "Only integral types");

	std::vector<T> ranges[10];
	for (size_t i = 0; i < N; ++i)
	{
		int digit = GetDigit(array[i], 0);
		if (digit != -1)
			ranges[digit].push_back(array[i]);
	}
}

// quick sort
template<typename T>
void QuickSort(T array[])
{
	std::function<void(int, int)> quick_sort = [&quick_sort, &array](int left, int right) -> void
	{
		if (left < right)
		{
			const T pivot = array[left + static_cast<int>(std::floor((right - left) / 2))];
			int i = left, j = right;
			while (i <= j)
			{
				for (; i <= right && array[i] > pivot; ++i);
				for (; j >= left && array[j] < pivot; --j);

				if (i <= j)
					std::swap(array[i++], array[j--]);
			}
			quick_sort(left, i - 1);
			quick_sort(i, right);
		}
	};

	quick_sort(0, N - 1);
}

template<typename T>
void TestSort(void (*sort_func)(T array[]), size_t count)
{
	srand(time(nullptr));
	bool is_printed = false;

	while (count > 0)
	{
		T array[N];
		for (size_t i = 0; i < N; ++i)
			array[i] = std::floor(rand() % RANGE);

		T old_array[N];
		std::copy(array, array + N, old_array);

		sort_func(array);
		if (!is_printed)
		{
			PrintArray(old_array);
			PrintArray(array);
			is_printed = true;
		}

		for (size_t i = 0; i < N - 1; ++i)
			if (array[i] < array[i + 1])
			{
				PrintArray(old_array);
				PrintArray(array);
				std::cout << "fail" << std::endl;
				return;
			}

		--count;
		std::cout << "success" << std::endl;
	}
}

int main(int argc, char argv[])
{
	TestSort<int>(QuickSort, 10);
	getchar();
}