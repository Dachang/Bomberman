#include "GameObject.h"
GameObject::GameObject(Ogre::Entity* entity,Ogre::SceneNode* node,Ogre::SceneManager *sceneMgr,Ogre::Camera *camera,OIS::Keyboard *keyboard)
{
	_Entity=entity;
	_Node=node;
	_Camera=camera;
	_SceneMgr=sceneMgr;
	_Keyboard=keyboard;
	Start();
}
GameObject::~GameObject(void)
{

}
void GameObject::Start(void)
{

}
void GameObject::Update(void)
{

}