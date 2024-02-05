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
    static std::string getElementName(Element element);
    static std::string getPlayerAbilityNames(Element element);
    static int getEffectiveDamage(Element playerElement, Element targetElement);
};
