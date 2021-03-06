#ifndef __GAMEBOMB_H_
#define __GAMEBOMB_H_
#include "GameObject.h"
#include "GameMap.h"
#include <audiere.h>

class GameBomb : public GameObject
{
public:
	GameBomb(Ogre::Entity* entity,Ogre::SceneNode* node,Ogre::SceneManager *sceneMgr,Ogre::Camera *camera,OIS::Keyboard *keyboard,int BombLevel);
	~GameBomb(void);
	virtual void Start(void);
	virtual bool Update(const Ogre::FrameEvent& evt,GameMap* map);
	void setPosition(Ogre::Vector2 pos);
	int bombIndex;
	
protected:
private:
	Ogre::Vector2 _bombPosition;
	float _timeToExplode;
	float _explodeDuration;
	int _bombLevel;
	bool _isExplode;
	void countDown(const Ogre::FrameEvent& evt);
	void calculateDuration(const Ogre::FrameEvent& evt);
	void explode(GameMap* map,const Ogre::FrameEvent& evt);
	void expandPowerZone(GameMap* map);
	void revertPowerZone(GameMap* map);
};

#endif