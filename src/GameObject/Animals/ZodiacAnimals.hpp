#pragma once
//ZodiacAnimals
#include <string>
#include "Elements.hpp"

class ZodiacAnimals {
public:
    ZodiacAnimals(const std::string& name, Element element, double baseDmg, double health);
    virtual ~ZodiacAnimals();

    virtual void makeSound() const = 0;
    virtual void performMove() const = 0;
    virtual void attack() const = 0;
    virtual Element getElement() const = 0;
    virtual double getHealth() const = 0;
    virtual void setHealth(double newHealth) = 0;
    virtual std::string getName() const;

protected:
    std::string name; 
    Element element;
    double health;
    double baseDmg;

};


class Rat : public ZodiacAnimals {
public:
    Rat(const std::string& name);

    void makeSound() const override;
    void performMove() const override;
    void attack() const override;
    virtual Element getElement() const override;
    virtual double getHealth() const = 0;
    virtual void setHealth(double newHealth) = 0;
    virtual std::string getName() const;
};


class Tiger : public ZodiacAnimals {
public:
    Tiger(const std::string& name);

    void makeSound() const override;
    void performMove() const override;
    void attack() const override;

    Element getElement() const;
    double getHealth() const;
    void setHealth(double newHealth);
};
