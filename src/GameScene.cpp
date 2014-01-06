#include "GameScene.h"

float winTime = 0;
audiere::AudioDevicePtr device3(audiere::OpenDevice());
audiere::OutputStreamPtr gameOverStream(audiere::OpenSound(device3,"Resources/Sound/gameover.wav",false));
audiere::OutputStreamPtr gameWinStream(audiere::OpenSound(device3,"Resources/Sound/win.wav",false));

GameScene::GameScene(void)
{
	nonNPCAnimState = NULL;
	NPCAnimState = NULL;
	isSpaceKeyDown = false;
	bombIndex = 0;
	gameOver = false;
	gameWin = false;
	_addBonusDuration = 20.0;
	_canAddBonus = true;
	_gameWinDuration = 9.0;
	_gameOverDuration = 3.0;
	showWinState = false;
	showLoseState = false;
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
	mSceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "GameSceneMgr");

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
//#pragma region tempboard
//	Ogre::Entity *tempBoard = mSceneMgr->createEntity("bomb_tempboard","cube.mesh");
//	tempBoard->setMaterialName("Examples/Rockwall");
//	Ogre::SceneNode *tempBoardNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("boomb_tempboard");
//	//tempBoardNode->setScale(7.0,7.0,7.0);
//	tempBoardNode->setPosition(0,0,0);
//	Ogre::Quaternion q1 = tempBoardNode->getOrientation();
//	Ogre::Quaternion q2(Ogre::Degree(90),Ogre::Vector3::UNIT_Y);
//	tempBoardNode->setOrientation(q1*q2);
//	tempBoardNode->attachObject(tempBoard);
//#pragma endregion

	//for(int x=0;x<15;++x)
	//{
	//	for(int y=0;y<13;++y)
	//	{
	//		if(map.getMapAtPos(x,y)==MAP_DESTROYABLE)
	//		{
	//			Ogre::Entity *destroyableBlock = mSceneMgr->createEntity("cube.mesh");
	//			//destroyableBlock->setMaterialName("Examples/Rockwall");
	//			destroyableBlock->setCastShadows(true);
	//			mapNode[x][y] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//			mapNode[x][y]->setPosition(0,0,0);
	//			mapNode[x][y]->setPosition(getWorldCoord(Ogre::Vector2(x,y)));
	//			mapNode[x][y]->attachObject(destroyableBlock);
	//		}
	//		else if(map.getMapAtPos(x,y) == MAP_INDESTROYABLE)
	//		{
	//			Ogre::Entity *indestroyableBlock = mSceneMgr->createEntity("cube.mesh");
	//			//indestroyableBlock->setMaterialName("boomb_testInDestroyable");
	//			indestroyableBlock->setCastShadows(true);
	//			mapNode[x][y] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//			mapNode[x][y]->setPosition(0,0,0);
	//			mapNode[x][y]->setPosition(getWorldCoord(Ogre::Vector2(x,y)));
	//			mapNode[x][y]->attachObject(indestroyableBlock);
	//		}
	//	}
	//}

	gameMap =  new GameMap(mSceneMgr);

	//test player
	Ogre::Entity *playerEntity = mSceneMgr->createEntity("player","robot.mesh");
	playerEntity->setCastShadows(false);
	Ogre::SceneNode* PlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("playerNode");
	//Ogre::Vector2 testvector=Ogre::Vector2(20,5);
	PlayerNode->setPosition(getWorldCoord( gameMap->getPlayerInitPos(),0));
	PlayerNode->setScale(4.0,2.0,4.0);
	PlayerNode->attachObject(playerEntity);


	player=new Player(playerEntity,PlayerNode,mSceneMgr,mCamera,mKeyboard);

	//add 3 enemy
	//1
	EnemyAI* tempEnemy;
	Ogre::Vector2 vectorPosition;
	bool isVisible;
	Ogre::Entity *npc;

	npc = mSceneMgr->createEntity("Bomberman1Sphere12.mesh");
	npc->setCastShadows(false);
	NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	NPCPlayerNode->setScale(120,120,120);
	NPCPlayerNode->yaw(Ogre::Degree(180));
	isVisible=gameMap->getEnemyInitPos(vectorPosition);
	if(isVisible)
	{
		NPCPlayerNode->setPosition(getWorldCoord(vectorPosition,80));
		NPCPlayerNode->attachObject(npc);
		tempEnemy=new EnemyAI(npc,NPCPlayerNode,mSceneMgr,mCamera,mKeyboard);
	}
	else
	{
		vectorPosition.x=1;
		vectorPosition.y=1;
		NPCPlayerNode->setPosition(getWorldCoord(vectorPosition,80));
		NPCPlayerNode->attachObject(npc);
		tempEnemy=new EnemyAI(npc,NPCPlayerNode,mSceneMgr,mCamera,mKeyboard);
		tempEnemy->setDead();
	}
	//push
	enemyList.push_back(tempEnemy);

	//2

	npc = mSceneMgr->createEntity("bomberman2Sphere09.mesh");
	npc->setCastShadows(false);
	NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	NPCPlayerNode->setScale(120,120,120);
	NPCPlayerNode->yaw(Ogre::Degree(180));
	isVisible=gameMap->getEnemyInitPos(vectorPosition);
	if(isVisible)
	{
		NPCPlayerNode->setPosition(getWorldCoord(vectorPosition,80));
		NPCPlayerNode->attachObject(npc);
		tempEnemy=new EnemyAI(npc,NPCPlayerNode,mSceneMgr,mCamera,mKeyboard);
	}
	else
	{
		vectorPosition.x=1;
		vectorPosition.y=1;
		NPCPlayerNode->setPosition(getWorldCoord(vectorPosition,80));
		NPCPlayerNode->attachObject(npc);
		tempEnemy=new EnemyAI(npc,NPCPlayerNode,mSceneMgr,mCamera,mKeyboard);
		tempEnemy->setDead();
	}
	//push
	enemyList.push_back(tempEnemy);


	//npc = mSceneMgr->createEntity("ninja.mesh");
	//npc->setCastShadows(false);
	//NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//NPCPlayerNode->setScale(1.5,1.0,1.5);
	//vectorPosition=Ogre::Vector2(15,15);
	//NPCPlayerNode->setPosition(getWorldCoord(vectorPosition));
	////NPCPlayerNode->yaw( Ogre::Degree( -90 ) );
	//NPCPlayerNode->attachObject(npc);

	//tempEnemy=new EnemyAI(npc,NPCPlayerNode,mSceneMgr,mCamera,mKeyboard);
	//enemyList.push_back(tempEnemy);
	//3

	npc = mSceneMgr->createEntity("Bomberman1Sphere12.mesh");
	npc->setCastShadows(false);
	NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	NPCPlayerNode->setScale(120,120,120);
	NPCPlayerNode->yaw(Ogre::Degree(180));
	isVisible=gameMap->getEnemyInitPos(vectorPosition);
	if(isVisible)
	{
		NPCPlayerNode->setPosition(getWorldCoord(vectorPosition,80));
		NPCPlayerNode->attachObject(npc);
		tempEnemy=new EnemyAI(npc,NPCPlayerNode,mSceneMgr,mCamera,mKeyboard);
	}
	else
	{
		vectorPosition.x=1;
		vectorPosition.y=1;
		NPCPlayerNode->setPosition(getWorldCoord(vectorPosition,80));
		NPCPlayerNode->attachObject(npc);
		tempEnemy=new EnemyAI(npc,NPCPlayerNode,mSceneMgr,mCamera,mKeyboard);
		tempEnemy->setDead();
	}
	//push
	enemyList.push_back(tempEnemy);
	/*
	npc = mSceneMgr->createEntity("ninja.mesh");
	npc->setCastShadows(false);
	NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	NPCPlayerNode->setScale(1.5,1.0,1.5);
	vectorPosition=Ogre::Vector2(5,13);
	NPCPlayerNode->setPosition(getWorldCoord(vectorPosition));
	NPCPlayerNode->attachObject(npc);
	tempEnemy=new EnemyAI(npc,NPCPlayerNode,mSceneMgr,mCamera,mKeyboard);
	enemyList.push_back(tempEnemy);*/

	

	//ÉèÖÃbonus
	//gameMap->setMapTypeAtGridPos(3,15,GRID_ADD_SPEED);
	//gameMap->setMapTypeAtGridPos(12,3,GRID_ADD_SPEED);
	//gameMap->setMapTypeAtGridPos(20,10,GRID_ADD_HEALTH);


	//gameMap->setMapTypeAtGridPos(7,15,GRID_ADD_HEALTH);
	//gameMap->setMapTypeAtGridPos(22,3,GRID_ADD_SPEED);
	//gameMap->setMapTypeAtGridPos(20,10,GRID_ADD_BOMB);

	//gameMap->setMapTypeAtGridPos(3,7,GRID_ADD_HEALTH);
	//gameMap->setMapTypeAtGridPos(4,3,GRID_ADD_BOMB_POWER);
	//gameMap->setMapTypeAtGridPos(1,2,GRID_ADD_BOMB);
	//set life display
	npc = mSceneMgr->createEntity("PlayerHeadRobot2.mesh");
	npc->setCastShadows(false);
	NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	NPCPlayerNode->setScale(120,120,120);
	NPCPlayerNode->pitch(Ogre::Degree(60));
	vectorPosition.x=13;
	vectorPosition.y=19;
	NPCPlayerNode->setPosition(getWorldCoord(vectorPosition,100));
	NPCPlayerNode->attachObject(npc);
	playerLifeList.push_back(NPCPlayerNode);

	npc = mSceneMgr->createEntity("PlayerHeadRobot2.mesh");
	npc->setCastShadows(false);
	NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	NPCPlayerNode->setScale(120,120,120);
	NPCPlayerNode->pitch(Ogre::Degree(60));
	vectorPosition.x=12;
	vectorPosition.y=19;
	NPCPlayerNode->setPosition(getWorldCoord(vectorPosition,100));
	NPCPlayerNode->attachObject(npc);
	playerLifeList.push_back(NPCPlayerNode);

	npc = mSceneMgr->createEntity("PlayerHeadRobot2.mesh");
	npc->setCastShadows(false);
	NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	NPCPlayerNode->setScale(120,120,120);
	NPCPlayerNode->pitch(Ogre::Degree(60));
	vectorPosition.x=11;
	vectorPosition.y=19;
	NPCPlayerNode->setPosition(getWorldCoord(vectorPosition,100));
	NPCPlayerNode->attachObject(npc);
	playerLifeList.push_back(NPCPlayerNode);


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
		plane, 3000, 3000, 20, 20, true, 1,12.5, 12.5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	Ogre::SceneNode* planeNode= mSceneMgr->getRootSceneNode()->createChildSceneNode();
	planeNode->attachObject(entGround);
	planeNode->setPosition(60,0,60);
	entGround->setMaterialName("Examples/GrassFloor");
	entGround->setCastShadows(false);

	//sky
	mSceneMgr->setSkyBox(true, "Examples/SceneSkyBox1", 5000, false);
}
void GameScene::Update(const Ogre::FrameEvent& evt)
{
	player->Update(evt,gameMap);
	
	updateMapGridType();

	updateEnemyList(evt,gameMap);

	gameMap->Update(evt);

	updatePlayerLifecycle();

	updatePlayerLifeDisplay();

	updateBonus(evt);

	updateGameState(evt);
}

Ogre::Vector3 GameScene::getWorldCoord(Ogre::Vector2 pos, float yPos)
{
	Ogre::Vector3 rtn;
	rtn.y = yPos;
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
	if (!showLoseState)
	{
		std::vector<EnemyAI*>::iterator iter=enemyList.begin();
		int enemyNum = 0;
	
		for(;iter!=enemyList.end();iter++)
		{
			//*iter->Update(evt,gameMap);
			EnemyAI* temp=*iter;
			if(temp->Update(evt,gameMap))
			{
				//keep going
			}
			else
			{
				temp->setVisible(false);
				enemyNum++;
			}
			if (enemyNum == 3)
			{
				gameWinStream->setRepeat(false);
				gameWinStream->setVolume(1.0);
				gameWinStream->play();
				showWinState = true;
			}
			
			//temp->Update(evt,gameMap);
			//*iter->Update(evt,gameMap);
		}
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
		gameOverStream->setRepeat(false);
		gameOverStream->setVolume(1.0);
		gameOverStream->play();
		showLoseState = true;
		player->setDead();
		mSceneMgr->getSceneNode("playerNode")->setVisible(false);
	}
}

void GameScene::updateBonus(const Ogre::FrameEvent& evt)
{
	if (_canAddBonus && (!showLoseState))
	{
		for (int i = 0; i<MAP_WIDTH;i++)
		{
			for (int j=0;j<MAP_HEIGHT;j++)
			{
				int tempRandom = rand()%180 + 1;
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
						gameMap->setMapTypeAtGridPos(i,j,GRID_ADD_BOMB_POWER);
					}
					else if (tempRandom == 4)
					{
						gameMap->setMapTypeAtGridPos(i,j,GRID_ADD_SPEED);
					}
					_canAddBonus = false;
				}
			}
		}
	}
	updateBonusDuration(evt);
}

void GameScene::updateBonusDuration(const Ogre::FrameEvent& evt)
{
	if (!_canAddBonus)
	{
		_addBonusDuration -= evt.timeSinceLastFrame;
		if (_addBonusDuration <= 0)
		{
			_canAddBonus = true;
			_addBonusDuration = 20.0;
		}
	}
}

void GameScene::updateGameState(const Ogre::FrameEvent& evt)
{
	if (showWinState)
	{
		gameMap->setParticleEffectAtGrid(player->getPlayerXPos(),player->getPlayerYPos(),evt,PARTICLE_WIN);
		_gameWinDuration -= evt.timeSinceLastFrame;
		if (_gameWinDuration <= 0)
		{
			gameWin = true;
			showWinState = false;
			_gameWinDuration = 5.0;
		}
	}
	else if (showLoseState)
	{
		gameMap->setParticleEffectAtGrid(player->getPlayerXPos(),player->getPlayerYPos(),evt,PARTICLE_LOSE);
		_gameOverDuration -= evt.timeSinceLastFrame;
		if (_gameOverDuration <= 0)
		{
			gameOver = true;
			showLoseState = false;
			_gameOverDuration = 5.0;
		}
	}
}

void GameScene::updatePlayerLifeDisplay()
{
	int num=player->getPlayerHP();
	num=3-num;

	//all true
	std::vector<Ogre::SceneNode*>::iterator iter=playerLifeList.begin();

	for(;iter!=playerLifeList.end();iter++)
	{
		Ogre::SceneNode* temp=*iter;
		temp->setVisible(true);
	}

	//
	iter=playerLifeList.begin();

	for(;iter!=playerLifeList.end();iter++)
	{
		if(num<=0)
		{
			break;
		}
		Ogre::SceneNode* temp=*iter;
		temp->setVisible(false);
		num--;
	}
}
