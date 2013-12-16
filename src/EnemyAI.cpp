#include "EnemyAI.h"

EnemyAI::EnemyAI(Ogre::Entity* entity,Ogre::SceneNode* node,Ogre::SceneManager *sceneMgr,Ogre::Camera *camera,OIS::Keyboard *keyboard)
	:GameObject(entity,node,sceneMgr, camera, keyboard)
{
	Start();
}
EnemyAI::~EnemyAI(void)
{

}

void EnemyAI::Start(void)
{
	myMotionType=AI_WAIT;
	_moveSpeed = 250;
	_direction = AI_UP_DIRECTION;
	_isTurning = 1;
}
void EnemyAI::Update(const Ogre::FrameEvent& evt,GameMap* map)
{
	updateMyPosition();
	myMotionType= myBrain.Update(myMotionType,map,myPosition);

	Transform(evt,myMotionType);
}
void EnemyAI::updateMyPosition()
{
	myPosition=convertWorldPosToGridPos(this->_Node->getPosition());
}
void EnemyAI::Transform(const Ogre::FrameEvent& evt,EnemyAIMotionType type)
{
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	
	switch(type)
	{
		case AI_WAIT:
			break;
		case AI_LEFT_AND_BOMB:
			//set bomb

		case AI_LEFT:
			//
			//playerMoveLeft();
			transVector.x-= _moveSpeed;
			break;
		case AI_RIGHT_AND_BOMB:
			//set bomb

		case AI_RIGHT:
			//
			//playerMoveRight();
			transVector.x+=  _moveSpeed;
			break;
		case AI_UP_AND_BOMB:
			//set bomb

		case AI_UP:
			//
			//playerMoveUp();
			transVector.z -= _moveSpeed;
			break;
		case AI_DOWN_AND_BOMB:
			//set bomb

		case AI_DOWN:
			//
			//playerMoveDown();
			transVector.z += _moveSpeed;
			break;
		case  AI_BOMB:
			break;

	}






	//update
	//updateAnimation(evt);
	this->_Node->translate(transVector * evt.timeSinceLastFrame,Ogre::Node::TS_WORLD);
}

void EnemyAI::playerMoveLeft()
{
	switch (_direction)
	{
	case AI_RIGHT_DIRECTION:
		this->_Node->yaw(Ogre::Degree(180));
		break;
	case AI_UP_DIRECTION:
		this->_Node->yaw(Ogre::Degree(90));
		break;
	case AI_DOWN_DIRECTION:
		this->_Node->yaw(Ogre::Degree(-90));
		break;
	default:
		break;
	}
	_direction = AI_LEFT_DIRECTION;
}

void EnemyAI::playerMoveRight()
{
	switch(_direction)
	{
	case AI_LEFT_DIRECTION:          
		this->_Node->yaw(Ogre::Degree(180));
		_isTurning = -1;
		break;
	case AI_UP_DIRECTION:
		this->_Node->yaw(Ogre::Degree(-90));
		break;
	case AI_DOWN_DIRECTION:
		this->_Node->yaw(Ogre::Degree(90));
		break;
	default:
		break;
	}
	_direction = AI_RIGHT_DIRECTION;
}

void EnemyAI::playerMoveUp()
{
	switch(_direction)
	{
	case AI_DOWN_DIRECTION:
		this->_Node->yaw(Ogre::Degree(180));
		break;
	case AI_LEFT_DIRECTION:
		this->_Node->yaw(Ogre::Degree(-90));
		break;
	case AI_RIGHT_DIRECTION:
		this->_Node->yaw(Ogre::Degree(90));
		break;
	default:
		break;
	}
	_direction = AI_UP_DIRECTION;
}

void EnemyAI::playerMoveDown()
{
	switch (_direction)
	{
	case AI_UP_DIRECTION:
		this->_Node->yaw(Ogre::Degree(180));
		_isTurning = -1;
		break;
	case AI_LEFT_DIRECTION:
		this->_Node->yaw(Ogre::Degree(90));
		break;
	case AI_RIGHT_DIRECTION:
		this->_Node->yaw(Ogre::Degree(-90));
		break;
	default:
		break;
	}
	_direction = AI_DOWN_DIRECTION;
}


Ogre::Vector2 EnemyAI::convertWorldPosToGridPos(Ogre::Vector3 pos)
{
	Ogre::Vector2 rtn;
	rtn.x = (int)pos.x/120 + 12;
	rtn.y = (int)pos.z/120 + 10;
	return rtn;
}