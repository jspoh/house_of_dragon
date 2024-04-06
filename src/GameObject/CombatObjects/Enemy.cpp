/* Start Header ************************************************************************/
/*!
\file Enemy.cpp
\author Poh Jing Seng, jingseng.poh, 2301363 (50%, base enemy combat logic)
\par jingseng.poh\@digipen.edu
\author Kuek wei jie, weijie.kuek, 2301325 (50%, enemy rendering and animation logic)
\par weijie.kuek\@digipen.edu
\date 28 feb 2024
\brief handles enemy in combat
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Pch.h"
#include "Enemy.h"

//Enemy::Enemy() {
//
//}


//local variables
float paddingY = 120.f;
float paddingInfoY = 110.f;

Enemy::Enemy(Element element, float health, float dmg, std::string texturePath, std::string textureRef, float screenX, float screenY, float size)
    : Mob(element, health, dmg), _textureRef(textureRef), _size(size) {
    this->_spos.x = screenX;
    this->_spos.y = screenY;
    this->fullhealth = health;
    this->_textureRef = textureRef;
    _spos = AEVec2{ screenX, screenY};
    this->_wpos = stow(_spos.x, _spos.y);
    this->healthpos.x = this->_wpos.x - 50;
    this->healthpos.y = this->_wpos.y + 50;
    this->attackPoint.x = _wpos.x;
    this->attackPoint.y = _wpos.y;
    this->AttackEnd.y = -AEGfxGetWindowHeight() / 2.9f;
    this->AttackEnd.x = 0;
    this->attacking = false;
    this->attacked = false;
    this->startingHealth = health;
    this->AttackedRenderX = health / startingHealth;
    //RenderHelper::getInstance()->texture(_textureRef, _wpos.x, _wpos.y, _size, _size);
}


void Enemy::elementstringinput(std::string el) {
    this->elementString = el;
}
void Enemy::update([[maybe_unused]] double dt) {

    AEVec2 pos = wtos(_wpos.x - camOffset.x, _wpos.y - camOffset.y);

    if (AEInputCheckTriggered(AEVK_LBUTTON) && CollisionChecker::isMouseInRect(pos.x, pos.y, _size, _size, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
        //cout << _textureRef << " enemy selected\n";
        isSelected = !isSelected;
    }
    //cout << camOffset.x << ", " << camOffset.y << "\n";
    if (this->attacked && this->healthRenderTime < this->healthRenderTimeMax) {
        healthRenderTime += static_cast<float>(AEFrameRateControllerGetFrameTime());
        float percenttime = static_cast<float>(healthRenderTime / healthRenderTimeMax);
        float t = percenttime;
        if (t > healthRenderTimeMax) {
            t = healthRenderTimeMax;
        }
        this->AttackedRenderX = lerp(this->AttackedRenderXprev, health / startingHealth,t);
    }
    else {
        this->attacked = false;
    }
}


void Enemy::render() {
    //AEVec2 camOffset;
    //AEGfxGetCamPosition(&camOffset.x, &camOffset.y);


    // shaking motion when enemy is m_hasAttacked
    if(this->attacked){
        if (this->shakeDuration > 0) {
            // Apply shake effect only when m_hasAttacked (so smart to use sin fr thats crazy -js)
            float shakeOffset = sin(this->shakeFrequency * this->shakeDuration) * this->shakeAmplitude * this->shakeDuration;
            RenderHelper::getInstance()->texture(this->_textureRef, this->_wpos.x + shakeOffset, this->_wpos.y , this->_size, this->_size);
            this->shakeDuration -= static_cast<float>(AEFrameRateControllerGetFrameTime());
        }
        else {
            this->attacked = false;
            this->shakeDuration = 1.f;
        }

    }
    else {
        // render the enemy texture static
        RenderHelper::getInstance()->texture(this->_textureRef, this->_wpos.x, this->_wpos.y, this->_size, this->_size);
        //cout << "Enemy pos: " << this->_wpos.x << ", " << this->_wpos.y << "\n";
    }

    if (isSelected) {
        RenderHelper::getInstance()->texture("border", this->_wpos.x, this->_wpos.y, this->_size + 50, this->_size + 50 ); // size should change
    }
     
    //health rendering
      //rendering of enemy info
    RenderHelper::getInstance()->texture("enemyPanel", this->_wpos.x, this->healthpos.y - paddingInfoY, 160, 40); //start point, but coordinates is centralised so need to take account of the widthw
    RenderHelper::getInstance()->texture((elementString), this->_wpos.x - 60, this->healthpos.y - paddingInfoY + 5.f, 50, 50); //start point, but coordinates is centralised so need to take account of the widthw
    RenderHelper::getInstance()->texture((_textureRef + "name"), this->_wpos.x -10.f, this->healthpos.y - paddingInfoY + 5.f, 70, 20); //start point, but coordinates is centralised so need to take account of the widthw

    //  health over 2/3, green bar
    if (this->health > 66) {
        RenderHelper::getInstance()->texture("greenbar1", this->_wpos.x - 50, this->healthpos.y - paddingY, 10, 10); //start point, but coordinates is centralised so need to take account of the widthw
        RenderHelper::getInstance()->texture("greenbar3", this->_wpos.x - 45 + AttackedRenderX * 50 , this->healthpos.y - paddingY, AttackedRenderX * 100, 10);
        RenderHelper::getInstance()->texture("greenbar2", this->_wpos.x + AttackedRenderX * 100 - 40, this->healthpos.y - paddingY, 10, 10);
    }
     // health between 1/3 and 2/3 , bar turns to orange/yellow
    else if (this->health > 33) {
        RenderHelper::getInstance()->texture("yellowbar1", this->_wpos.x - 50, this->healthpos.y - paddingY, 10, 10); //start point, but coordinates is centralised so need to take account of the widthw
        RenderHelper::getInstance()->texture("yellowbar3", this->_wpos.x - 45 + AttackedRenderX * 50, this->healthpos.y - paddingY, AttackedRenderX * 100, 10);
        RenderHelper::getInstance()->texture("yellowbar2", this->_wpos.x + AttackedRenderX * 100 - 40, this->healthpos.y - paddingY, 10, 10);
    }
    // health lower than 1/3, bar will turn red

    else {
        RenderHelper::getInstance()->texture("redbar1", this->_wpos.x - 50, this->healthpos.y - paddingY, 10, 10); //start point, but coordinates is centralised so need to take account of the widthw
        RenderHelper::getInstance()->texture("redbar3", this->_wpos.x - 45 + AttackedRenderX * 50, this->healthpos.y - paddingY, AttackedRenderX * 100, 10);
        RenderHelper::getInstance()->texture("redbar2", this->_wpos.x + AttackedRenderX * 100 - 40, this->healthpos.y - paddingY, 10, 10);

    }






}

Enemy::~Enemy() {
    cout << "Destroying enemy with texture ref: " << this->_textureRef << "\n";
    // dont free here!!!!!!!!!!!!
    //RenderHelper::getInstance()->removeTextureByRef(this->_textureRef);
    //RenderHelper::getInstance()->removeTextureByRef("healthbar1");
    //RenderHelper::getInstance()->removeTextureByRef("healthbar2");
    //RenderHelper::getInstance()->removeTextureByRef("healthbar3");
}

void Enemy::enemyAttacked() {
    this->attacked = true;
    this->AttackedRenderXprev = AttackedRenderX;
    healthRenderTime = 0.f;

}

std::string Enemy::getTextureRef() const {
    return _textureRef;
}

void Enemy::enemyAttacking(float timeleft) {
    //this->attacking = true;
    // udpating the coordinates

    float timeStart = 0.25f;
    if ((timeleft < timeStart * 1000) && attacktime  < timeStart ) {
        attacktime += static_cast<float>(AEFrameRateControllerGetFrameTime());
        float percenttime = static_cast<float>(attacktime / timeStart);
        float t = percenttime;
        if (t > timeStart) {
            t = timeStart;
        }
        attackPoint.x = lerp(_wpos.x, AttackEnd.x, t);
        attackPoint.y = lerp(_wpos.y, AttackEnd.y, t);

    }
}
void Enemy::EnemyAttackStop() {
    //this->attacking = false;
    //this->attacktime = 0.f;
    //this->attackPoint.x = (_wpos.x);
    //this->attackPoint.y = (_wpos.y);
    return;
}

AEVec2 Enemy::getWorldPos() const {
    return _wpos;
}

AEVec2 Enemy::getSize() const {
    return { this->_size, this->_size };
}
