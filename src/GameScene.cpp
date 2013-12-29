#include "GameScene.h"

GameScene::GameScene(void)
{
	nonNPCAnimState = NULL;
	NPCAnimState = NULL;
	isSpaceKeyDown = false;
	bombIndex = 0;
	gameOver = false;
}

GameScene::~GameScene(void)
{
}

void GameScene::initGameData(void)
{
	nonNPCPlayer.playerType = NON_NPC;
	nonNPCPlayer.bombAvailable = 1;
	nonNPCPlayer.pos = map.nonNPCStartPos;
	nonNPCPlayer.power = 1;
	nonNPCPlayer.health = 3;
	nonNPCPlayer.invincible = 0.0f;
	nonNPCPlayer.speed = 0.3f;

	NPCPlayer.playerType = NPC;
	NPCPlayer.bombAvailable = 1;
	NPCPlayer.pos = map.NPCStartPos;
	NPCPlayer.power = 1;
	NPCPlayer.health = 3;
	NPCPlayer.invincible = 0.0f;
	NPCPlayer.speed = 0.3f;
}

void GameScene::createSceneMgr(Ogre::Root *_root)
{
	mSceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "MenuSceneMgr");

}
void GameScene::createInput(OIS::Keyboard* keyboard)
{
	mKeyboard=keyboard;
}
void GameScene::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("GameCamera");
	mCamera->setPosition(Ogre::Vector3(0,1600,2000));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
}

void GameScene::createScene(void)
{
#pragma region tempboard
	Ogre::Entity *tempBoard = mSceneMgr->createEntity("bomb_tempboard","cube.mesh");
	tempBoard->setMaterialName("Examples/Rockwall");
	Ogre::SceneNode *tempBoardNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("boomb_tempboard");
	//tempBoardNode->setScale(7.0,7.0,7.0);
	tempBoardNode->setPosition(0,0,0);
	Ogre::Quaternion q1 = tempBoardNode->getOrientation();
	Ogre::Quaternion q2(Ogre::Degree(90),Ogre::Vector3::UNIT_Y);
	tempBoardNode->setOrientation(q1*q2);
	tempBoardNode->attachObject(tempBoard);
#pragma endregion

	for(int x=0;x<15;++x)
	{
		for(int y=0;y<13;++y)
		{
			if(map.getMapAtPos(x,y)==MAP_DESTROYABLE)
			{
				Ogre::Entity *destroyableBlock = mSceneMgr->createEntity("cube.mesh");
				//destroyableBlock->setMaterialName("Examples/Rockwall");
				destroyableBlock->setCastShadows(true);
				mapNode[x][y] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
				mapNode[x][y]->setPosition(0,0,0);
				mapNode[x][y]->setPosition(getWorldCoord(Ogre::Vector2(x,y)));
				mapNode[x][y]->attachObject(destroyableBlock);
			}
			else if(map.getMapAtPos(x,y) == MAP_INDESTROYABLE)
			{
				Ogre::Entity *indestroyableBlock = mSceneMgr->createEntity("cube.mesh");
				//indestroyableBlock->setMaterialName("boomb_testInDestroyable");
				indestroyableBlock->setCastShadows(true);
				mapNode[x][y] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
				mapNode[x][y]->setPosition(0,0,0);
				mapNode[x][y]->setPosition(getWorldCoord(Ogre::Vector2(x,y)));
				mapNode[x][y]->attachObject(indestroyableBlock);
			}
		}
	}

	gameMap = new GameMap(mSceneMgr);

	//test player
	Ogre::Entity *playerEntity = mSceneMgr->createEntity("player","ninja.mesh");
	playerEntity->setCastShadows(false);
	Ogre::SceneNode* PlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("playerNode");
	Ogre::Vector2 testvector=Ogre::Vector2(20,5);
	PlayerNode->setPosition(getWorldCoord( testvector));
	PlayerNode->setScale(1.0,1.0,1.0);
	PlayerNode->attachObject(playerEntity);


	player=new Player(playerEntity,PlayerNode,mSceneMgr,mCamera,mKeyboard);

	//add 3 enemy
	//1
	Ogre::Entity *npc = mSceneMgr->createEntity("npcplayer1","ninja.mesh");
	npc->setCastShadows(false);
	NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("npcplayerNode1");
	NPCPlayerNode->setScale(2.0,2.0,2.0);
	Ogre::Vector2 vectorPosition=Ogre::Vector2(5,5);
	NPCPlayerNode->setPosition(getWorldCoord(vectorPosition));
	NPCPlayerNode->attachObject(npc);

	//
	EnemyAI* tempEnemy=new EnemyAI(npc,NPCPlayerNode,mSceneMgr,mCamera,mKeyboard);

	//push
	enemyList.push_back(tempEnemy);
	//2
	npc = mSceneMgr->createEntity("npcplayer2","ninja.mesh");
	npc->setCastShadows(false);
	NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("npcplayerNode2");
	NPCPlayerNode->setScale(2.0,2.0,2.0);
	vectorPosition=Ogre::Vector2(15,15);
	NPCPlayerNode->setPosition(getWorldCoord(vectorPosition));
	//NPCPlayerNode->yaw( Ogre::Degree( -90 ) );
	NPCPlayerNode->attachObject(npc);

	tempEnemy=new EnemyAI(npc,NPCPlayerNode,mSceneMgr,mCamera,mKeyboard);
	enemyList.push_back(tempEnemy);
	//3
	npc = mSceneMgr->createEntity("npcplayer3","ninja.mesh");
	npc->setCastShadows(false);
	NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("npcplayerNode3");
	NPCPlayerNode->setScale(2.0,2.0,2.0);
	vectorPosition=Ogre::Vector2(5,13);
	NPCPlayerNode->setPosition(getWorldCoord(vectorPosition));
	NPCPlayerNode->attachObject(npc);
	tempEnemy=new EnemyAI(npc,NPCPlayerNode,mSceneMgr,mCamera,mKeyboard);
	enemyList.push_back(tempEnemy);

	/*Ogre::Entity* bonus = mSceneMgr->createEntity("bonus1","cube.mesh");
	bonus->setCastShadows(false);
	Ogre::SceneNode* bonusNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("bonus1");
	bonusNode->setScale(0.7,0.7,0.7);
	vectorPosition=Ogre::Vector2(3,15);
	Ogre::Vector3 tempVector=getWorldCoord(vectorPosition);
	tempVector.y+=50;
	bonusNode->setPosition(tempVector);
	bonusNode->attachObject(bonus);

	bonus = mSceneMgr->createEntity("bonus2","cube.mesh");
	bonus->setCastShadows(false);
	bonusNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("bonus2");
	bonusNode->setScale(0.7,0.7,0.7);
	vectorPosition=Ogre::Vector2(22,3);
	tempVector=getWorldCoord(vectorPosition);
	tempVector.y+=50;
	bonusNode->setPosition(tempVector);
	bonusNode->attachObject(bonus);

	bonus = mSceneMgr->createEntity("bonus3","cube.mesh");
	bonus->setCastShadows(false);
	bonusNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("bonus3");
	bonusNode->setScale(0.7,0.7,0.7);
	vectorPosition=Ogre::Vector2(20,10);
	tempVector=getWorldCoord(vectorPosition);
	tempVector.y+=50;
	bonusNode->setPosition(tempVector);
	bonusNode->attachObject(bonus);*/

	//ÉèÖÃbonus
	gameMap->setMapTypeAtGridPos(3,15,GRID_ADD_SPEED);
	gameMap->setMapTypeAtGridPos(22,3,GRID_ADD_SPEED);
	gameMap->setMapTypeAtGridPos(20,10,GRID_ADD_SPEED);

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("pointlight");
	l->setType(Ogre::Light::LT_POINT);
	l->setPosition(Ogre::Vector3(0, 700, 0));
	l->setDiffuseColour(1.0, 1.0, 1.0);
	l->setSpecularColour(0.5, 0.5, 0.5);

	//terrain
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 3000, 3000, 20, 20, true, 1, 25, 25, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	Ogre::SceneNode* planeNode= mSceneMgr->getRootSceneNode()->createChildSceneNode();
	planeNode->attachObject(entGround);
	planeNode->setPosition(60,0,60);
	entGround->setMaterialName("Examples/GrassFloor");
	entGround->setCastShadows(false);

	//sky
	mSceneMgr->setSkyBox(true, "Examples/SceneSkyBox2", 5000, false);

	//UI Elements
	
}
void GameScene::Update(const Ogre::FrameEvent& evt)
{
	player->Update(evt,gameMap);
	
	updateMapGridType();

	updateEnemyList(evt,gameMap);

	gameMap->Update();

	updatePlayerLifecycle();

	updateBonus();
}

Ogre::Vector3 GameScene::getWorldCoord(Ogre::Vector2 pos)
{
	Ogre::Vector3 rtn;
	rtn.y = 0;
	rtn.x = 120*(pos.x - 12)+60;
	rtn.z = 120*(pos.y - 10)+60;
	return rtn;
}

void GameScene::addBonus(Ogre::Vector2 pos)
{
	int x = pos.x;
	int y = pos.y;
	int randResult = rand()%11;
	if (randResult <= 1){
		map.setMapAtPos(x, y, MAP_ADD_BOMB);
		//..
	} else if (randResult <= 3){
		map.setMapAtPos(x, y, MAP_ADD_POWER);
		//..
	} else if (randResult <= 5){
		map.setMapAtPos(x, y, MAP_ADD_SPEED);
		//..
	} else if (randResult <= 6){
		map.setMapAtPos(x, y, MAP_ADD_HEALTH);
		//..
	}
}

void GameScene::updateEnemyList(const Ogre::FrameEvent& evt,GameMap* gameMap)
{
	std::vector<EnemyAI*>::iterator iter=enemyList.begin();

	for(;iter!=enemyList.end();iter++)
	{
		//*iter->Update(evt,gameMap);
		EnemyAI* temp=*iter;
		temp->Update(evt,gameMap);
		//*iter->Update(evt,gameMap);
	}
}
void GameScene::updateMapGridType()
{
	//directionType playerDirection = player->getPlayerDirection();
	//if (player->_currentGridType == GRID_NORMAL)
	//{
	//	if (playerDirection == LEFT_DIRECTION && player->_rightGridType != GRID_BOMB)
	//	{
	//		//gameMap->setMapTypeAtGridPos(player->getPlayerXPos()+1,player->getPlayerYPos(),GRID_NORMAL);
	//		gameMap->updateGridWithPlayerPosition(player->getPlayerXPos()+1,player->getPlayerYPos(),false);
	//	}
	//	else if (playerDirection == RIGHT_DIRECTION && player->_leftGridType != GRID_BOMB)
	//	{
	//		//gameMap->setMapTypeAtGridPos(player->getPlayerXPos()-1,player->getPlayerYPos(),GRID_NORMAL);
	//		gameMap->updateGridWithPlayerPosition(player->getPlayerXPos()-1,player->getPlayerYPos(),false);
	//	}
	//	else if (playerDirection == UP_DIRECTION && player->_downGridType != GRID_BOMB)
	//	{
	//		//gameMap->setMapTypeAtGridPos(player->getPlayerXPos(),player->getPlayerYPos()+1,GRID_NORMAL);
	//		gameMap->updateGridWithPlayerPosition(player->getPlayerXPos(),player->getPlayerYPos()+1,false);
	//	}
	//	else if (playerDirection == DOWN_DIRECTION && player->_upGridType != GRID_BOMB)
	//	{
	//		//gameMap->setMapTypeAtGridPos(player->getPlayerXPos(),player->getPlayerYPos()-1,GRID_NORMAL);
	//		gameMap->updateGridWithPlayerPosition(player->getPlayerXPos(),player->getPlayerYPos()-1,false);
	//	}
	//}
	//gameMap->setMapTypeAtGridPos(player->getPlayerXPos(),player->getPlayerYPos(),GRID_PLAYER);

	for (int i=0; i < MAP_WIDTH; ++i)
	{
		for (int j=0; j < MAP_HEIGHT; ++j)
		{
			gameMap->updateGridWithPlayerPosition(i,j,false);
		}
	}

	gameMap->updateGridWithPlayerPosition(player->getPlayerXPos(),player->getPlayerYPos(),true);
	/*gameMap->updateGridWithPlayerPosition(player->getPlayerXPos()+1,player->getPlayerYPos(),false);
	gameMap->updateGridWithPlayerPosition(player->getPlayerXPos()-1,player->getPlayerYPos(),false);
	gameMap->updateGridWithPlayerPosition(player->getPlayerXPos(),player->getPlayerYPos()+1,false);
	gameMap->updateGridWithPlayerPosition(player->getPlayerXPos(),player->getPlayerYPos()-1,false);*/
}


void GameScene::updatePlayerLifecycle()
{
	if (player->getPlayerHP() <= 0)
	{
		gameOver = true;
	}
}

void GameScene::updateBonus()
{
	for (int i = 0; i<MAP_WIDTH;i++)
	{
		for (int j=0;j<MAP_HEIGHT;j++)
		{
			int tempRandom = rand()%120 + 1;
			gridType tempType = gameMap->getMapTypeAtGridPos(i,j);
			if (tempType == GRID_NORMAL)
			{
				if (tempRandom == 1)
				{
					gameMap->setMapTypeAtGridPos(i,j,GRID_ADD_HEALTH);
				}
				else if (tempRandom == 2)
				{
					gameMap->setMapTypeAtGridPos(i,j,GRID_ADD_BOMB);
				}
				else if (tempRandom == 3)
				{
					gameMap->setMapTypeAtGridPos(i,j,GRID_ADD_POWER);
				}
				else if (tempRandom == 4)
				{
					gameMap->setMapTypeAtGridPos(i,j,GRID_ADD_SPEED);
				}
			}
		}
	}


	//gridType tempType = gameMap->getMapTypeAtGridPos(tempXPos,tempYPos);
	//if (tempType != GRID_WALL || tempType != GRID_DISTORYABLE_WALL)
	//{
	//	if (tempRandom == 1)
	//	{
	//		gameMap->setMapTypeAtGridPos(tempXPos,tempYPos,GRID_ADD_SPEED);
	//	}
	//	else if (tempRandom == 2)
	//	{
	//		gameMap->setMapTypeAtGridPos(tempXPos,tempYPos,GRID_ADD_HEALTH);
	//	}
	//	else if (tempRandom == 3)
	//	{
	//		gameMap->setMapTypeAtGridPos(tempXPos,tempYPos,GRID_ADD_BOMB);
	//	}
	//	else if (tempRandom == 4)
	//	{
	//		gameMap->setMapTypeAtGridPos(tempXPos,tempYPos,GRID_ADD_POWER);
	//	}
	//}
}