#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "GameObject.h"

#define START_POSITION Ogre::Vector2(20,0)

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

	virtual void Start(void);
	virtual void Update(const Ogre::FrameEvent& evt);
	int getPlayerXPos();
	int getPlayerYPos();
	void updatePlayerWithGrid(int gt);
private:
	//member variables
	Ogre::Real _moveSpeed;  
	directionType _direction;  
	Ogre::Vector3 _transVector;
	int _isTurningVertical;
	int _isTurningHorizontal;

	int _leftToRight;
	int _upToDown;
	int _leftToUp;
	int _leftToDown;
	int _rightToUp;
	int _rightToDown;

	Ogre::Vector2 _pos;
	Ogre::Vector2 _destinationPos;

	//methods
	void playerMoveLeft();
	void playerMoveRight();
	void playerMoveUp();
	void playerMoveDown();

	Ogre::Vector2 convertWorldPosToGridPos(Ogre::Vector3 pos);
	void updateAnimation(const Ogre::FrameEvent& evt);
};
#endif