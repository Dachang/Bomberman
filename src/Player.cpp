#include "Player.h"

#define  MOVESPEED 10;

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
	_healthValue = 3;
	_moveSpeed = MOVESPEED;
	_direction = UP_DIRECTION;
	_leftToUp = 1;
	_leftToRight = 1;
	_leftToDown = 1;
	_rightToDown = 1;
	_rightToUp = 1;
	_upToDown = 1;

	_pos = START_POSITION;
	_bombAvailableNumber = 3;
	_bombLevel=3;
	_playerIsPlacingBomb = false;
}
void Player::Update(const Ogre::FrameEvent& evt,GameMap* gameMap)
{
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	//get input
	if (_Keyboard->isKeyDown(OIS::KC_SPACE))
	{
		if (!_playerIsPlacingBomb)
		{
			if (_bombAvailableNumber > 0)
			{
				GameBomb* bomb = new GameBomb(_Entity,_Node,_SceneMgr,_Camera,_Keyboard,_bombLevel);
				//switch(_direction)
				//{
				//case LEFT_DIRECTION:
				//	gameMap->setMapTypeAtGridPos(convertWorldPosToGridPos(_Node->getPosition()).x+1,convertWorldPosToGridPos(_Node->getPosition()).y,GRID_BOMB);
				//	bomb->setPosition(Ogre::Vector2(convertWorldPosToGridPos(_Node->getPosition()).x+1,convertWorldPosToGridPos(_Node->getPosition()).y));
				//	break;
				//case RIGHT_DIRECTION:
				//	gameMap->setMapTypeAtGridPos(convertWorldPosToGridPos(_Node->getPosition()).x-1,convertWorldPosToGridPos(_Node->getPosition()).y,GRID_BOMB);
				//	bomb->setPosition(Ogre::Vector2(convertWorldPosToGridPos(_Node->getPosition()).x-1,convertWorldPosToGridPos(_Node->getPosition()).y));
				//	break;
				//case UP_DIRECTION:
				//	gameMap->setMapTypeAtGridPos(convertWorldPosToGridPos(_Node->getPosition()).x,convertWorldPosToGridPos(_Node->getPosition()).y+1,GRID_BOMB);
				//	bomb->setPosition(Ogre::Vector2(convertWorldPosToGridPos(_Node->getPosition()).x,convertWorldPosToGridPos(_Node->getPosition()).y+1));
				//	break;
				//case DOWN_DIRECTION:
				//	gameMap->setMapTypeAtGridPos(convertWorldPosToGridPos(_Node->getPosition()).x,convertWorldPosToGridPos(_Node->getPosition()).y-1,GRID_BOMB);
				//	bomb->setPosition(Ogre::Vector2(convertWorldPosToGridPos(_Node->getPosition()).x,convertWorldPosToGridPos(_Node->getPosition()).y-1));
				//	break;
				//default:
				//	break;
				//}
				gameMap->setMapTypeAtGridPos(convertWorldPosToGridPos(_Node->getPosition()).x,convertWorldPosToGridPos(_Node->getPosition()).y,GRID_BOMB);
				bomb->setPosition(Ogre::Vector2(convertWorldPosToGridPos(_Node->getPosition()).x,convertWorldPosToGridPos(_Node->getPosition()).y));
				bomb->bombIndex = _bombIndex;
				_bombList.insert(std::pair<int,GameBomb*>(_bombIndex++,bomb));
				_bombAvailableNumber--;
			}
			_playerIsPlacingBomb = true;
		}
	}
	else
	{
		_playerIsPlacingBomb = false;
	}


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

	//update
	updateAnimation(evt);

	transVector=transVector;// *evt.timeSinceLastFrame;
	transVector.x=(int)transVector.x;
	transVector.y=(int)transVector.y;
	transVector.z=(int)transVector.z;

	this->_Node->translate(transVector,Ogre::Node::TS_LOCAL);
	//this->_Node->translate(transVector ,Ogre::Node::TS_LOCAL);
	this->_pos = convertWorldPosToGridPos(this->_Node->getPosition());
	updatePlayerWithGrid(evt,gameMap);
	updateBomb(evt,gameMap);
	TurnBack(gameMap);
}



void Player::TurnBack(GameMap* gameMap)
{
	int x=_pos.x;
	int y=_pos.y;

	gridType gridtype= gameMap->getMapTypeAtGridPos(x,y);
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	if(gridtype==GRID_WALL)
	{
		switch(_direction)
		{
		case UP_DIRECTION:
			_pos.y++;
			transVector.z+=_moveSpeed*5;
			this->_Node->translate(transVector,Ogre::Node::TS_WORLD);
			break;
		case DOWN_DIRECTION:
			_pos.y--;
			transVector.z-=_moveSpeed*5;
			this->_Node->translate(transVector,Ogre::Node::TS_WORLD);
			break;
		case RIGHT_DIRECTION:
			_pos.x--;
			transVector.x-=_moveSpeed*5;
			this->_Node->translate(transVector,Ogre::Node::TS_WORLD);
			break;
		case LEFT_DIRECTION:
			_pos.x++;
			transVector.x+=_moveSpeed*5;
			this->_Node->translate(transVector,Ogre::Node::TS_WORLD);
			break;
		}
	}

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

	double xTemp=(pos.x-60)/120;

	if(xTemp>0)
	{
		rtn.x =  int((pos.x-60)/120+0.5) + 12;
	}
	else if (xTemp == 0)
	{
		rtn.x =  int((pos.x-60)/120) + 12;
	}
	else
	{
		rtn.x =  int((pos.x-60)/120-0.5) + 12;
	}
	double yTemp=(pos.z-60)/120;
	if(yTemp>0)
	{
		rtn.y = int((pos.z-60)/120+0.5) + 10;
	}
	else if (yTemp == 0)
	{
		rtn.y = int((pos.z-60)/120) + 10;
	}
	else
	{
		rtn.y = int((pos.z-60)/120-0.5) + 10;
	}
	//rtn.x =  int((pos.x-60)/120+0.5) + 12;
	//rtn.y = int((pos.z-60)/120+0.5) + 10;
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

void Player::updatePlayerWithGrid(const Ogre::FrameEvent& evt,GameMap* gameMap)
{
	_currentGridType = gameMap->getMapTypeAtGridPos(_pos.x,_pos.y);
	_lastGridType = gameMap->getLastMapTypeAtGridPos(_pos.x,_pos.y);
	_upGridType = gameMap->getMapTypeAtGridPos(_pos.x,_pos.y-1);
	_downGridType = gameMap->getMapTypeAtGridPos(_pos.x,_pos.y+1);
	_leftGridType = gameMap->getMapTypeAtGridPos(_pos.x-1,_pos.y);
	_rightGridType = gameMap->getMapTypeAtGridPos(_pos.x+1,_pos.y);

	switch (_currentGridType)
	{
	case GRID_ADD_HEALTH:
		break;
	case GRID_BOMB_POWER:
		if (!_unbreakable)
		{
			_healthValue--;
			_unbreakable = true;
		}
		break;

	case  GRID_WALL:
		//_moveSpeed = 0;
	default:
		break;
	}
	switch (_lastGridType)
	{
	case GRID_ADD_HEALTH:
		gameMap->setMapTypeAtGridPos(_pos.x,_pos.y,GRID_NORMAL);
		_healthValue++;
		break;
	case GRID_ADD_SPEED:
		gameMap->setMapTypeAtGridPos(_pos.x,_pos.y,GRID_NORMAL);
		_moveSpeed += 50;
		break;
	case GRID_BOMB_POWER:
		if (!_unbreakable)
		{
			_healthValue--;
			_unbreakable = true;
		}
		break;
	default:
		
		break;
	}
	switch(_upGridType)
	{
	case GRID_WALL:
		if (_direction == UP_DIRECTION)
		{
			_moveSpeed = 0;
		}
		else
			_moveSpeed = MOVESPEED;
		break;
	case GRID_DISTORYABLE_WALL:
		if (_direction == UP_DIRECTION)
		{
			_moveSpeed = 0;
		}
		else
			_moveSpeed = MOVESPEED;
		break;
	default:
		break;
	}
	switch(_downGridType)
	{
	case GRID_WALL:
		if (_direction == DOWN_DIRECTION)
		{
			_moveSpeed = 0;
		}
		else _moveSpeed = MOVESPEED;
		break;
	case GRID_DISTORYABLE_WALL:
		if (_direction == DOWN_DIRECTION)
		{
			_moveSpeed = 0;
		}
		else
			_moveSpeed = MOVESPEED;
		break;
	default:
		break;
	}
	switch(_leftGridType)
	{
	case GRID_WALL:
		if (_direction == LEFT_DIRECTION)
		{
			_moveSpeed = 0;
		}
		else _moveSpeed = MOVESPEED;
		break;
	case GRID_DISTORYABLE_WALL:
		if (_direction == LEFT_DIRECTION)
		{
			_moveSpeed = 0;
		}
		else
			_moveSpeed = MOVESPEED;
		break;
	default:
		break;
	}
	switch(_rightGridType)
	{
	case GRID_WALL:
		if (_direction == RIGHT_DIRECTION)
		{
			_moveSpeed = 0;
		}
		else _moveSpeed = MOVESPEED;
		break;
	case GRID_DISTORYABLE_WALL:
		if (_direction == RIGHT_DIRECTION)
		{
			_moveSpeed = 0;
		}
		else
			_moveSpeed = MOVESPEED;
		break;
	default:
		break;
	}
	updateUnbreakable(evt);
}
int Player::getPlayerHP()
{
	return _healthValue;
}

void Player::updateBomb(const Ogre::FrameEvent& evt,GameMap* gameMap)
{
	if(_bombList.size()!=0)
	{
	for (std::map<int,GameBomb*>::iterator iter = _bombList.begin();iter != _bombList.end();)
	{
		//explode => true
		if(iter->second->Update(evt,gameMap))
		{
			iter=_bombList.erase(iter);
			_bombAvailableNumber++;
			if(_bombList.size()==0)
			{
				break;
			}
		}
		else
		{
			iter++;
		}
		
	}
	}
}

void Player::updateUnbreakable(const Ogre::FrameEvent& evt)
{
	if (_unbreakable)
	{
		_unbreakableDuration -= evt.timeSinceLastFrame;
		if (_unbreakableDuration <= 0)
		{
			_unbreakable = false;
			_unbreakableDuration = 2.0;
		}
	}
}