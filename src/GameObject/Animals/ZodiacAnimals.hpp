#pragma once

#include <string>
#include "Elements.hpp"

class ZodiacAnimals {
public:
    ZodiacAnimals(const std::string& name, Element element);
    virtual ~ZodiacAnimals();

    virtual void makeSound() const = 0;
    virtual void performMove() const = 0;


protected:
    std::string name;
    Element element;
    int health;
    double baseDmg;  // u see how la ok
};


class Rat : public ZodiacAnimals {
public:
    Rat(const std::string& name);

    void makeSound() const override;
    void performMove() const override;
};


class Tiger : public ZodiacAnimals {
public:
    Tiger(const std::string& name);

    void makeSound() const override;
    void performMove() const override;
};
