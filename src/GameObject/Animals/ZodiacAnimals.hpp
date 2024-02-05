#pragma once
//ZodiacAnimals
#include <string>
#include "Elements.hpp"

class ZodiacAnimals {
public:
    ZodiacAnimals(const std::string& name, Element element, double baseDmg, double health);
    virtual ~ZodiacAnimals();

    virtual std::string makeSound() const = 0; // returns a string to start the battle, will use text to print it out.
    virtual std::string performMove() const = 0; // when animal perform move, use the return string to print it out.
    virtual void attack() const = 0;
    virtual Element getElement() const = 0;
    virtual double* getHealth() ;
    virtual void setHealth(double newHealth) = 0;
    virtual std::string getName() const;

protected:
    std::string name; 
    Element element;
    double *health;
    double baseDmg;

};


class Rat : public ZodiacAnimals {
public:
    Rat(const std::string& name);

    std::string makeSound() const override;
    std::string performMove() const override;
    void attack() const override;
    virtual Element getElement() const override;
    //virtual double getHealth() const override;
    virtual void setHealth(double newHealth) = 0;
    virtual std::string getName() const;
};


class Tiger : public ZodiacAnimals {
public:
    Tiger(const std::string& name);

    std::string makeSound() const override;
    std::string performMove() const override;
    void attack() const override;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======

    Element getElement() const override;
    double* getHealth()  override;
    void setHealth(double newHealth) override;
=======
    virtual Element getElement() const override;
    virtual double getHealth() const = 0;
    virtual void setHealth(double newHealth) = 0;
    virtual std::string getName() const;
>>>>>>> Stashed changes
};




struct PlayerMoves {
    std::string move1;
    std::string move2;
    std::string move3;
    std::string move4;
    std::string move5;
};




class Player {
public:
    Player(const std::string& name);

    void makeSound() const;
    void performMove() const;
    double getHealth() const;
    void setHealth(double newHealth);

    Element getElementMove() const;
    void changeElement(Element newElement);

    const PlayerMoves& getPlayerMoves() const;

private:
    std::string name;
    double health;
    Element currentElement;
    PlayerMoves playerMoves;
    // Add other member variables and functions as needed.
>>>>>>> Stashed changes
};

