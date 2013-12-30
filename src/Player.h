#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "GameObject.h"
#include "GameMap.h"
#include "GameBomb.h"

//#define START_POSITION Ogre::Vector2(20,0)

typedef enum directionType
{
	LEFT_DIRECTION = 1,
	RIGHT_DIRECTION,
	UP_DIRECTION,
	DOWN_DIRECTION
};

class Player : public GameObject
{
public:
	Player(Ogre::Entity* entity,Ogre::SceneNode* node,Ogre::SceneManager *sceneMgr,Ogre::Camera *camera,OIS::Keyboard *keyboard);
	~Player(void);
	//methods
	virtual void Start(void);
	virtual void Update(const Ogre::FrameEvent& evt,GameMap* gameMap);
	int getPlayerXPos();
	int getPlayerYPos();
	directionType getPlayerDirection();
	int getPlayerHP();
	void updatePlayerWithGrid(const Ogre::FrameEvent& evt, GameMap* gameMap);
	//properties
	gridType _currentGridType;
	gridType _lastGridType;
	gridType _leftGridType;
	gridType _rightGridType;
	gridType _upGridType;
	gridType _downGridType;
private:
	//member variables
	int _healthValue;
	float _unbreakableDuration;
	bool _unbreakable;
	Ogre::Real _moveSpeed;  
	int  MOVESPEED;
	directionType _direction;  
	directionType lastDirection;
	Ogre::Vector3 _transVector;
	int _leftToRight;
	int _upToDown;
	int _leftToUp;
	int _leftToDown;
	int _rightToUp;
	int _rightToDown;
	Ogre::Vector2 _pos;
	Ogre::Vector2 _destinationPos;
	int _bombAvailableNumber;
	int _bombLevel;
	bool _playerIsPlacingBomb;
	int _bombIndex;
	std::map<int,GameBomb*> _bombList;
	//class methods
	void playerMoveLeft();
	void playerMoveRight();
	void playerMoveUp();
	void playerMoveDown();
	Ogre::Vector2 convertWorldPosToGridPos(Ogre::Vector3 pos);
	void updateAnimation(const Ogre::FrameEvent& evt);
	void updateBomb(const Ogre::FrameEvent& evt,GameMap* gameMap);
	void Transform(GameMap* gameMap,const Ogre::FrameEvent& evt);
	void TurnBack(GameMap* gameMap);
	void Rotate();
	void updateUnbreakable(const Ogre::FrameEvent& evt);
};
#endif