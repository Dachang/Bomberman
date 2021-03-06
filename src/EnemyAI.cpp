#include "EnemyAI.h"

audiere::AudioDevicePtr device5(audiere::OpenDevice());
audiere::OutputStreamPtr enemyDeadStream(audiere::OpenSound(device5,"Resources/Sound/killEnemy.wav",false));

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
	MOVESPEED=150;
	myMotionType=AI_WAIT;
	_moveSpeed =MOVESPEED;
	_direction = AI_UP_DIRECTION;
	lastDirection=_direction;
	_isTurning = 1;


	_bombAvailableNumber = 3;
	_bombLevel=3;
	_playerIsPlacingBomb = false;

	healthValue=3;
	unbreakable=false;
	unbreakableDuration=0;

	hasPlayedDeadSound = false;
}
bool EnemyAI::Update(const Ogre::FrameEvent& evt,GameMap* map)
{
	gameMap=map;
	updateMyPosition();

	if(healthValue<=0)
	{
		updateBomb(evt,map);

		return false;
	}
	else
	{
		myMotionType= myBrain.Update(myMotionType,map,myPosition);

		Transform(map,evt,myMotionType);
		updateBomb(evt,map);

		UpdateUnbreakable(evt);

		CheckBombCollision(map);

		CheckBonusCollision(map);
		return true;
	}

}

void EnemyAI::UpdateUnbreakable(const Ogre::FrameEvent& evt)
{
	if (unbreakable)
	{
		unbreakableDuration -= evt.timeSinceLastFrame;
		if (unbreakableDuration <= 0)
		{
			unbreakable = false;
			unbreakableDuration = 2.0;
		}
	}

}
void EnemyAI::CheckBombCollision(GameMap* gameMap)
{
	if(!unbreakable)
	{
		gridType tempType=gameMap->getMapTypeAtGridPos(myPosition.x,myPosition.y);
		if(tempType==GRID_BOMB_POWER)
		{
			healthValue--;
			unbreakable=true;
		}

	}
}

void EnemyAI::updateBomb(const Ogre::FrameEvent& evt,GameMap* gameMap)
{
	if(_bombList.size()!=0)
	{
		for (std::map<int,GameBomb*>::iterator iter = _bombList.begin();iter != _bombList.end();)
		{
			//explode => true
			//iter--;
			if(iter->second->Update(evt,gameMap))
			{
				iter=_bombList.erase(iter);
				//iter = _bombList.end();
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

//�����ײ ����bonus
void EnemyAI::CheckBonusCollision(GameMap* gameMap)
{
	/*for(int x=0;x<MAP_WIDTH;x++ )
	{
	for (int y=0;y<MAP_HEIGHT;y++)
	{*/
	gridType tempType=gameMap->getMapTypeAtGridPos(myPosition.x,myPosition.y);



	if(tempType==GRID_ADD_BOMB||
		tempType==GRID_ADD_HEALTH||
		tempType==GRID_ADD_SPEED)
	{
		gameMap->setMapTypeAtGridPos(myPosition.x,myPosition.y,GRID_NORMAL);
		if(tempType==GRID_ADD_BOMB)
		{
			_bombAvailableNumber++;
			if(_bombAvailableNumber>5)
			{
				_bombAvailableNumber=5;
			}
		}
		else if(tempType==GRID_ADD_HEALTH)
		{
			healthValue++;
			if(healthValue>3)
			{
				healthValue=3;
			}
		}
		else if(tempType==GRID_ADD_SPEED)
		{
			if(_moveSpeed<400)
			{
				_moveSpeed+=80;
			}
			
		}
		else if(tempType==GRID_ADD_BOMB_POWER)
		{
			_bombLevel+=2;
			if(_bombLevel>7)
			{
				_bombLevel=7;
			}
		}

	}




}
void EnemyAI::updateMyPosition()
{
	if(this->_Node!=NULL)
	{
		myPosition=convertWorldPosToGridPos(this->_Node->getPosition());
	}
	
}
void EnemyAI::AddBomb(GameMap* gameMap)

{
	if(gameMap->getMapTypeAtGridPos(convertWorldPosToGridPos(_Node->getPosition()).x,convertWorldPosToGridPos(_Node->getPosition()).y)==GRID_BOMB)
	{
		//nothing
	}
	else if(gameMap->getMapTypeAtGridPos(convertWorldPosToGridPos(_Node->getPosition()).x,convertWorldPosToGridPos(_Node->getPosition()).y)!=GRID_NOTHING)
	{
		if(_bombAvailableNumber>0)
		{
			GameBomb* bomb = new GameBomb(_Entity,_Node,_SceneMgr,_Camera,_Keyboard,_bombLevel);
			gameMap->setMapTypeAtGridPos(convertWorldPosToGridPos(_Node->getPosition()).x,convertWorldPosToGridPos(_Node->getPosition()).y,GRID_BOMB);
			bomb->setPosition(Ogre::Vector2(convertWorldPosToGridPos(_Node->getPosition()).x,convertWorldPosToGridPos(_Node->getPosition()).y));

			bomb->bombIndex = _bombIndex;
			_bombList.insert(std::pair<int,GameBomb*>(_bombIndex++,bomb));
			_bombAvailableNumber--;
		}
	}
}
void EnemyAI::Transform(GameMap* gameMap,const Ogre::FrameEvent& evt,EnemyAIMotionType type)
{
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;

	switch(type)
	{
	case AI_WAIT:
		break;
	case AI_LEFT_AND_BOMB:
		//set bomb
		AddBomb(gameMap);
	case AI_LEFT:
		//
		//playerMoveLeft();
		_direction=AI_LEFT_DIRECTION;
		transVector.x-= _moveSpeed;
		break;
	case AI_RIGHT_AND_BOMB:
		//set bomb
		AddBomb(gameMap);
	case AI_RIGHT:
		//
		//playerMoveRight();
		_direction=AI_RIGHT_DIRECTION;
		transVector.x+=  _moveSpeed;
		break;
	case AI_UP_AND_BOMB:
		//set bomb
		AddBomb(gameMap);
	case AI_UP:
		//
		//playerMoveUp();
		transVector.z -= _moveSpeed;
		_direction=AI_UP_DIRECTION;
		break;
	case AI_DOWN_AND_BOMB:
		//set bomb
		AddBomb(gameMap);
	case AI_DOWN:
		//
		//playerMoveDown();
		_direction=AI_DOWN_DIRECTION;
		transVector.z += _moveSpeed;
		break;
	case  AI_BOMB:
		AddBomb(gameMap);
		break;

	}




	Rotate();

	//update
	//updateAnimation(evt);
	transVector=transVector *evt.timeSinceLastFrame;
	transVector.x=(int)transVector.x;
	transVector.y=(int)transVector.y;
	transVector.z=(int)transVector.z;
	this->_Node->translate(transVector,Ogre::Node::TS_WORLD);
	//this->_Node->translate(transVector ,Ogre::Node::TS_WORLD);
}
void EnemyAI::Rotate()
{
	if(this->_Node!=NULL)
	{
	this->_Node->yaw( Ogre::Degree( lastDirection*90-_direction*90 ) );
	lastDirection=_direction;
	}
}
void EnemyAI::setVisible(bool isVisible)
{
	this->_Node->setVisible(isVisible);
	if ((!isVisible) && (!hasPlayedDeadSound))
	{
		playEnemyDeadSound();
		hasPlayedDeadSound = true;
	}
}
void EnemyAI::setDead()
{
	healthValue=0;
}

Ogre::Vector2 EnemyAI::convertWorldPosToGridPos(Ogre::Vector3 pos)
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

void EnemyAI::playEnemyDeadSound()
{
	enemyDeadStream->setRepeat(false);
	enemyDeadStream->setVolume(1.0);
	enemyDeadStream->play();
}