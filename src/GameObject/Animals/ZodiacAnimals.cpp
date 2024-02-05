
#include "ZodiacAnimals.hpp"
#include <iostream>

ZodiacAnimals::ZodiacAnimals(const std::string& name, Element element, double baseDmg, double health) : name(name), element(element), baseDmg(baseDmg) , health(health) {}

ZodiacAnimals::~ZodiacAnimals() {}

Rat::Rat(const std::string& name) : ZodiacAnimals(name, Element::Fire,12.5f, 50) {}

void Rat::makeSound() const {
    std::cout << "Squeak! Squeak!" << std::endl;
}

void Rat::performMove() const {
    std::cout << "Rat performs digging move." << std::endl;
}

void Rat::attack() const {

}

Element Rat::getElement() const {
    return Element::MAX_ELEMENTS;  // for compilation only. replace this with appropriate value

}

std::string Rat::getName() const {
    return "";
}

// Tiger class implementation

Tiger::Tiger(const std::string& name) : ZodiacAnimals(name, Element::Earth,20,100) {}

void Tiger::makeSound() const {
    std::cout << "Roar!" << std::endl;
}

void Tiger::performMove() const {
    std::cout << "Tiger performs hunting move." << std::endl;
}


void Tiger::attack() const {
    std::cout << "Animal attacks!" << std::endl;
}

Element Tiger::getElement() const {
    return element;
}

double Tiger::getHealth() const {
    return health;
}

void Tiger::setHealth(double newHealth) {
    health = newHealth;
}

std::string ZodiacAnimals::getName() const {
    return name;
}
