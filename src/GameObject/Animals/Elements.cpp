// Elements.cpp
#include "Elements.hpp"

// functions of the elements, can be more interactive with more functions as we go along.

namespace elements{
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

    std::string GetPlayerAbilityNames(Element element) {
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
            return "Unknown Ability?";
    }
}

int getEffectiveDamage(Element playerElement, Element targetElement) {
    // This functions returns how effective the damage is from the player move element, 1 = default, 2 = effective, 3 = weak.

    // Elemental interactions
    if (playerElement == Element::Water) {
        if (targetElement == Element::Fire) {
            return 2; // Water is effective against Fire
        } else if (targetElement == Element::Metal) {
            return 3; // Water is weak against Metal
        }
    } else if (playerElement == Element::Fire) {
        if (targetElement == Element::Wind) {
            return 2; // Fire is effective against Wind
        } else if (targetElement == Element::Earth) {
            return 3; // Fire is weak against Earth
        }
    } else if (playerElement == Element::Wind) {
        if (targetElement == Element::Earth) {
            return 2; // Wind is effective against Earth
        } else if (targetElement == Element::Water) {
            return 3; // Wind is weak against Water
        }
    } else if (playerElement == Element::Earth) {
        if (targetElement == Element::Metal) {
            return 2; // Earth is effective against Metal
        } else if (targetElement == Element::Fire) {
            return 3; // Earth is weak against Fire
        }
    } else if (playerElement == Element::Metal) {
        if (targetElement == Element::Water) {
            return 2; // Metal is neutral against Water
        } else if (targetElement == Element::Wind) {
            return 3; // Metal is weak against Wind
        }
    }

    return 1; // Default damage for non-interacting elements
}
    

}
