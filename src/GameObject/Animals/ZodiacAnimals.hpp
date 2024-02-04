
#ifndef ZodiacAnimals_H
#define ZodiacAnimals_H

#include <string>
#include "Elements.hpp"

class ZodiacAnimals {
public:
    ZodiacAnimals(const std::string& name, Element element);
    virtual ~ZodiacAnimals();

    virtual void makeSound() const = 0;
    virtual void performMove() const = 0;

    void printInfo() const;

protected:
    std::string name;
    Element element;
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

#endif // ANIMAL_H