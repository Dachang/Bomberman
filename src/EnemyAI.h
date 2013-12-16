#ifndef __ENEMYAI_H__
#define __ENEMYAI_H__
#include "GameObject.h"
#include "AIBrain.h"
#include "GameMap.h"
#include "EnemyAIMotionType.h"
//#define START_POSITION Ogre::Vector2(20,0)

typedef enum EnemyDirectionType
{
	AI_LEFT_DIRECTION = 1,
	AI_RIGHT_DIRECTION,
	AI_UP_DIRECTION,
	AI_DOWN_DIRECTION
};

class EnemyAI : public GameObject
{
public:
	EnemyAI(Ogre::Entity* entity,Ogre::SceneNode* node,Ogre::SceneManager *sceneMgr,Ogre::Camera *camera,OIS::Keyboard *keyboard);
	~EnemyAI(void);

	virtual void Start(void);
	virtual void Update(const Ogre::FrameEvent& evt,GameMap* map);
	void Transform(const Ogre::FrameEvent& evt,EnemyAIMotionType type);
	Ogre::Vector2 convertWorldPosToGridPos(Ogre::Vector3 pos);
private:
	//member variables
	AIBrain myBrain;
	EnemyAIMotionType myMotionType;
	GameMap* gameMap;
	Ogre::Vector2 myPosition;

	Ogre::Real _moveSpeed;  
	EnemyDirectionType _direction;  
	Ogre::Vector3 _transVector;
	int _isTurning;

	void updateMyPosition();
	void playerMoveLeft();
	void playerMoveRight();
	void playerMoveUp();
	void playerMoveDown();
};
#endif