
#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include <string>

enum class Element {
    Water,
    Earth,
    Metal,
    Fire,
    Wind
};

namespace ElementProperties {
    static std::string getElementName(Element element);
    static std::string getElementAbility(Element element);
    static int getEffectiveDamage(Element playerElement, Element targetElement);
};

#endif // ELEMENTS_HPP