#pragma once

#include <cmath>
#include <functional>
#include <algorithm>
#include <utility>
#include <type_traits>
#include <vector>

constexpr static size_t ARRAY_SIZE = 11;

template<typename T>
void BubbleSort(T array[])
{
    for(size_t i = 0; i < ARRAY_SIZE; ++i)
        for(size_t j = 0; j < ARRAY_SIZE - i - 1; ++j)
            if(array[j] < array[j + 1])
                std::swap(array[j], array[j + 1]);
}

template<typename T>
void SelectionSort(T array[])
{
    for(size_t i = 0; i < ARRAY_SIZE - 1; ++i)
    {
        size_t maxIndex = i;
        for(size_t j = i + 1; j < ARRAY_SIZE; ++j)
            if(array[j] > array[maxIndex])
                maxIndex = j;

        if(maxIndex != i)
            std::swap(array[i], array[maxIndex]);
    }
}

template<typename T>
void CocktailSort(T array[])
{
    size_t left = 0, right = ARRAY_SIZE - 1;
    while(left < right)
    {
        for(size_t i = left; i < right; ++i)
            if(array[i] < array[i + 1])
                std::swap(array[i], array[i + 1]);
        --right;

        for(size_t i = right; i > left; --i)
            if(array[i - 1] < array[i])
                std::swap(array[i - 1], array[i]);
        ++left;
    }

}

template<typename T>
void InsertionSort(T array[], size_t arraySize, size_t d = 1)
{
    for(size_t i = d; i < arraySize; ++i)
        for(size_t j = i; (j >= d) && (array[j - d] < array[j]); j -= d)
            std::swap(array[j - d], array[j]);
}

template<typename T>
void InsertionSort(T array[])
{
    InsertionSort(array, ARRAY_SIZE);
}

template<typename T>
void ShellSort(T array[])
{
    size_t center = static_cast<size_t>(std::floor(ARRAY_SIZE / 2));
    for(size_t d = center; d != 0; d /= 2)
        InsertionSort(array, ARRAY_SIZE, d);
}

template<typename T>
void GnomeSort(T array[])
{
    size_t i = 1, j = i;
    while(i < ARRAY_SIZE)
    {
        if(i != 0 && array[i - 1] < array[i])
        {
            std::swap(array[i - 1], array[i]);
            if(j < i)
                j = i;

            --i;
        }
        else
            i = ++j;
    }
}

template<typename T>
void MergeSort(T array[])
{
    std::function<void (T[], size_t)> mergeSort = [&mergeSort](T array[], size_t size) -> void
    {
        if(size == 1)
            return;

        size_t size1 = static_cast<size_t>(std::floor(size / 2));
        size_t size2 = size - size1;

        T* array1 = array;
        T* array2 = array + size1;
        mergeSort(array1, size1);
        mergeSort(array2, size2);

        T* tmpArray = new T[size];
        size_t index1 = 0;
        size_t index2 = 0;
        for(size_t i = 0; i < size; ++i)
        {
            // First array is done, fill another
            if(index1 == size1)
            {
                for(size_t j = index2; j < size2; ++j)
                    tmpArray[i++] = std::move(array2[j]);
                break;
            }

            // Second array is done, fill another
            if(index2 == size2)
            {
                for(size_t j = index1; j < size1; ++j)
                    tmpArray[i++] = std::move(array1[j]);
                break;
            }

            if(array1[index1] > array2[index2])
                tmpArray[i] = std::move(array1[index1++]);
            else
                tmpArray[i] = std::move(array2[index2++]);
        }

        std::copy(tmpArray, tmpArray + size, array);
        delete[] tmpArray;
    };

    mergeSort(array, ARRAY_SIZE);
}

template<typename T>
T GetDigit(T number, int radix)
{
    if(radix > 0)
        number = static_cast<T>(std::floor(number / radix)) % 10;
    return number;
}

template<typename T>
std::pair<T, T> GetMinMax(T array[], int radix = 0)
{
    T min = GetDigit(array[0], radix), max = min;
    for(size_t i = 0; i < ARRAY_SIZE; ++i)
    {
        T number = GetDigit(array[i], radix);
        if(number > max)
            max = number;
        if(number < min)
            min = number;
    }

    return std::make_pair(min, max);
}

template<typename T>
void CountingSort(T array[], int radix)
{
    static_assert(std::is_integral<T>::value, "Only integral types");

    std::pair<T, T> minmax = GetMinMax(array, radix);
    const T min = minmax.first;
    const T max = minmax.second - min;
    int* counting = new int[max + 1] { 0 };
    for(size_t i = 0; i < ARRAY_SIZE; ++i)
        ++counting[GetDigit(array[i], radix) - min];

    for(int i = max; i > 0; --i)
        counting[i - 1] += counting[i];

    T* tmpArray = new T[ARRAY_SIZE];
    for(int i = ARRAY_SIZE - 1; i >= 0; --i)
    {
        int index = --counting[GetDigit(array[i], radix) - min];
        tmpArray[index] = array[i];
    }

    std::copy(tmpArray, tmpArray + ARRAY_SIZE, array);
    delete[] counting, tmpArray;
}

template<typename T>
void CountingSort(T array[])
{
    CountingSort<T>(array, 0);
}

template<typename T>
void RadixSort(T array[])
{
    static_assert(std::is_integral<T>::value, "Only integral types");

    T max = *std::max_element(array, array + ARRAY_SIZE);
    for(int radix = 1; (max / radix) > 0; radix *= 10)
        CountingSort(array, radix);
}

template<typename T>
void QuickSort(T array[])
{
    std::function<void(int, int)> quickSort = [&quickSort, &array](int left, int right) -> void
    {
        if(left < right)
        {
            const T pivot = array[left + static_cast<int>(std::floor((right - left) / 2))];
            int i = left, j = right;
            while(i <= j)
            {
                for(; (i <= right) && (array[i] > pivot); ++i);
                for(; (j >= left) && (array[j] < pivot); --j);

                if(i <= j)
                    std::swap(array[i++], array[j--]);
            }
            quickSort(left, i - 1);
            quickSort(i, right);
        }
    };

    quickSort(0, ARRAY_SIZE - 1);
}

template<typename T>
void Heapify(T array[], size_t arraySize, int rootIndex)
{
    int smallest = rootIndex;
    int leftIndex = 2*rootIndex + 1;
    int rightIndex = 2*rootIndex + 2;

    if(leftIndex < arraySize && array[leftIndex] < array[smallest])
        smallest = leftIndex;

    if(rightIndex < arraySize && array[rightIndex] < array[smallest])
        smallest = rightIndex;

    if(smallest != rootIndex)
    {
        std::swap(array[rootIndex], array[smallest]);
        Heapify(array, arraySize, smallest);
    }
}

template<typename T>
void HeapSort(T array[])
{
    int i = std::floor(ARRAY_SIZE / 2) - 1;
    for(; i >= 0; --i)
        Heapify(array, ARRAY_SIZE, i);

    for(int i = ARRAY_SIZE - 1; i >= 0; --i)
    {
        std::swap(array[0], array[i]);
        Heapify(array, i, 0);
    }
}

size_t GetMinRun(size_t arraySize)
{
    size_t run = 0;
    while(arraySize >= 64)
    {
        run |= arraySize & 1;
        arraySize >>= 1;
    }
    return arraySize + run;
}

template<typename T>
void TimSortMerge(T array[], size_t left, size_t middle, size_t right)
{
    size_t len1 = middle - left + 1, len2 = right - middle;
    T* leftArray = new T[len1];
    T* rightArray = new T[len2];

    for(size_t i = 0; i < len1; ++i)
        leftArray[i] = array[left + i];
    for(size_t i = 0; i < len2; ++i)
        rightArray[i] = array[middle + 1 + i];

    size_t i = 0;
    size_t j = 0;
    size_t k = left;

    while(i < len1 && j < len2)
    {
        if(leftArray[i] <= rightArray[j])
            array[k] = leftArray[i++];
        else
            array[k] = rightArray[j++];
        ++k;
    }

    while(i < len1)
        array[k++] = leftArray[i++];

    while(j < len2)
        array[k++] = rightArray[j++];

    delete[] leftArray, rightArray;
}

template<typename T>
void TimSort(T array[])
{
    size_t run = GetMinRun(ARRAY_SIZE);

    for(size_t i = 0; i < ARRAY_SIZE; i += run)
    {
        size_t arraySize = std::min((i + run - 1), (ARRAY_SIZE - 1));
        InsertionSort(array, arraySize, i - 1);
    }

    for(size_t size = run; size < ARRAY_SIZE; size *= 2)
    {
        for(size_t left = 0; left < ARRAY_SIZE; left += 2*size)
        {
            int middle = left + size - 1;
            int right = std::min((left + 2*size - 1), (ARRAY_SIZE - 1));

            if(middle < right)
                TimSortMerge(array, left, middle, right);
        }
    }
}

template<typename T>
void BucketSort(T array[])
{
    constexpr size_t numberOfBuckets = ARRAY_SIZE;
    std::pair<T, T> minmax = GetMinMax(array);
    float range = (minmax.second - minmax.first) / static_cast<float>(numberOfBuckets);
    std::vector<T> buckets[numberOfBuckets];

    size_t index, i = 0;
    float fractionalPart;
    for(i = 0; i < ARRAY_SIZE; ++i)
    {
        float integerPart;
        float number = (array[i] - minmax.first) / range;
        fractionalPart = static_cast<float>(std::modf(number, &integerPart));
        index = static_cast<size_t>(integerPart);

        if(std::clamp(fractionalPart , -0.01f, 0.01f) == fractionalPart && array[i] != minmax.first)
            buckets[index - 1].push_back(std::move(array[i]));
        else
            buckets[index].push_back(std::move(array[i]));
    }

    for(size_t i = 0; i < numberOfBuckets; ++i)
        if(buckets[i].size() > 1)
            InsertionSort(&buckets[i][0], buckets[i].size());

    index = 0;
    for(int i = numberOfBuckets - 1; i >= 0; --i)
        for(size_t j = 0; j < buckets[i].size(); ++j)
            array[index++] = buckets[i][j];
}