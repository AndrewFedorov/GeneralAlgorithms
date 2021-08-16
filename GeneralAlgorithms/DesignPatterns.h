#pragma once

#include <iostream>
#include <vector>

class Food
{
public:
    virtual void Eat() = 0;
};

class Fruit : public Food
{
public:
    virtual void Eat() override {};
};

class Meat : public Food
{
public:
    virtual void Eat() override
    {
        std::cout << "Meat" << std::endl;
    };
};

class GoldMeat : public Meat
{
public:
    virtual void Eat() override
    {
        std::cout << "GoldMeat" << std::endl;
    };
};

class Fish : public Food
{
public:
    virtual void Eat() override {};
};

class Dish
{
public:
    Dish(Food* food)
    {
        this->food = food;
    }

private:
    Food* food;
};

class FoodFactory
{
public:
    virtual Fruit* MakeFruit() const
    {
        return new Fruit();
    }

    virtual Meat* MakeMeat() const
    {
        return new Meat();
    }

    virtual Fish* MakeFish() const
    {
        return new Fish();
    }

    virtual Dish* MakeDish(Food* food) const
    {
        return new Dish(food);
    }

    // Factory method
    Dish* CreateDish()
    {
        return MakeDish(MakeMeat());
    }
};

class BetterFoodFactory : public FoodFactory
{
public:
    virtual Meat* MakeMeat() const
    {
        return new GoldMeat();
    }
};

class FoodPrototypeFactory : public FoodFactory
{
public:
    FoodPrototypeFactory(Dish* dish, Fruit* fruit, Meat* meat, Fish* fish):
        prototypeDish(dish), prototypeFruit(fruit), prototypeMeat(meat), prototypeFish(fish) {}

    virtual Meat* MakeMeat() const
    {
        std::cout << "prototypeMeat" << std::endl;
        return prototypeMeat ? new Meat(*prototypeMeat) : nullptr;
    }

private:
    Dish* prototypeDish;
    Fruit* prototypeFruit;
    Meat* prototypeMeat;
    Fish* prototypeFish;
};

class DishBuilder
{
public:
    virtual void AddFruitToDish() {}
    virtual void AddMeatToDish() {}
    virtual void AddFishToDish() {}

    virtual Dish* GetDish() const
    {
        return nullptr;
    }
};

class Adapter : private Dish, public Meat
{
public:
    Adapter(): Dish(new Meat()) {};

    virtual void Eat() override
    {
        std::cout << "Adapter" << std::endl;
    }
};

class Singleton
{
public:
    static Singleton* GetInstance()
    {
        if(!singleton)
            singleton = new Singleton();
        return singleton;
    }
private:
    inline static Singleton* singleton = nullptr;

    Singleton() {};
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

class Composite : public Food
{
public:
    void Add(Food* food)
    {
        listOfFood.push_back(food);
    }

protected:
    std::vector<Food*> listOfFood;
};