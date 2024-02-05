/*
#include "ZodiacAnimals.hpp"
#include <iostream>

ZodiacAnimals::ZodiacAnimals(const std::string& name, Element element, double baseDmg, double* health) : name(name), element(element), baseDmg(baseDmg) , health(health) {}

ZodiacAnimals::~ZodiacAnimals() {}

Rat::Rat(const std::string& name) : ZodiacAnimals(name, Element::Fire,12.5f, 50) {}

std::string Rat::makeSound() const {
    return "Squeak! Squeak!";
}

std::string Rat::performMove() const {
    return "Rat performs digging move.";
}

<<<<<<< Updated upstream
=======
void Rat::attack() const {

}

Element Rat::getElement() const {
    return Element::MAX_ELEMENTS;  // for compilation only. replace this with appropriate value

}

std::string Rat::getName() const {
    return "THE FIRST ALWAYS, THE RAT ARRIVES!!";
}

>>>>>>> Stashed changes
// Tiger class implementation

Tiger::Tiger(const std::string& name) : ZodiacAnimals("Tiger", Element::Earth, 20, 100) {}

std::string Tiger::makeSound() const {
    return "Roar!! Fear the king of the jungle!!";
}

std::string Tiger::performMove() const {
    return "Tiger use Blazing Claw!!";
}


void Tiger::attack() const {
    std::cout << "Animal attacks!" << std::endl;
}

Element Tiger::getElement() const {
    return element;
}

double* Tiger::getHealth() const {
    return *health;
}

void Tiger::setHealth(double newHealth) {
    *health = newHealth;
}

std::string ZodiacAnimals::getName() const {
    return name;
}


Player::Player(const std::string& name)
    : name(name), health(100.0), currentElement(Element::Water) {
    // Initialize player moves (you can customize these)
    playerMoves.move1 = "Aqua Blast!";
    playerMoves.move2 = "Earthquake Strike!";
    playerMoves.move3 = "Inferno Burst!";
    playerMoves.move4 = "Gale Slash!";
    playerMoves.move1 = "Metal Stab!";
}



//int Player::performMove(std::string) const {
//    return;
//}



double Player::getHealth() const {
    return health;
}

void Player::setHealth(double newHealth) {
    // Optional: Add validation logic if needed.
    health = newHealth;
}

//Element Player::getCurrentElement() const {
//    return Element::Water;
//}

void Player::changeElement(Element newElement) {
    currentElement = newElement;
}

const PlayerMoves& Player::getPlayerMoves() const {
    return playerMoves;
}
*/

