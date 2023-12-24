#include <iostream>
#include <string>
using namespace std;

// Основной класс Животное
class Animal {
protected:
    std::string name;

public:
    Animal(const std::string& animalName) : name(animalName) {}

    virtual void MakeSound() const {
        std::cout << "Животное издает звук" << std::endl;
    }
    virtual ~Animal() = default;
};

// Класс Млекопитающие
class Mammal : public Animal {
public:
    Mammal(const std::string& mammalName) : Animal(mammalName) {}
};

// Класс Рыбы
class Fish : public Animal {
public:
    Fish(const std::string& fishName) : Animal(fishName) {}
};

// Класс Карпообразные
class CarpLikeFish : public Fish {
public:
    CarpLikeFish(const std::string& carpLikeFishName) : Fish(carpLikeFishName) {}
};

// Класс Карась
class Carp : public CarpLikeFish {
public:
    Carp(const std::string& carpName) : CarpLikeFish(carpName) {}
};

// Класс Медвежьи
class BearFamily : public Mammal {
public:
    BearFamily(const std::string& bearFamilyName) : Mammal(bearFamilyName) {}
};

// Класс Медведь
class Bear : public BearFamily {
public:
    Bear(const std::string& bearName) : BearFamily(bearName) {}
};

// Класс Псовые
class Canine : public Mammal {
public:
    Canine(const std::string& canineName) : Mammal(canineName) {}
};

// Класс Собаки
class Dog : public Canine {
public:
    Dog(const std::string& dogName) : Canine(dogName) {}
};

// Класс Гризли
class Grizzly : public Bear {
public:
    Grizzly(const std::string& grizzlyName) : Bear(grizzlyName) {}
    void Hunt() const {
        std::cout << "Гризли охотится на карася" << std::endl;
    }
};

// Класс Лабрадор
class Labrador : public Dog {
public:
    Labrador(const std::string& labradorName) : Dog(labradorName) {}
    void Fetch() const {
        std::cout << "Лабрадор выбегает за палкой" << std::endl;
    }
};

// Класс Локация
class Location {
public:
    void Interact(const Carp& carp, const Grizzly& grizzly, const Labrador& labrador) const {
        std::cout << "Карась пытается уйти от гризли, а лабрадор преследует его" << std::endl;
        grizzly.Hunt();
        labrador.Fetch();
    }
};

int main() {
    Carp carp("Карась");
    Grizzly grizzly("Гризли");
    Labrador labrador("Лабрадор");
    Location location;
    location.Interact(carp, grizzly, labrador);

    return 0;
}
