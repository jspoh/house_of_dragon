#pragma once

#include <string>

enum Element {
    NO_ELEMENT,
    Water,
    Earth,
    Metal,
    Fire,
    Wood,
    MAX_ELEMENTS
};
enum DamageMultiplier {
    Weak,
    Neutral,
    Strong,
};

namespace ElementProperties {
    std::string getElementName(Element element);
    std::string getPlayerAbilityNames(Element element);
    DamageMultiplier getEffectiveDamage(Element playerElement, Element targetElement);
};
