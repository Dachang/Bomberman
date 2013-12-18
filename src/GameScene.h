
#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include <Ogre.h>
#include <map>
#include "BMPlayer.h"
#include "AIBrain.h"
#include "Map.h"
#include "Bomb.h"
#include <OIS.h>
#include "Player.h"
#include "EnemyAI.h"
#include "GameMap.h"

#define GRID_SIZE 70
#define DEFAULT_SPEED 0.3

class GameScene
{
public:
	GameScene(void);
	~GameScene(void);

	Ogre::SceneManager *mSceneMgr;
	Ogre::Camera *mCamera;
	OIS::Keyboard *mKeyboard;

	Ogre::AnimationState *nonNPCAnimState;
	Ogre::AnimationState *NPCAnimState;

	Ogre::SceneNode *nonNPCPlayerNode;
	Ogre::SceneNode *NPCPlayerNode;
	Ogre::SceneNode *mapNode[15][13];


	
	Map map;
	BMPlayer nonNPCPlayer;
	BMPlayer NPCPlayer;
	AIBrain aiBrain;

	std::map<int, Bomb> bombPool;

	bool isSpaceKeyDown;

	void initGameData(void);
	void createSceneMgr(Ogre::Root *_root);
	void createInput(OIS::Keyboard* keyboard);
	void createCamera(void);
	void createScene(void);

	//void askingPlayer(int playerType, int eventType);
	//void updatePlayerPos(void);
	//void updateNPCPlayerPos(void);

	//int thromBomb(BMPlayer &player);
	//void updateBombInfo(const Ogre::FrameEvent& evt);

	//int updatePlayerInfo(const Ogre::FrameEvent& evt);

	void Update(const Ogre::FrameEvent& evt);


private:
	Player* player;
	GameMap* gameMap;
	EnemyAI* enemy;

	int bombIndex;

	Ogre::Vector3 getWorldCoord(Ogre::Vector2 pos);
	//void movingPlayer(int playerType, float speed, Ogre::SceneNode *playerNode, Ogre::Vector3 direction, Ogre::AnimationState *animState);
	//void calculateBombArea(Bomb &bomb);
	//void explodeBomb(int bombIdx);
	void addBonus(Ogre::Vector2 pos);

	//void updatePlayerGridInfo();
	void updateMapGridType();
};

#endif