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
	timeToExplode = 5.0;
}

void GameBomb::Update(const Ogre::FrameEvent& evt)
{
	countDown(evt);
	if (timeToExplode <= 0)
	{
		explode();
	}
}

void GameBomb::countDown(const Ogre::FrameEvent& evt)
{
	timeToExplode -= evt.timeSinceLastFrame;
}

void GameBomb::explode()
{

}