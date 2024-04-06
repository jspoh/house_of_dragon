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

Enemy::Enemy(Element element, float m_health, float m_dmg, std::string texturePath, std::string textureRef, float screenX, float screenY, float size)
    : Mob(element, m_health, m_dmg), m_textureRef(textureRef), m_size(size) {
    this->m_spos.x = screenX;
    this->m_spos.y = screenY;
    this->m_fullhealth = m_health;
    this->m_textureRef = textureRef;
    m_spos = AEVec2{ screenX, screenY};
    this->m_wpos = stow(m_spos.x, m_spos.y);
    this->m_healthpos.x = this->m_wpos.x - 50;
    this->m_healthpos.y = this->m_wpos.y + 50;
    this->m_attackPoint.x = m_wpos.x;
    this->m_attackPoint.y = m_wpos.y;
    this->m_attackEnd.y = -AEGfxGetWindowHeight() / 2.9f;
    this->m_attackEnd.x = 0;
    this->m_isAttacking = false;
    this->m_attacked = false;
    this->m_attackedRenderX = m_health / m_maxHealth;
    //RenderHelper::getInstance()->texture(m_textureRef, m_wpos.x, m_wpos.y, m_size, m_size);
}


void Enemy::elementstringinput(std::string el) {
    this->m_elementString = el;
}
void Enemy::update([[maybe_unused]] double dt) {

    AEVec2 pos = wtos(m_wpos.x - camOffset.x, m_wpos.y - camOffset.y);

    if (AEInputCheckTriggered(AEVK_LBUTTON) && CollisionChecker::isMouseInRect(pos.x, pos.y, m_size, m_size, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
        //cout << m_textureRef << " enemy selected\n";
        m_isSelected = !m_isSelected;
    }
    //cout << camOffset.x << ", " << camOffset.y << "\n";
    if (this->m_attacked && this->m_healthRenderTime < this->m_HEALTH_RENDER_TIME_MAX) {
        m_healthRenderTime += static_cast<float>(AEFrameRateControllerGetFrameTime());
        float percenttime = static_cast<float>(m_healthRenderTime / m_HEALTH_RENDER_TIME_MAX);
        float t = percenttime;
        if (t > m_HEALTH_RENDER_TIME_MAX) {
            t = m_HEALTH_RENDER_TIME_MAX;
        }
        this->m_attackedRenderX = lerp(this->m_attackedRenderXprev, m_health / m_maxHealth,t);
    }
    else {
        this->m_attacked = false;
    }
}


void Enemy::render() {
    //AEVec2 camOffset;
    //AEGfxGetCamPosition(&camOffset.x, &camOffset.y);


    // shaking motion when enemy is m_hasAttacked
    if(this->m_attacked){
        if (this->m_shakeDuration > 0) {
            // Apply shake effect only when m_hasAttacked (so smart to use sin fr thats crazy -js)
            float shakeOffset = sin(this->m_shakeFrequency * this->m_shakeDuration) * this->m_shakeAmplitude * this->m_shakeDuration;
            RenderHelper::getInstance()->texture(this->m_textureRef, this->m_wpos.x + shakeOffset, this->m_wpos.y , this->m_size, this->m_size);
            this->m_shakeDuration -= static_cast<float>(AEFrameRateControllerGetFrameTime());
        }
        else {
            this->m_attacked = false;
            this->m_shakeDuration = 1.f;
        }

    }
    else {
        // render the enemy texture static
        RenderHelper::getInstance()->texture(this->m_textureRef, this->m_wpos.x, this->m_wpos.y, this->m_size, this->m_size);
        //cout << "Enemy pos: " << this->m_wpos.x << ", " << this->m_wpos.y << "\n";
    }

    if (m_isSelected) {
        RenderHelper::getInstance()->texture("border", this->m_wpos.x, this->m_wpos.y, this->m_size + 50, this->m_size + 50 ); // size should change
    }
     
    //health rendering
      //rendering of enemy info
    RenderHelper::getInstance()->texture("enemyPanel", this->m_wpos.x, this->m_healthpos.y - paddingInfoY, 160, 40); //start point, but coordinates is centralised so need to take account of the widthw
    RenderHelper::getInstance()->texture((m_elementString), this->m_wpos.x - 60, this->m_healthpos.y - paddingInfoY + 5.f, 50, 50); //start point, but coordinates is centralised so need to take account of the widthw
    RenderHelper::getInstance()->texture((m_textureRef + "name"), this->m_wpos.x -10.f, this->m_healthpos.y - paddingInfoY + 5.f, 70, 20); //start point, but coordinates is centralised so need to take account of the widthw

    //  health over 2/3, green bar
    if (this->m_health > 66) {
        RenderHelper::getInstance()->texture("greenbar1", this->m_wpos.x - 50, this->m_healthpos.y - paddingY, 10, 10); //start point, but coordinates is centralised so need to take account of the widthw
        RenderHelper::getInstance()->texture("greenbar3", this->m_wpos.x - 45 + m_attackedRenderX * 50 , this->m_healthpos.y - paddingY, m_attackedRenderX * 100, 10);
        RenderHelper::getInstance()->texture("greenbar2", this->m_wpos.x + m_attackedRenderX * 100 - 40, this->m_healthpos.y - paddingY, 10, 10);
    }
     // health between 1/3 and 2/3 , bar turns to orange/yellow
    else if (this->m_health > 33) {
        RenderHelper::getInstance()->texture("yellowbar1", this->m_wpos.x - 50, this->m_healthpos.y - paddingY, 10, 10); //start point, but coordinates is centralised so need to take account of the widthw
        RenderHelper::getInstance()->texture("yellowbar3", this->m_wpos.x - 45 + m_attackedRenderX * 50, this->m_healthpos.y - paddingY, m_attackedRenderX * 100, 10);
        RenderHelper::getInstance()->texture("yellowbar2", this->m_wpos.x + m_attackedRenderX * 100 - 40, this->m_healthpos.y - paddingY, 10, 10);
    }
    // health lower than 1/3, bar will turn red

    else {
        RenderHelper::getInstance()->texture("redbar1", this->m_wpos.x - 50, this->m_healthpos.y - paddingY, 10, 10); //start point, but coordinates is centralised so need to take account of the widthw
        RenderHelper::getInstance()->texture("redbar3", this->m_wpos.x - 45 + m_attackedRenderX * 50, this->m_healthpos.y - paddingY, m_attackedRenderX * 100, 10);
        RenderHelper::getInstance()->texture("redbar2", this->m_wpos.x + m_attackedRenderX * 100 - 40, this->m_healthpos.y - paddingY, 10, 10);

    }






}

Enemy::~Enemy() {
    cout << "Destroying enemy with texture ref: " << this->m_textureRef << "\n";
    // dont free here!!!!!!!!!!!!
    //RenderHelper::getInstance()->removeTextureByRef(this->m_textureRef);
    //RenderHelper::getInstance()->removeTextureByRef("healthbar1");
    //RenderHelper::getInstance()->removeTextureByRef("healthbar2");
    //RenderHelper::getInstance()->removeTextureByRef("healthbar3");
}

void Enemy::enemyAttacked() {
    this->m_attacked = true;
    this->m_attackedRenderXprev = m_attackedRenderX;
    m_healthRenderTime = 0.f;

}

std::string Enemy::getTextureRef() const {
    return m_textureRef;
}

void Enemy::enemyAttacking(float timeleft) {
    //this->m_isAttacking = true;
    // udpating the coordinates

    float timeStart = 0.25f;
    if ((timeleft < timeStart * 1000) && m_attacktime  < timeStart ) {
        m_attacktime += static_cast<float>(AEFrameRateControllerGetFrameTime());
        float percenttime = static_cast<float>(m_attacktime / timeStart);
        float t = percenttime;
        if (t > timeStart) {
            t = timeStart;
        }
        m_attackPoint.x = lerp(m_wpos.x, m_attackEnd.x, t);
        m_attackPoint.y = lerp(m_wpos.y, m_attackEnd.y, t);

    }
}
void Enemy::EnemyAttackStop() {
    //this->m_isAttacking = false;
    //this->m_attacktime = 0.f;
    //this->m_attackPoint.x = (m_wpos.x);
    //this->m_attackPoint.y = (m_wpos.y);
    return;
}

AEVec2 Enemy::getWorldPos() const {
    return m_wpos;
}

AEVec2 Enemy::getSize() const {
    return { this->m_size, this->m_size };
}
