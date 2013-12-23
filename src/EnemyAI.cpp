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
	_moveSpeed = 150;
	_direction = AI_UP_DIRECTION;
	lastDirection=_direction;
	_isTurning = 1;
}
void EnemyAI::Update(const Ogre::FrameEvent& evt,GameMap* map)
{
	gameMap=map;
	updateMyPosition();
	myMotionType= myBrain.Update(myMotionType,map,myPosition);

	//test
	//myMotionType=AI_LEFT;

	Transform(evt,myMotionType);

	CheckBonusCollision();

}

//¼ì²éÅö×² Ïû³ýbonus
void EnemyAI::CheckBonusCollision()
{
	for(int x=0;x<MAP_WIDTH;x++ )
	{
		for (int y=0;y<MAP_HEIGHT;y++)
		{
			gridType tempType=gameMap->getMapTypeAtGridPos(x,y);


			if(myPosition.x==x&&myPosition.y==y)
			{
				if(tempType==GRID_ADD_BOMB||
					tempType==GRID_ADD_HEALTH||
					tempType==GRID_ADD_SPEED)
				{
					gameMap->setMapTypeAtGridPos(x,y,GRID_NORMAL);
					if(tempType==GRID_ADD_BOMB)
					{

					}
					else if(tempType==GRID_ADD_HEALTH)
					{

					}
					else if(tempType==GRID_ADD_SPEED)
					{
						_moveSpeed+=50;
					}
				}
			}

		}
	}
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
		_direction=AI_LEFT_DIRECTION;
		transVector.x-= _moveSpeed;
		break;
	case AI_RIGHT_AND_BOMB:
		//set bomb

	case AI_RIGHT:
		//
		//playerMoveRight();
		_direction=AI_RIGHT_DIRECTION;
		transVector.x+=  _moveSpeed;
		break;
	case AI_UP_AND_BOMB:
		//set bomb

	case AI_UP:
		//
		//playerMoveUp();
		transVector.z -= _moveSpeed;
		_direction=AI_UP_DIRECTION;
		break;
	case AI_DOWN_AND_BOMB:
		//set bomb

	case AI_DOWN:
		//
		//playerMoveDown();
		_direction=AI_DOWN_DIRECTION;
		transVector.z += _moveSpeed;
		break;
	case  AI_BOMB:
		break;

	}




	Rotate();

	//update
	//updateAnimation(evt);
	this->_Node->translate(transVector * evt.timeSinceLastFrame,Ogre::Node::TS_WORLD);
}
void EnemyAI::Rotate()
{
	this->_Node->yaw( Ogre::Degree( lastDirection*90-_direction*90 ) );
	lastDirection=_direction;
}

Ogre::Vector2 EnemyAI::convertWorldPosToGridPos(Ogre::Vector3 pos)
{
	Ogre::Vector2 rtn;
	rtn.x = (int)pos.x/120 + 12;
	rtn.y = (int)pos.z/120 + 10;
	return rtn;
}