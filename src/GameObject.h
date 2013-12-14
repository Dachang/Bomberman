#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include <Ogre.h>
#include <OIS.h>
//#include "GameScene.h"

class GameObject 
{
public:
	GameObject(Ogre::Entity* entity,Ogre::SceneNode* node,Ogre::SceneManager *sceneMgr,Ogre::Camera *camera,OIS::Keyboard *keyboard);
	~GameObject(void);
	virtual void Start(void);
	virtual void Update(void);
protected:

	Ogre::Entity *_Entity;
	Ogre::SceneNode *_Node;
	Ogre::SceneManager *_SceneMgr;
	Ogre::Camera *_Camera;
	OIS::Keyboard *_Keyboard;
	Ogre::AnimationState *_animationState;
};
#endif