#pragma once

#include "Animals/ZodiacAnimals.hpp"


class Combat {
public:
    static void startBattle(ZodiacAnimals* player, ZodiacAnimals* opponent);
    static void playerTurn(ZodiacAnimals* player, ZodiacAnimals* opponent);
    static void opponentTurn(ZodiacAnimals* player, ZodiacAnimals* opponent);
    static bool isBattleOver(ZodiacAnimals* player, ZodiacAnimals* opponent);

private:
    static int calculateAttackDamage(ZodiacAnimals* attacker, ZodiacAnimals* target);
};