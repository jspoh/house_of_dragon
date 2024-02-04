
#include "ZodiacAnimals.hpp"
#include <iostream>

ZodiacAnimals::ZodiacAnimals(const std::string& name, Element element) : name(name), element(element) {}

ZodiacAnimals::~ZodiacAnimals() {}

Rat::Rat(const std::string& name) : ZodiacAnimals(name, Element::Fire) {}

void Rat::makeSound() const {
    std::cout << "Squeak! Squeak!" << std::endl;
}

void Rat::performMove() const {
    std::cout << "Rat performs digging move." << std::endl;
}

// Lion class implementation

Tiger::Tiger(const std::string& name) : ZodiacAnimals(name, Element::Earth) {}

void Tiger::makeSound() const {
    std::cout << "Roar!" << std::endl;
}

void Tiger::performMove() const {
    std::cout << "Lion performs hunting move." << std::endl;
}