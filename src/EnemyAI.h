#ifndef __ENEMYAI_H__
#define __ENEMYAI_H__
#include "GameObject.h"
#include "AIBrain.h"
#include "GameMap.h"
#include "EnemyAIMotionType.h"
#include "GameBomb.h"
//#define START_POSITION Ogre::Vector2(20,0)

typedef enum EnemyDirectionType
{
	AI_UP_DIRECTION=0,
	AI_RIGHT_DIRECTION,
	AI_DOWN_DIRECTION,
	AI_LEFT_DIRECTION,
};

class EnemyAI : public GameObject
{
public:
	EnemyAI(Ogre::Entity* entity,Ogre::SceneNode* node,Ogre::SceneManager *sceneMgr,Ogre::Camera *camera,OIS::Keyboard *keyboard);
	~EnemyAI(void);

	virtual void Start(void);
	virtual void Update(const Ogre::FrameEvent& evt,GameMap* map);
	void Transform(GameMap* gameMap,const Ogre::FrameEvent& evt,EnemyAIMotionType type);
	Ogre::Vector2 convertWorldPosToGridPos(Ogre::Vector3 pos);
	void CheckBonusCollision(GameMap* gameMap);
private:
	//member variables
	AIBrain myBrain;
	EnemyAIMotionType myMotionType;
	GameMap* gameMap;
	Ogre::Vector2 myPosition;

	Ogre::Real _moveSpeed;  
	EnemyDirectionType _direction;
	EnemyDirectionType lastDirection;
	Ogre::Vector3 _transVector;
	int _isTurning;

	int _bombAvailableNumber;
	int _bombLevel;
	bool _playerIsPlacingBomb;
	int _bombIndex;

	void AddBomb(GameMap* gameMap);
	void updateBomb(const Ogre::FrameEvent& evt,GameMap* gameMap);
	std::map<int,GameBomb*> _bombList;
	void updateMyPosition();
	void Rotate();
};
#endif