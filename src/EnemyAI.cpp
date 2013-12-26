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
	_moveSpeed =100;
	_direction = AI_UP_DIRECTION;
	lastDirection=_direction;
	_isTurning = 1;


	_bombAvailableNumber = 3;
	_bombLevel=3;
	_playerIsPlacingBomb = false;
}
void EnemyAI::Update(const Ogre::FrameEvent& evt,GameMap* map)
{
	gameMap=map;
	updateMyPosition();
	myMotionType= myBrain.Update(myMotionType,map,myPosition);

	//test
	//myMotionType=AI_LEFT;

	Transform(map,evt,myMotionType);
	updateBomb(evt,map);
	CheckBonusCollision(map);

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

//¼ì²éÅö×² Ïû³ýbonus
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
void EnemyAI::updateMyPosition()
{
	myPosition=convertWorldPosToGridPos(this->_Node->getPosition());
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
	this->_Node->yaw( Ogre::Degree( lastDirection*90-_direction*90 ) );
	lastDirection=_direction;
}

Ogre::Vector2 EnemyAI::convertWorldPosToGridPos(Ogre::Vector3 pos)
{
	Ogre::Vector2 rtn;
	rtn.x = (int)((pos.x-60)/120) + 12;
	rtn.y = (int)((pos.z-60)/120) + 10;
	return rtn;
}