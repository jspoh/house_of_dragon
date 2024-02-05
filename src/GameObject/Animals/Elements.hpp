#pragma once

#include <string>

enum Element {
    NO_ELEMENT,
    Water,
    Earth,
    Metal,
    Fire,
    Wind,
    MAX_ELEMENTS
};
enum DamageMultiplier {
    Weak,
    Neutral,
    Strong,
};

namespace ElementProperties {
    static std::string getElementName(Element element);
    static std::string getPlayerAbilityNames(Element element);
    DamageMultiplier getEffectiveDamage(Element playerElement, Element targetElement);
};
