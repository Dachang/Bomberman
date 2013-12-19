#include "GameBomb.h"

GameBomb::GameBomb(Ogre::Entity* entity,Ogre::SceneNode* node,Ogre::SceneManager *sceneMgr,Ogre::Camera *camera,OIS::Keyboard *keyboard)
	:GameObject(entity,node,sceneMgr,camera,keyboard)
{
	Start();
}

GameBomb::~GameBomb(void)
{

}

void GameBomb::Start(void)
{
	_timeToExplode = 5.0;
}

void GameBomb::Update(const Ogre::FrameEvent& evt,GameMap* map)
{
	countDown(evt);
	if (_timeToExplode <= 0)
	{
		explode(map);
	}
}

void GameBomb::setPosition(Ogre::Vector2 pos)
{
	_bombPosition = pos;
}

void GameBomb::countDown(const Ogre::FrameEvent& evt)
{
	_timeToExplode -= evt.timeSinceLastFrame;
}

void GameBomb::explode(GameMap* map)
{
	map->setMapTypeAtGridPos(_bombPosition.x,_bombPosition.y,GRID_NORMAL);
}