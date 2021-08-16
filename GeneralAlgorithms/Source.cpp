#include "SortFunctions.h"
#include "BinaryTree.h"
#include "AVLTree.h"
#include "RedBlackTree.h"
#include "DesignPatterns.h"

#include <iostream>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <stack>

constexpr static int RANGE = 100;

template<typename T>
void PrintArray(const T array[])
{
    for(size_t i = 0; i < ARRAY_SIZE; ++i)
        std::cout << array[i] << " ";
    std::cout << std::endl;
}

template<typename T>
float ExecuteFunction(void (*function)(T array[]), T array[])
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    function(array);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration<float, std::milli> diff = end - begin;
    return diff.count();
}

template<typename T>
void TestSortFunction(void (*sortFunction)(T array[]), size_t numberOfTests)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    bool is_printed = false;
    float sumOfTimes = 0.0f;

    for(int j = numberOfTests; j > 0; --j)
    {
        T array[ARRAY_SIZE];
        for(size_t i = 0; i < ARRAY_SIZE; ++i)
            array[i] = static_cast<T>(rand() % RANGE);

        T old_array[ARRAY_SIZE];
        std::copy(array, array + ARRAY_SIZE, old_array);

        sumOfTimes += ExecuteFunction<T>(sortFunction, array);
 
        if(!is_printed)
        {
            PrintArray(old_array);
            PrintArray(array);
            is_printed = true;
        }

        for(size_t i = 0; i < ARRAY_SIZE - 1; ++i)
        {
            if(array[i] < array[i + 1])
            {
                std::cout << "fail" << std::endl;
                PrintArray(old_array);
                PrintArray(array);
                return;
            }
        }
    }

    float averageExecutionTime = sumOfTimes / static_cast<float>(numberOfTests);
    std::cout << "Average execution time: " << averageExecutionTime << " milliseconds" << std::endl;
}

void TestTree()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    // TODO: make unit test for Remove

    BinaryTree<size_t, size_t> tree;
    AVLTree<size_t, size_t> tree2;
    for(size_t i = 1; i < 15; ++i)
    {
        size_t key = static_cast<size_t>(rand() % 100);
        tree.Insert(key, key * 2);
        tree2.Insert(key, key * 2);
    }

    tree2.InOrderTraverse([](size_t key, const size_t& value) {
        std::cout << key << "-" << value << " ";
    });

    std::cout << std::endl;

    tree2.PreOrderTraverse([](size_t key, const size_t& value) {
        std::cout << key << "-" << value << " ";
        });

    std::cout << std::endl;

    tree2.PostOrderTraverse([](size_t key, const size_t& value) {
        std::cout << key << "-" << value << " ";
        });

    std::cout << std::endl;

    /*
    size_t key;
    std::cin >> key;
    std::cout << tree.Find(key).value_or(0) << std::endl;
    tree.Remove(key);
    std::cout << tree.Find(key).value_or(0) << std::endl;
    */
}

void TestDesignPatterns()
{
    // Factory
    FoodPrototypeFactory foodFactory(new Dish(new Fruit()), new Fruit(), new Meat(), new Fish());
    Fish* fish = foodFactory.MakeFish();
    Meat* meat = foodFactory.MakeMeat();
    Fruit* fruit = foodFactory.MakeFruit();
    Dish* dish = foodFactory.MakeDish(fruit);
    meat->Eat();

    // Builder
    /*
    DishBuilder dishBuilder;
    dishBuilder.AddFruitToDish();
    dishBuilder.AddMeatToDish();
    dishBuilder.AddFishToDish();
    dishBuilder.GetDish();
    */

    // Singleton
    Singleton* singleton = Singleton::GetInstance();

    // Adapter
    Meat* adapter = new Adapter();
    adapter->Eat();
}



int main(int argc, char argv[])
{
    //TestSortFunction<int>(ShellSort, 10000);
    //TestTree();
    //TestDesignPatterns();


    return 0;
}