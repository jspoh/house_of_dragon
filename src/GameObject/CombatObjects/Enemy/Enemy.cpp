#include "Enemy.h"

Enemy::Enemy(Element element, double health, double dmg, std::string texturePath, std::string textureRef, float screenX, float screenY, float size)
    : Mob(element, health, dmg), _textureRef(textureRef), _size(size) {
    this->_spos.x = screenX;
    this->_spos.y = screenY;

    _spos = Point{ AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f };
    this->_wpos = stow(_spos.x, _spos.y);

    RenderHelper::getInstance()->registerTexture(textureRef, texturePath);  // problematic code stopping execution

    RenderHelper::getInstance()->texture(_textureRef, _wpos.x, _wpos.y, _size, _size);
}

void Enemy::render() {
    std::cout << RenderHelper::getInstance()->getTextureByRef(this->_textureRef) << ", " << this->_textureRef << "\n";
    RenderHelper::getInstance()->texture(_textureRef, _wpos.x, _wpos.y, _size, _size);
    RenderHelper::getInstance()->text(std::to_string(this->health), this->_spos.x, this->_spos.y - _size / 3 * 2);
}

Enemy::~Enemy() {
    RenderHelper::getInstance()->removeTextureByRef(this->_textureRef);
}