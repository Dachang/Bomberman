
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
	std::vector<EnemyAI*>enemyList;
	std::vector<Ogre::SceneNode*>playerLifeList;
	bool isSpaceKeyDown;

	void initGameData(void);
	void createSceneMgr(Ogre::Root *_root);
	void createInput(OIS::Keyboard* keyboard);
	void createCamera(void);
	void createScene(void);
	bool gameOver;
	bool gameWin;
	bool showWinState;
	bool showLoseState;
	void Update(const Ogre::FrameEvent& evt);


private:
	Player* player;
	GameMap* gameMap;
	EnemyAI* enemy;
	//EnemyAI* enemy;
	

	int bombIndex;
	float _addBonusDuration;
	bool _canAddBonus;
	float _gameWinDuration;
	float _gameOverDuration;

	Ogre::Vector3 getWorldCoord(Ogre::Vector2 pos, float yPos);

	void addBonus(Ogre::Vector2 pos);
	void updateEnemyList(const Ogre::FrameEvent& evt,GameMap* gameMap);
	void updateMapGridType();
	void updatePlayerLifecycle();
	void updateBonus(const Ogre::FrameEvent& evt);
	void updateBonusDuration(const Ogre::FrameEvent& evt);
	void updateGameState(const Ogre::FrameEvent& evt);
	void updatePlayerLifeDisplay();
};

#endif