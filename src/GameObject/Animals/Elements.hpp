#pragma once

#include <string>

enum Element {
    Water,
    Earth,
    Metal,
    Fire,
    Wind,
    MAX_ELEMENTS
};
enum DamageMultipler {
    Weak,
    Neutral,
    Strong,
};

namespace ElementProperties {
    std::string getElementName(Element element);
    std::string getPlayerAbilityNames(Element element);
    int getEffectiveDamage(Element playerElement, Element targetElement);
};
