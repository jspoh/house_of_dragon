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


Enemy::Enemy(Element element, double health, double dmg, std::string texturePath, std::string textureRef, float screenX, float screenY, float size)
    : Mob(element, health, dmg), _textureRef(textureRef), _size(size) {
    this->_spos.x = screenX;
    this->_spos.y = screenY;

    _spos = Point{ AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f };
    this->_wpos = stow(_spos.x, _spos.y);

    RenderHelper::getInstance()->registerTexture(textureRef, texturePath);  // problematic code stopping execution

    //RenderHelper::getInstance()->texture(_textureRef, _wpos.x, _wpos.y, _size, _size);
}

void Enemy::render() {
    //std::cout << RenderHelper::getInstance()->getTextureByRef(this->_textureRef) << ", " << this->_textureRef << "\n";
    RenderHelper::getInstance()->texture(this->_textureRef, this->_wpos.x, this->_wpos.y, this->_size, this->_size);
    RenderHelper::getInstance()->text(std::to_string(this->health), this->_spos.x, this->_spos.y - this->_size / 3 * 2);
}

Enemy::~Enemy() {
    std::cout << "Destroying enemy with texture ref: " << this->_textureRef << "\n";
    RenderHelper::getInstance()->removeTextureByRef(this->_textureRef);
}