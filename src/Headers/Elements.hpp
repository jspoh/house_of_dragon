/* Start Header************************************************************************ /
/*!
\file Elements.hpp
\author Kuek wei jie, weijie.kuek, 2301325
\par weijie.kuek\@digipen.edu
\date 28 feb 2024
\brief game element mechanics implementation
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

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
