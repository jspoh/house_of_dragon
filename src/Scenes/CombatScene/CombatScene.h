#pragma once

#include "../../Scenes/Scene.h"
#include "../../GameObject/Animals/Elements.hpp"
#include "../../utils/utils.h"

#include <vector>
//Testing SpriteAnimation
class SceneManager;
class CombatScene : public Scene
{
public:
	CombatScene();
	~CombatScene();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	CombatScene(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static CombatScene* sInstance; // The pointer to the object that gets registered
};


/*defining these functions here to chiong prototype, no time to make nicenice rn*/

class Mob {
public:
	double health;
	double maxHealth;
	double dmg;
	Element element;

	Mob(Element element, double health, double dmg);

	bool isDead();

	/**
	 * .
	 * 
	 * \param target
	 * \param attackEl
	 * \return amount of damage dealt
	 */
	virtual double attack(Mob& target);

	virtual void reset();
};


class Enemy : public Mob {
	private:
		std::string _textureRef;
		// screen pos
		Point _spos;
		// world pos (rendering only!!!!)
		Point _wpos;
		float _size = 50;
		
	public:
		Enemy(Element element, double health, double dmg, std::string texturePath, std::string textureRef, float screenX = 100, float screenY = 100, float size = 50);
		~Enemy();
		
			void render();
	};

class Cat : public Enemy {
public:
	Cat(Element element = Element::Water, double health = 100, double dmg = 10, std::string texturePath = "", std::string textureRef = "", float screenX = 100, float screenY = 100, float size = 50);
};

class Player : public Mob {
public:
	Player(double health = 100, double dmg = 10, Element element = Element::NO_ELEMENT);

	/**
	 * .
	 *
	 * \param target
	 * \param attackEl element player attacked with
	 * \param qtMultiplier quicktime event multiplier
	 * \return damage dealt
	 */
	double attack(Mob& target, Element attackEl, double qtMultiplier);
};
