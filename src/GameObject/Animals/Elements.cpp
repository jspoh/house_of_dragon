// Elements.cpp
#include "Elements.hpp"
#include "../../Backend/Pch.h"

// functions of the elements, can be more interactive with more functions as we go along.

namespace ElementProperties {
    std::string getElementName(Element element) {
    switch (element) {
        case Element::Water:
            return "Water";
        case Element::Earth:
            return "Earth";
        case Element::Metal:
            return "Metal";
        case Element::Fire:
            return "Fire";
        case Element::Wind:
            return "Wind";
        default:
            return "Unknown";
    }
}

    std::string getPlayerAbilityNames(Element element) { // can be used to return a string to print out onto the screen
    switch (element) {
        case Element::Water:
            return "Hydro Blast!!";
        case Element::Earth:
            return "Quake Strike!!";
        case Element::Metal:
            return "Iron cross!!";
        case Element::Fire:
            return "Inferno Burst!!";
        case Element::Wind:
            return "Aero Slash!!";
        default:
            std::cerr << "bad boy\n";
            return "Unknown Ability?";
    }
}

    DamageMultiplier getEffectiveDamage(Element playerElement, Element targetElement) {  // use enum instead
    // This functions returns how effective the damage is from the player move element, 1 = default, 2 = effective, 3 = weak.

    // Elemental interactions
    switch (playerElement) {
    case Element::Water:
        if (targetElement == Element::Fire) {
            return DamageMultiplier::Strong; // Water is effective against Fire
        }
        else if (targetElement == Element::Metal) {
            return DamageMultiplier::Weak; // Water is weak against Metal
        }
        break;
    case Element::Fire:
        if (targetElement == Element::Wind) {
            return DamageMultiplier::Strong; // Fire is effective against Wind
        }
        else if (targetElement == Element::Earth) {
            return DamageMultiplier::Weak; // Fire is weak against Earth
        }
        break;
    case Element::Wind:
        if (targetElement == Element::Earth) {
            return DamageMultiplier::Strong; // Wind is effective against Earth
        }
        else if (targetElement == Element::Water) {
            return DamageMultiplier::Weak; // Wind is weak against Water
        }
        break;
    case Element::Earth:
        if (targetElement == Element::Metal) {
            return DamageMultiplier::Strong; // Earth is effective against Metal
        }
        else if (targetElement == Element::Fire) {
            return DamageMultiplier::Weak; // Earth is weak against Fire
        }
        break;
    case Element::Metal:
        if (targetElement == Element::Water) {
            return DamageMultiplier::Strong; // Metal is neutral against Water
        }
        else if (targetElement == Element::Wind) {
            return DamageMultiplier::Weak; // Metal is weak against Wind
        }
        break;
    default:
        return DamageMultiplier::Neutral;
        break;


        // continue like this
    }
    return DamageMultiplier::Neutral;



 
}
    

}
