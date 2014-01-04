#include "GameBomb.h"

audiere::AudioDevicePtr device2(audiere::OpenDevice());
audiere::OutputStreamPtr bombExplodeStream(audiere::OpenSound(device2,"Resources/Sound/explode.wav",false));

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
	_isExplode = false;
}

bool GameBomb::Update(const Ogre::FrameEvent& evt,GameMap* map)
{
	countDown(evt);
	if ((_timeToExplode <= 0) || map->getMapTypeAtGridPos(_bombPosition.x,_bombPosition.y)==GRID_BOMB_POWER)
	{
		if (map->getMapTypeAtGridPos(_bombPosition.x,_bombPosition.y)==GRID_BOMB_POWER)
		{
			_timeToExplode = 0;
		}
		if (!_isExplode)
		{
			explode(map,evt);
			_isExplode = true;
		}
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

void GameBomb::explode(GameMap* map,const Ogre::FrameEvent& evt)
{
	bombExplodeStream->setRepeat(false);
	bombExplodeStream->setVolume(1.0);
	bombExplodeStream->play();
	map->setMapTypeAtGridPos(_bombPosition.x,_bombPosition.y,GRID_NORMAL);
	map->setParticleEffectAtGrid(_bombPosition.x,_bombPosition.y,evt,PARTICLE_BOMB);
	expandPowerZone(map);
}

void GameBomb::expandPowerZone(GameMap* map)
{
	for (int i=1; i<_bombLevel; i++)
	{
		if (_bombPosition.x - i >=0)
		{
			if ((map->getMapTypeAtGridPos(_bombPosition.x-i,_bombPosition.y)==GRID_WALL) || (map->getMapTypeAtGridPos(_bombPosition.x-i,_bombPosition.y)==GRID_BOMB_POWER))
			{
				break;
			}
			else
				map->setMapTypeAtGridPos(_bombPosition.x - i,_bombPosition.y,GRID_BOMB_POWER);
		}
	}
	for (int j=1; j<_bombLevel; j++)
	{
		if (_bombPosition.x + j < 25)
		{
			if ((map->getMapTypeAtGridPos(_bombPosition.x+j,_bombPosition.y)==GRID_WALL) || (map->getMapTypeAtGridPos(_bombPosition.x+j,_bombPosition.y)==GRID_BOMB_POWER))
			{
				break;
			}
			else
				map->setMapTypeAtGridPos(_bombPosition.x + j,_bombPosition.y,GRID_BOMB_POWER);
		}
	}
	for (int k=1; k<_bombLevel; k++)
	{
		if (_bombPosition.y - k >=0)
		{
			if ((map->getMapTypeAtGridPos(_bombPosition.x,_bombPosition.y - k)==GRID_WALL) || (map->getMapTypeAtGridPos(_bombPosition.x,_bombPosition.y - k)==GRID_BOMB_POWER))
			{
				break;
			}
			else
				map->setMapTypeAtGridPos(_bombPosition.x,_bombPosition.y - k,GRID_BOMB_POWER);
		}
	}
	for (int m=1; m<_bombLevel; m++)
	{
		if(_bombPosition.y + m < 20)
		{
			if ((map->getMapTypeAtGridPos(_bombPosition.x,_bombPosition.y + m)==GRID_WALL) || (map->getMapTypeAtGridPos(_bombPosition.x,_bombPosition.y + m)==GRID_BOMB_POWER))
			{
				break;
			}
			else
				map->setMapTypeAtGridPos(_bombPosition.x,_bombPosition.y + m,GRID_BOMB_POWER);
		}
	}
}

void GameBomb::revertPowerZone(GameMap* map)
{
	for (int i=1; i<_bombLevel; i++)
	{
		if (_bombPosition.x - i >=0)
		{
			if (map->getMapTypeAtGridPos(_bombPosition.x-i,_bombPosition.y)==GRID_BOMB_POWER)
			{
				map->setMapTypeAtGridPos(_bombPosition.x - i,_bombPosition.y,GRID_NORMAL);
				map->destroyParticleEffectAtGrid(_bombPosition.x,_bombPosition.y);
			}
		}
	}
	for (int j=1; j<_bombLevel; j++)
	{
		if (_bombPosition.x + j < 25)
		{
			if (map->getMapTypeAtGridPos(_bombPosition.x+j,_bombPosition.y)==GRID_BOMB_POWER)
			{
				map->setMapTypeAtGridPos(_bombPosition.x + j,_bombPosition.y,GRID_NORMAL);
				map->destroyParticleEffectAtGrid(_bombPosition.x,_bombPosition.y);
			}
		}
	}
	for (int k=1; k<_bombLevel; k++)
	{
		if (_bombPosition.y - k >=0)
		{
			if (map->getMapTypeAtGridPos(_bombPosition.x,_bombPosition.y - k)==GRID_BOMB_POWER)
			{
				map->setMapTypeAtGridPos(_bombPosition.x,_bombPosition.y - k,GRID_NORMAL);
				map->destroyParticleEffectAtGrid(_bombPosition.x,_bombPosition.y);
			}	
		}
	}
	for (int m=1; m<_bombLevel; m++)
	{
		if(_bombPosition.y + m < 20)
		{
			if (map->getMapTypeAtGridPos(_bombPosition.x,_bombPosition.y + m)==GRID_BOMB_POWER)
			{
				map->setMapTypeAtGridPos(_bombPosition.x,_bombPosition.y + m,GRID_NORMAL);
				map->destroyParticleEffectAtGrid(_bombPosition.x,_bombPosition.y);
			}
		}
	}
	map->destroyParticleEffectAtGrid(_bombPosition.x,_bombPosition.y);
}