#include "GameBomb.h"

GameBomb::GameBomb(Ogre::Entity* entity,Ogre::SceneNode* node,Ogre::SceneManager *sceneMgr,Ogre::Camera *camera,OIS::Keyboard *keyboard,int BombLevel=3)
	:GameObject(entity,node,sceneMgr,camera,keyboard)
{
	_bombLevel=BombLevel;
	Start();
}

GameBomb::~GameBomb(void)
{

}

void GameBomb::Start(void)
{
	_timeToExplode = 4.0;
	_explodeDuration = 0.5;
}

bool GameBomb::Update(const Ogre::FrameEvent& evt,GameMap* map)
{
	countDown(evt);
	if (_timeToExplode <= 0)
	{
		explode(map);
		calculateDuration(evt);
		if (_explodeDuration <= 0)
		{
			revertPowerZone(map);
			return true;
		}
	}
	return false;
}

void GameBomb::setPosition(Ogre::Vector2 pos)
{
	_bombPosition = pos;
}

void GameBomb::countDown(const Ogre::FrameEvent& evt)
{
	_timeToExplode -= evt.timeSinceLastFrame;
}

void GameBomb::calculateDuration(const Ogre::FrameEvent& evt)
{
	_explodeDuration -= evt.timeSinceLastFrame;
}

void GameBomb::explode(GameMap* map)
{
	map->setMapTypeAtGridPos(_bombPosition.x,_bombPosition.y,GRID_NORMAL);
	expandPowerZone(map);
}

void GameBomb::expandPowerZone(GameMap* map)
{
	for (int i=1; i<_bombLevel; i++)
	{
		if (_bombPosition.x - i >=0)
		{
			map->setMapTypeAtGridPos(_bombPosition.x - i,_bombPosition.y,GRID_BOMB_POWER);
		}
		if (_bombPosition.x + i < 25)
		{
			map->setMapTypeAtGridPos(_bombPosition.x + i,_bombPosition.y,GRID_BOMB_POWER);
		}
		if (_bombPosition.y - i >=0)
		{
			map->setMapTypeAtGridPos(_bombPosition.x,_bombPosition.y - i,GRID_BOMB_POWER);
		}
		if(_bombPosition.y + i < 20)
		{
			map->setMapTypeAtGridPos(_bombPosition.x,_bombPosition.y + i,GRID_BOMB_POWER);
		}
	}
}

void GameBomb::revertPowerZone(GameMap* map)
{
	for (int i=1; i<_bombLevel; i++)
	{
		map->setMapTypeAtGridPos(_bombPosition.x - i,_bombPosition.y,GRID_NORMAL);
		map->setMapTypeAtGridPos(_bombPosition.x + i,_bombPosition.y,GRID_NORMAL);
		map->setMapTypeAtGridPos(_bombPosition.x,_bombPosition.y - i,GRID_NORMAL);
		map->setMapTypeAtGridPos(_bombPosition.x,_bombPosition.y + i,GRID_NORMAL);
	}
}