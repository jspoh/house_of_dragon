#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "../GameObject/GameObject.h"
#include <vector>

class SceneManager;
class SceneBase : public Scene
{
public:
	SceneBase();
	~SceneBase();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	SceneBase(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static SceneBase* sInstance; // The pointer to the object that gets registered

	//// Tile information
	//const int kiHalfTileWidth = 12;
	//const int kiHalfTileHeight = 12;

	//// Scene2D images
	//SpriteEntity* Scene2D_Background;
	//SpriteEntity* Scene2D_Hero;
	//SpriteEntity* Scene2D_TileGround;
	//SpriteEntity* Scene2D_TileTree;
	//SpriteEntity* Scene2D_RearStructure;
	//SpriteEntity** Scene2D_Hero_Animated;
	//SpriteEntity* Scene2D_Goodies_TreasureChest;

	//// Handle to the tilemaps
	//CMap* m_cMap;
	//CMap* m_cRearMap;	// Pointer to the rear map

	//// This class stores all information about the player / avatar
	//CPlayerInfo2D* thePlayerInfo;

	//// The enemies
	//CEnemy** theEnemy;
	//int m_iNumEnemy;
	//SpriteEntity* Scene2D_Enemy;

	//// Render the tile map
	//void RenderTileMap(void);
	//// Render the rear tile map
	//void RenderRearTileMap(void);
	//// Render the player
	//void RenderPlayer(void);
	//// Render the enemy
	//void RenderEnemy(void);
	//// Create the Goodies
	//void CreateGoodies(void);
};

#endif