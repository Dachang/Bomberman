#include "Player.h"

Player::Player(Ogre::Entity* entity,Ogre::SceneNode* node,Ogre::SceneManager *sceneMgr,Ogre::Camera *camera,OIS::Keyboard *keyboard)
	:GameObject(entity,node,sceneMgr, camera, keyboard)
{
	Start();
}
Player::~Player(void)
{

}

void Player::Start(void)
{
	_moveSpeed = 250;
	_direction = UP_DIRECTION;
	_leftToUp = 1;
	_leftToRight = 1;
	_leftToDown = 1;
	_rightToDown = 1;
	_rightToUp = 1;
	_upToDown = 1;

	_pos = START_POSITION;
}
void Player::Update(const Ogre::FrameEvent& evt,GameMap* gameMap)
{
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	//get input
	if (_Keyboard->isKeyDown(OIS::KC_RIGHT))
	{
		playerMoveRight();
		transVector.z -= _leftToRight * _moveSpeed;
		transVector.z += _rightToUp * _moveSpeed;
		transVector.z += _rightToDown * _moveSpeed;
	}
	else if (_Keyboard->isKeyDown(OIS::KC_LEFT))
	{
		playerMoveLeft();
		transVector.z += _leftToUp * _moveSpeed;
		transVector.z += _leftToRight * _moveSpeed;
		transVector.z += _leftToDown * _moveSpeed;
	}
	else if (_Keyboard->isKeyDown(OIS::KC_UP))
	{
		playerMoveUp();
		transVector.z -= _leftToUp * _moveSpeed;
		transVector.z -= _rightToUp * _moveSpeed;
		transVector.z += _upToDown * _moveSpeed;
	}
	else if (_Keyboard->isKeyDown(OIS::KC_DOWN))
	{
		playerMoveDown();
		transVector.z -= _leftToDown * _moveSpeed;
		transVector.z -= _rightToDown * _moveSpeed;
		transVector.z -= _upToDown * _moveSpeed;
	}
	else if (_Keyboard->isKeyDown(OIS::KC_SPACE))
	{
	}
	//update
	updateAnimation(evt);
	this->_Node->translate(transVector * evt.timeSinceLastFrame,Ogre::Node::TS_LOCAL);
	this->_pos = convertWorldPosToGridPos(this->_Node->getPosition());
	updatePlayerWithGrid(gameMap);
}

void Player::playerMoveLeft()
{
	switch (_direction)
	{
	case RIGHT_DIRECTION:
		this->_Node->yaw(Ogre::Degree(180));
		_leftToRight = -1;
		_leftToUp = 0;
		_leftToDown = 0;
		break;
	case UP_DIRECTION:
		this->_Node->yaw(Ogre::Degree(90));
		_leftToRight = 0;
		_leftToUp = -1;
		_leftToDown = 0;
		break;
	case DOWN_DIRECTION:
		this->_Node->yaw(Ogre::Degree(-90));
		_leftToRight = 0;
		_leftToUp = 0;
		_leftToDown = -1;
		break;
	default:
		break;
	}
	_direction = LEFT_DIRECTION;
}

void Player::playerMoveRight()
{
	switch(_direction)
	{
	case LEFT_DIRECTION:          
		this->_Node->yaw(Ogre::Degree(180));
		_leftToRight = 1;
		_rightToDown = 0;
		_rightToUp = 0;
		break;
	case UP_DIRECTION:
		this->_Node->yaw(Ogre::Degree(-90));
		_leftToRight = 0;
		_rightToDown = 0;
		_rightToUp = -1;
		break;
	case DOWN_DIRECTION:
		this->_Node->yaw(Ogre::Degree(90));
		_leftToRight = 0;
		_rightToDown = -1;
		_rightToUp = 0;
		break;
	default:
		break;
	}
	_direction = RIGHT_DIRECTION;
}

void Player::playerMoveUp()
{
	switch(_direction)
	{
	case DOWN_DIRECTION:
		this->_Node->yaw(Ogre::Degree(180));
		_upToDown = -1;
		_rightToUp = 0;
		_leftToUp = 0;
		break;
	case LEFT_DIRECTION:
		this->_Node->yaw(Ogre::Degree(-90));
		_upToDown = 0;
		_rightToUp = 0;
		_leftToUp = 1;
		break;
	case RIGHT_DIRECTION:
		this->_Node->yaw(Ogre::Degree(90));
		_upToDown = 0;
		_rightToUp = 1;
		_leftToUp = 0;
		break;
	default:
		break;
	}
	_direction = UP_DIRECTION;
}

void Player::playerMoveDown()
{
	switch (_direction)
	{
	case UP_DIRECTION:
		this->_Node->yaw(Ogre::Degree(180));
		_upToDown = 1;
		_leftToDown = 0;
		_rightToDown = 0;
		break;
	case LEFT_DIRECTION:
		this->_Node->yaw(Ogre::Degree(90));
		_upToDown = 0;
		_leftToDown = 1;
		_rightToDown = 0;
		break;
	case RIGHT_DIRECTION:
		this->_Node->yaw(Ogre::Degree(-90));
		_upToDown = 0;
		_leftToDown = 0;
		_rightToDown = 1;
		break;
	default:
		break;
	}
	_direction = DOWN_DIRECTION;
}

void Player::updateAnimation(const Ogre::FrameEvent& evt)
{
	if (this->_Keyboard->isKeyDown(OIS::KC_UP) || (this->_Keyboard->isKeyDown(OIS::KC_DOWN)) 
		|| (this->_Keyboard->isKeyDown(OIS::KC_LEFT)) || (this->_Keyboard->isKeyDown(OIS::KC_RIGHT)) )
	{
		this->_animationState = this->_Entity->getAnimationState("Walk");
	}
	else
	{
		this->_animationState = this->_Entity->getAnimationState("Idle2");
	}
	this->_animationState->setLoop(true);
	this->_animationState->setEnabled(true);
	this->_animationState->addTime(evt.timeSinceLastFrame);
}

Ogre::Vector2 Player::convertWorldPosToGridPos(Ogre::Vector3 pos)
{
	Ogre::Vector2 rtn;
	rtn.x = pos.x/120 + 12;
	rtn.y = pos.z/120 + 10;
	return rtn;
}

int Player::getPlayerXPos()
{
	return _pos.x;
}

int Player::getPlayerYPos()
{
	return _pos.y;
}

directionType Player::getPlayerDirection()
{
	return _direction;
}

void Player::updatePlayerWithGrid(GameMap* gameMap)
{
	_currentGridType = gameMap->getMapTypeAtGridPos(_pos.x,_pos.y);
	_upGridType = gameMap->getMapTypeAtGridPos(_pos.x,_pos.y-1);
	_downGridType = gameMap->getMapTypeAtGridPos(_pos.x,_pos.y+1);
	_leftGridType = gameMap->getMapTypeAtGridPos(_pos.x-1,_pos.y);
	_rightGridType = gameMap->getMapTypeAtGridPos(_pos.x+1,_pos.y);

	switch (_currentGridType)
	{
	case 0:
		break;
	default:
		break;
	}
	switch(_upGridType)
	{
	case 4:
		if (_direction == UP_DIRECTION)
		{
			_moveSpeed = 0;
		}
		else
			_moveSpeed = 250;
		break;
	default:
		break;
	}
	switch(_downGridType)
	{
	case 4:
		if (_direction == DOWN_DIRECTION)
		{
			_moveSpeed = 0;
		}
		else _moveSpeed = 250;
		break;
	default:
		break;
	}
	switch(_leftGridType)
	{
	case 4:
		if (_direction == LEFT_DIRECTION)
		{
			_moveSpeed = 0;
		}
		else _moveSpeed = 250;
		break;
	default:
		break;
	}
	switch(_rightGridType)
	{
	case 4:
		if (_direction == RIGHT_DIRECTION)
		{
			_moveSpeed = 0;
		}
		else _moveSpeed = 250;
		break;
	default:
		break;
	}
}