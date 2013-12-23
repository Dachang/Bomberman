#pragma once
//#define AI_UP 1001
//#define AI_DOWN 1002
//#define AI_LEFT 1003
//#define AI_RIGHT 1004
//#define AI_BOMB	1005
//#define AI_WAIT 1006
//
//#define AI_STAT_FARTHER 1001
//#define AI_STAT_NPC 1002
//#define AI_STAT_WAIT 1003

//#include "BMPlayer.h"
//#include "Map.h"
#include "EnemyAIMotionType.h"
#include "GameMap.h"

typedef enum AIMode
{
	AI_ATTACK=0,
	AI_FIND,
};


class AIBrain
{
public:
	AIBrain(void);
	~AIBrain(void);
	//int mapFlood[15][13];
	//int calculateNPCNextEvent(BMPlayer &nonNPC, BMPlayer &npc, Map &map);
	EnemyAIMotionType Update(EnemyAIMotionType newType,GameMap* map,Ogre::Vector2 position);
private:
	
	EnemyAIMotionType myCurrenType;
	EnemyAIMotionType myLastType;
	GameMap* gameMap;
	int FloodMapForPlayer[MAP_WIDTH][MAP_HEIGHT];
	int FloodMapForBonus[MAP_WIDTH][MAP_HEIGHT];
	double WeightTable[MAP_WIDTH][MAP_HEIGHT];
	Ogre::Vector2 myPosition;
	Ogre::Vector2 playerPosition;
	Ogre::Vector2 bonusPosition;
	//the level after flood
	int PlayerFillLevel;
	int BonusFillLevel;
	int playerOriginalWeight;
	int bonusOriginalWeight;
	int attenuationOfWeight;
	double lastDistanceToBonus;
	int minDistanceToPlayer;

	//attack
	AIMode motionMode;
	int motionSeed;
	int timeToLiveOfmotion;

	void Thinking();
	void ThinkingForMoving();
	void ThinkingForAttacking();
	void AttackMode();
	double Distance(Ogre::Vector2 from,Ogre::Vector2 to);
	void UpdateAllPosition(Ogre::Vector2 position);
	void SetOthersPosition();
	void FloodFillForPlayer( int x,int y,int level);
	void FloodFillForBonus( int x,int y,int level);
	void traceBack(int _x, int _y,  const int map[MAP_WIDTH][MAP_HEIGHT],int PlayerOrBonus);
	void DetectionForThingsUnableAcross();
	void DetectionForDestroyableWall();
	void ChooseType();
	//Ogre::Vector2 GetBonusPosition();
	Ogre::Vector2 convertWorldPosToGridPos(Ogre::Vector3 pos);
	/*int AIState;
	void calculateRecursive(int x, int y, int level);
	void output();
	int farthestLevel;
	int farthestX;
	int farthestY;
	int npcPosX;
	int npcPosY;
	void getFarthestInfo(void);
	int getValue(int x, int y);
	int traceBack(int _x, int _y);
	int distance(int x1, int y1, int x2, int y2);*/
};