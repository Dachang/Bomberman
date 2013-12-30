#include "MenuScene.h"

MenuScene::MenuScene(void)
{

}


MenuScene::~MenuScene(void)
{
}

void MenuScene::createSceneMgr(Ogre::Root *_root)
{
	mSceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "MenuSceneMgr");
}

void MenuScene::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("MenuCamera");

	mCamera->setPosition(Ogre::Vector3(0,500,0));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,-80,80));
	mCamera->setNearClipDistance(5);
}


void MenuScene::createScene(void)
{
	// title
	Ogre::Entity* title = mSceneMgr->createEntity("boomb_title", "cube.mesh");
	title->setMaterialName("Examples/terrain_texture");
	title->setCastShadows(false);
	Ogre::SceneNode* titleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("boomb_title");
	titleNode->setPosition(0, 0, 100);
	//Ogre::Quaternion q1 = titleNode->getOrientation();
	//Ogre::Quaternion q2(Ogre::Degree(45), Ogre::Vector3::UNIT_X);
	//titleNode->setOrientation(q1*q2);
	titleNode->attachObject(title);

	// Start text
	//Ogre::Entity* startText = mSceneMgr->createEntity("boomb_starttext", "cube.mesh");
	//Ogre::SceneNode* textNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("boomb_starttext");
	//textNode->setPosition(0, 800, 900);
	//textNode->attachObject(startText);

	// background
	Ogre::SceneNode *backgroundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("boomb_background");
	backgroundNode->setPosition(0, 0, 0);

	//Ogre::Entity *grass = mSceneMgr->createEntity("boomb_grass", "cube.mesh");
	//grass->setMaterialName("Examples/Rockwall");
	//grass->setCastShadows(true);
	//Ogre::SceneNode *grassNode = backgroundNode->createChildSceneNode("boomb_grass");
	//grassNode->setPosition(-300, 0, 1200);
	//grassNode->setScale(2, 2, 2);
	//grassNode->attachObject(grass);

	//Ogre::Entity *tree = mSceneMgr->createEntity("boomb_tree", "cube.mesh");
	//tree->setMaterialName("Examples/Rockwall");
	//tree->setCastShadows(true);
	//Ogre::SceneNode *treeNode = backgroundNode->createChildSceneNode("boomb_tree");
	//treeNode->setPosition(-100, 0, -100);
	//treeNode->setScale(10, 10, 10);
	//treeNode->attachObject(tree);

	//Ogre::Entity *building = mSceneMgr->createEntity("boomb_building", "cube.mesh");
	//Ogre::SceneNode *buildingNode = backgroundNode->createChildSceneNode("boomb_building");
	//building->setMaterialName("Examples/Rockwall");
	//building->setCastShadows(true);
	//buildingNode->setPosition(400, 0, -400);
	//buildingNode->setScale(500,500,500);
	//q1 = buildingNode->getOrientation();
	//q2 = Ogre::Quaternion(Ogre::Degree(45), Ogre::Vector3::UNIT_Y);
	//buildingNode->setOrientation(q1*q2);
	//buildingNode->attachObject(building);

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground2", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 3000, 3000, 20, 20, true, 1, 12.5, 12.5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground2");
	Ogre::SceneNode* planeNode= mSceneMgr->getRootSceneNode()->createChildSceneNode();
	planeNode->attachObject(entGround);
	planeNode->setPosition(-570,0,-100);
	planeNode->setScale(3.15,3.15,3.15);
	entGround->setMaterialName("Examples/BumpyMetal");
	entGround->setCastShadows(false);
	// Set Fog

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("directlight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
	l->setDirection(Ogre::Vector3(-1, -1, -1));

	// Create Overlay

	this->createAnimation();
}

void MenuScene::createAnimation(void)
{

	Ogre::SceneNode *titleNode = static_cast<Ogre::SceneNode *>(mSceneMgr->getRootSceneNode()->getChild("boomb_title"));
	Ogre::Animation *anim = mSceneMgr->createAnimation("title_animation", 3);
	anim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
	Ogre::NodeAnimationTrack *track = anim->createNodeTrack(0, titleNode);
	Ogre::TransformKeyFrame *key = track->createNodeKeyFrame(0);
	key->setTranslate(Ogre::Vector3(0,0,100));
	Ogre::Quaternion q1 = titleNode->getOrientation();
	Ogre::Quaternion q2(Ogre::Degree(45), Ogre::Vector3::UNIT_X);
	key->setRotation(q2);
	key = track->createNodeKeyFrame(1.5);
	key->setTranslate(Ogre::Vector3(0,0,120));
	key->setRotation(q2);
	key = track->createNodeKeyFrame(3);
	key->setTranslate(Ogre::Vector3(0,0,100));
	key->setRotation(q2);

	mTitleAnimState = mSceneMgr->createAnimationState("title_animation");
	mTitleAnimState->setEnabled(true);
	mTitleAnimState->setLoop(true);

	Ogre::SceneNode *backgroundNode = static_cast<Ogre::SceneNode *>(mSceneMgr->getRootSceneNode()->getChild("boomb_background"));
	anim = mSceneMgr->createAnimation("background_animation", 30);
	anim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
	track = anim->createNodeTrack(0, backgroundNode);
	key = track->createNodeKeyFrame(0);
	key->setTranslate(Ogre::Vector3(20,0,20));
	key = track->createNodeKeyFrame(15);
	key->setTranslate(Ogre::Vector3(-20,0,-20));
	key = track->createNodeKeyFrame(30);
	key->setTranslate(Ogre::Vector3(20,0,20));
	mBackgroundAnimState = mSceneMgr->createAnimationState("background_animation");
	mBackgroundAnimState->setEnabled(true);
	mBackgroundAnimState->setLoop(true);
}