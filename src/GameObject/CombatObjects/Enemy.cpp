/* Start Header ************************************************************************/
/*!
\file Enemy.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 28 feb 2024
\brief handles enemy in combat
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Enemy.h"
#include "utils.h"


Enemy::Enemy(Element element, double health, double dmg, std::string texturePath, std::string textureRef, float screenX, float screenY, float size)
    : Mob(element, health, dmg), _textureRef(textureRef), _size(size) {
    this->_spos.x = screenX;
    this->_spos.y = screenY;
    this->fullhealth = health;
    this->_textureRef = textureRef;
 
    _spos = Point{ screenX, screenY};
    this->_wpos = stow(_spos.x, _spos.y);
    this->healthpos.x = this->_wpos.x - 50;
    this->healthpos.y = this->_wpos.y + 50;

    //RenderHelper::getInstance()->texture(_textureRef, _wpos.x, _wpos.y, _size, _size);
}
double Enemy::newHealth(double desiredX) { 
    return this->health;

}
void Enemy::render() {
    //std::cout << RenderHelper::getInstance()->getTextureByRef(this->_textureRef) << ", " << this->_textureRef << "\n";
    RenderHelper::getInstance()->texture("border", this->_wpos.x, this->_wpos.y + 100, this->_size + 50, this->_size + 50 ); // size should change
    RenderHelper::getInstance()->texture(this->_textureRef, this->_wpos.x, this->_wpos.y + 100, this->_size, this->_size);


    RenderHelper::getInstance()->texture("bar1", this->healthpos.x, this->healthpos.y, 10, 10); //start point, but coordinates is centralised so need to take account of the widthw
    RenderHelper::getInstance()->texture("bar3", this->_wpos.x - 45 + 50, this->healthpos.y, (health / 100) * 100, 10);
    RenderHelper::getInstance()->texture("bar2", this->_wpos.x + (fullhealth / 100) * 100 - 40, this->healthpos.y, 10, 10);
    //healthbar; currently hardcoded so its not as usable
    if (this->health > 66) {
        RenderHelper::getInstance()->texture("greenbar1", this->_wpos.x - 50, this->healthpos.y, 10, 10); //start point, but coordinates is centralised so need to take account of the widthw
        RenderHelper::getInstance()->texture("greenbar3", this->_wpos.x - 45 + (health / 100) * 50, this->healthpos.y, (health / 100) * 100, 10);
        RenderHelper::getInstance()->texture("greenbar2", this->_wpos.x + (health / 100) * 100 - 40, this->healthpos.y, 10, 10);
    }
    else if (this->health > 33) {
        RenderHelper::getInstance()->texture("yellowbar1", this->_wpos.x - 50, this->healthpos.y, 10, 10); //start point, but coordinates is centralised so need to take account of the widthw
        RenderHelper::getInstance()->texture("yellowbar3", this->_wpos.x - 45 + (health / 100) * 50, this->healthpos.y, (health / 100) * 100, 10);
        RenderHelper::getInstance()->texture("yellowbar2", this->_wpos.x + (health / 100) * 100 - 40, this->healthpos.y, 10, 10);
    }
    else {
        RenderHelper::getInstance()->texture("redbar1", this->_wpos.x - 50, this->healthpos.y, 10, 10); //start point, but coordinates is centralised so need to take account of the widthw
        RenderHelper::getInstance()->texture("redbar3", this->_wpos.x - 45 + (health / 100) * 50, this->healthpos.y, (health / 100) * 100, 10);
        RenderHelper::getInstance()->texture("redbar2", this->_wpos.x + (health / 100) * 100 - 40, this->healthpos.y, 10, 10);
    }

    /* need to change the values into variable, formaula::healthbar1 = (position x - (standardsize), position y - (standardsize);(texSize,texSize)
    healthbar2 = (position x - (standardsize - texSize/2) + (health / healthlength) * healthlength/2,  position y - (standardsize);(healthlength,texSize)
    healthbar3 = (position x -  (standardsize - texSize) +  (health / healthlength) * healthlength,position y - (standardsize);(texSize,texSize);
    */
}

Enemy::~Enemy() {
    std::cout << "Destroying enemy with texture ref: " << this->_textureRef << "\n";
    RenderHelper::getInstance()->removeTextureByRef(this->_textureRef);
    RenderHelper::getInstance()->removeTextureByRef("healthbar1");
    RenderHelper::getInstance()->removeTextureByRef("healthbar2");
    RenderHelper::getInstance()->removeTextureByRef("healthbar3");
}