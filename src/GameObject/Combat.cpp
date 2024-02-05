#include "Combat.hpp"
#include <iostream>
#include "Animals/ZodiacAnimals.hpp"

void Combat::startBattle(ZodiacAnimals* player, ZodiacAnimals* opponent) {
    while (!isBattleOver(player, opponent)) {
        // Player's turn
        playerTurn(player, opponent);

        // Check if the battle is over after player's turn
        if (isBattleOver(player, opponent)) {
            break;
        }

        // Opponent's turn
        opponentTurn(player, opponent);
    }

    // Output battle result
    if (player->getHealth() <= 0) {
        std::cout << "Player's animal fainted. Battle over!" << std::endl;
    }
    else {
        std::cout << "Opponent's animal fainted. You won!" << std::endl;
    }
}

void Combat::playerTurn(ZodiacAnimals* player, ZodiacAnimals* opponent) {
    // Player's turn logic
    std::cout << "Player's turn!" << std::endl;

    // Example: Attack opponent
    int damage = calculateAttackDamage(player, opponent);

    // Modify opponent's health
    double newHealth = opponent->getHealth() - damage;
    opponent->setHealth(newHealth);

    std::cout << "Player's animal dealt " << damage << " damage to the opponent's animal." << std::endl;
    std::cout << "Opponent's health: " << opponent->getHealth() << std::endl;
}

void Combat::opponentTurn(ZodiacAnimals* player, ZodiacAnimals* opponent) {
    // Opponent's turn logic
    std::cout << "Opponent's turn!" << std::endl;

    // Example: Attack player
    int damage = calculateAttackDamage(opponent, player);

    // Modify player's health
    double newHealth = player->getHealth() - damage;
    player->setHealth(newHealth);

    std::cout << "Opponent's animal dealt " << damage << " damage to the player's animal." << std::endl;
    std::cout << "Player's health: " << player->getHealth() << std::endl;
}

bool Combat::isBattleOver(ZodiacAnimals* player, ZodiacAnimals* opponent) {
    // Check if either player or opponent has zero or less health
    return player->getHealth() <= 0 || opponent->getHealth() <= 0;
}

int Combat::calculateAttackDamage(ZodiacAnimals* attacker, ZodiacAnimals* target) {
    // Example: Calculate damage based on the attacker's base damage
    double baseDamage = attacker->getBaseDamage();

    // Element effectiveness
    int effectiveness = ElementProperties::getEffectiveDamage(attacker->getElement(), target->getElement());

    // Apply effectiveness to the base damage
    int totalDamage = static_cast<int>(baseDamage * effectiveness);

    return totalDamage;
}