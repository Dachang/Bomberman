#ifndef __GAMEBOMB_H_
#define __GAMEBOMB_H_
#include "GameObject.h"

class GameBomb : public GameObject
{
public:
	GameBomb(Ogre::Entity* entity,Ogre::SceneNode* node,Ogre::SceneManager *sceneMgr,Ogre::Camera *camera,OIS::Keyboard *keyboard);
	~GameBomb(void);
	virtual void Start(void);
	virtual void Update(const Ogre::FrameEvent& evt);
protected:
private:
	Ogre::Vector2 bombPosition;
	float timeToExplode;
	void countDown(const Ogre::FrameEvent& evt);
	void explode();
};

#endif