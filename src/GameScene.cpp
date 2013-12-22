#include "GameScene.h"

GameScene::GameScene(void)
{
	nonNPCAnimState = NULL;
	NPCAnimState = NULL;
	isSpaceKeyDown = false;
	bombIndex = 0;
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

	//Ogre::Entity *nonNPC = mSceneMgr->createEntity("nonnpcplayer","testSphere001.mesh");
	//nonNPC->setCastShadows(true);
	//nonNPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("nonnpcplayerNode");
	//nonNPCPlayerNode->setPosition(getWorldCoord(nonNPCPlayer.pos));
	//nonNPCPlayerNode->setScale(2.0,2.0,2.0);
	//nonNPCPlayerNode->attachObject(nonNPC);


	//test player
	Ogre::Entity *playerEntity = mSceneMgr->createEntity("player","ninja.mesh");
	playerEntity->setCastShadows(true);
	Ogre::SceneNode* PlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("playerNode");
	Ogre::Vector2 testvector=Ogre::Vector2(20,5);
	PlayerNode->setPosition(getWorldCoord( testvector));
	PlayerNode->setScale(2.0,2.0,2.0);
	PlayerNode->attachObject(playerEntity);


	player=new Player(playerEntity,PlayerNode,mSceneMgr,mCamera,mKeyboard);
	//tempplayer.Start(playerEntity,PlayerNode);
	//player=&tempplayer;
	//player.Start(playerEntity,PlayerNode);


	Ogre::Entity *npc = mSceneMgr->createEntity("npcplayer","ninja.mesh");
	npc->setCastShadows(true);
	NPCPlayerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("npcplayerNode");
	NPCPlayerNode->setScale(2.0,2.0,2.0);
	NPCPlayerNode->setPosition(getWorldCoord(NPCPlayer.pos));
	NPCPlayerNode->attachObject(npc);

	enemy=new EnemyAI(npc,NPCPlayerNode,mSceneMgr,mCamera,mKeyboard);

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("pointlight");
	l->setType(Ogre::Light::LT_POINT);
	l->setPosition(Ogre::Vector3(0, 700, 0));
	l->setDiffuseColour(1.0, 1.0, 1.0);
	l->setSpecularColour(0.5, 0.5, 0.5);


	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 3000, 3000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);

	entGround->setMaterialName("Examples/Rockwall");
	entGround->setCastShadows(false);
}
void GameScene::Update(const Ogre::FrameEvent& evt)
{
	player->Update(evt,gameMap);
	
	updateMapGridType();

	gameMap->Update();

	enemy->Update(evt,gameMap);

	//if (mKeyboard->isKeyDown(OIS::KC_SPACE))
	//{
	//	if (!isSpaceKeyDown)
	//	{
	//		if (nonNPCPlayer.bombAvailable && 
	//			map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) <= 2)
	//		{

	//			thromBomb(nonNPCPlayer);
	//		}
	//		isSpaceKeyDown = true;
	//	}
	//}
	//else 
	//{
	//	isSpaceKeyDown = false;
	//}

	//updateBombInfo(evt);

	//if (mKeyboard->isKeyDown(OIS::KC_DOWN)){
	//	if (!nonNPCAnimState){
	//		askingPlayer(NON_NPC, EVENT_DOWN);
	//	} else if (nonNPCAnimState->hasEnded()){
	//		askingPlayer(NON_NPC, EVENT_DOWN);
	//	}
	//} else if (mKeyboard->isKeyDown(OIS::KC_UP)){
	//	if (!nonNPCAnimState){
	//		askingPlayer(NON_NPC, EVENT_UP);
	//	} else if (nonNPCAnimState->hasEnded()){
	//		askingPlayer(NON_NPC, EVENT_UP);
	//	}
	//} else if (mKeyboard->isKeyDown(OIS::KC_LEFT)){
	//	if (!nonNPCAnimState){
	//		askingPlayer(NON_NPC, EVENT_LEFT);
	//	} else if (nonNPCAnimState->hasEnded()){
	//		askingPlayer(NON_NPC, EVENT_LEFT);
	//	}
	//} else if (mKeyboard->isKeyDown(OIS::KC_RIGHT)){
	//	if (!nonNPCAnimState){
	//		askingPlayer(NON_NPC, EVENT_RIGHT);
	//	} else if (nonNPCAnimState->hasEnded()){
	//		askingPlayer(NON_NPC, EVENT_RIGHT);
	//	}
	//}

	//if (!NPCAnimState){
	//	int npcNextEvent = aiBrain.calculateNPCNextEvent(nonNPCPlayer, NPCPlayer, map);
	//	if (npcNextEvent == AI_BOMB){
	//		if (NPCPlayer.bombAvailable && 
	//			map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) <= 2){

	//				thromBomb(NPCPlayer);
	//		}
	//	} else if (npcNextEvent == AI_DOWN){
	//		askingPlayer(NPC, EVENT_DOWN);
	//	} else if (npcNextEvent == AI_UP){
	//		askingPlayer(NPC, EVENT_UP);
	//	} else if (npcNextEvent == AI_LEFT){
	//		askingPlayer(NPC, EVENT_LEFT);
	//	} else if (npcNextEvent == AI_RIGHT){
	//		askingPlayer(NPC, EVENT_RIGHT);
	//	}
	//} else if (NPCAnimState->hasEnded()){
	//	int npcNextEvent = aiBrain.calculateNPCNextEvent(nonNPCPlayer, NPCPlayer, map);
	//	if (npcNextEvent == AI_BOMB){
	//		if (NPCPlayer.bombAvailable && 
	//			map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) <= 2){

	//				thromBomb(NPCPlayer);
	//		}
	//	} else if (npcNextEvent == AI_DOWN){
	//		askingPlayer(NPC, EVENT_DOWN);
	//	} else if (npcNextEvent == AI_UP){
	//		askingPlayer(NPC, EVENT_UP);
	//	} else if (npcNextEvent == AI_LEFT){
	//		askingPlayer(NPC, EVENT_LEFT);
	//	} else if (npcNextEvent == AI_RIGHT){
	//		askingPlayer(NPC, EVENT_RIGHT);
	//	}
	//}

	//if(updatePlayerInfo(evt)){
	//	//gameEnd();
	//	return;
	//}
	//if (nonNPCAnimState){
	//	if (!nonNPCAnimState->hasEnded()){
	//		nonNPCAnimState->addTime(evt.timeSinceLastFrame);
	//		updatePlayerPos();
	//	}
	//} 
	//if (NPCAnimState){
	//	if (!NPCAnimState->hasEnded()){
	//		NPCAnimState->addTime(evt.timeSinceLastFrame);
	//		updateNPCPlayerPos();
	//	}
	//} 
}

Ogre::Vector3 GameScene::getWorldCoord(Ogre::Vector2 pos)
{
	Ogre::Vector3 rtn;
	rtn.y = 0;
	rtn.x = 120*(pos.x - 12);
	rtn.z = 120*(pos.y - 10);
	return rtn;
}

//void GameScene::askingPlayer(int playerType, int eventType)
//{
//	switch(eventType)
//	{
//	case EVENT_UP:
//		{
//			if (playerType == NON_NPC){
//				int mapType = map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y - 1);
//				if (mapType  <= 2){
//					if (nonNPCAnimState){
//						if (nonNPCAnimState->hasEnded()){
//							nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
//							nonNPCPlayer.destinationPos.y--;
//							movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), nonNPCAnimState);
//						}
//					} else {
//						nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
//						nonNPCPlayer.destinationPos.y--;
//						movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), nonNPCAnimState);
//					}
//				} // else if
//			} else {
//				int mapType = map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y - 1);
//				if (mapType  <= 2){
//					if (NPCAnimState){
//						if (NPCAnimState->hasEnded()){
//							NPCPlayer.destinationPos = NPCPlayer.pos;
//							NPCPlayer.destinationPos.y--;
//							movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), NPCAnimState);
//						}
//					} else {
//						NPCPlayer.destinationPos = NPCPlayer.pos;
//						NPCPlayer.destinationPos.y--;
//						movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(0, 0, -GRID_SIZE), NPCAnimState);
//					}
//				} // else if
//			}
//			break;
//		}
//	case EVENT_DOWN:
//		{
//			if (playerType == NON_NPC){
//				int mapType = map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y + 1);
//				if (mapType  <= 2){
//					if (nonNPCAnimState){
//						if (nonNPCAnimState->hasEnded()){
//							nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
//							nonNPCPlayer.destinationPos.y++;
//							movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), nonNPCAnimState);
//						}
//					} else {
//						nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
//						nonNPCPlayer.destinationPos.y++;
//						movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), nonNPCAnimState);
//					}
//				} // else if
//			} else {
//				int mapType = map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y + 1);
//				if (mapType  <= 2){
//					if (NPCAnimState){
//						if (NPCAnimState->hasEnded()){
//							NPCPlayer.destinationPos = NPCPlayer.pos;
//							NPCPlayer.destinationPos.y++;
//							movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), NPCAnimState);
//						}
//					} else {
//						NPCPlayer.destinationPos = NPCPlayer.pos;
//						NPCPlayer.destinationPos.y++;
//						movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(0, 0, GRID_SIZE), NPCAnimState);
//					}
//				} // else if
//			}
//			break;
//		}
//	case EVENT_LEFT:
//		{
//			if (playerType == NON_NPC){
//				int mapType = map.getMapAtPos(nonNPCPlayer.pos.x - 1, nonNPCPlayer.pos.y);
//				if (mapType  <= 2){
//					if (nonNPCAnimState){
//						if (nonNPCAnimState->hasEnded()){
//							nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
//							nonNPCPlayer.destinationPos.x--;
//							movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), nonNPCAnimState);
//						}
//					} else {
//						nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
//						nonNPCPlayer.destinationPos.x--;
//						movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), nonNPCAnimState);
//					}
//				} // else if
//			} else {
//				int mapType = map.getMapAtPos(NPCPlayer.pos.x - 1, NPCPlayer.pos.y);
//				if (mapType  <= 2){
//					if (NPCAnimState){
//						if (NPCAnimState->hasEnded()){
//							NPCPlayer.destinationPos = NPCPlayer.pos;
//							NPCPlayer.destinationPos.x--;
//							movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), NPCAnimState);
//						}
//					} else {
//						NPCPlayer.destinationPos = NPCPlayer.pos;
//						NPCPlayer.destinationPos.x--;
//						movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(-GRID_SIZE, 0, 0), NPCAnimState);
//					}
//				} // else if
//			}
//			break;
//		}
//	case EVENT_RIGHT:
//		{
//			if (playerType == NON_NPC){
//				int mapType = map.getMapAtPos(nonNPCPlayer.pos.x + 1, nonNPCPlayer.pos.y);
//				if (mapType  <= 2){
//					if (nonNPCAnimState){
//						if (nonNPCAnimState->hasEnded()){
//							nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
//							nonNPCPlayer.destinationPos.x++;
//							movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), nonNPCAnimState);
//						}
//					} else {
//						nonNPCPlayer.destinationPos = nonNPCPlayer.pos;
//						nonNPCPlayer.destinationPos.x++;
//						movingPlayer(NON_NPC, nonNPCPlayer.speed, nonNPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), nonNPCAnimState);
//					}
//				} // else if
//			} else {
//				int mapType = map.getMapAtPos(NPCPlayer.pos.x + 1, NPCPlayer.pos.y);
//				if (mapType  <= 2){
//					if (NPCAnimState){
//						if (NPCAnimState->hasEnded()){
//							NPCPlayer.destinationPos = NPCPlayer.pos;
//							NPCPlayer.destinationPos.x++;
//							movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), NPCAnimState);
//						}
//					} else {
//						NPCPlayer.destinationPos = NPCPlayer.pos;
//						NPCPlayer.destinationPos.x++;
//						movingPlayer(NPC, NPCPlayer.speed, NPCPlayerNode, Ogre::Vector3(GRID_SIZE, 0, 0), NPCAnimState);
//					}
//				} // else if
//			}
//			break;
//		}
//	default:
//		break;
//	}
//}

//void GameScene::movingPlayer(int playerType, float speed, Ogre::SceneNode *playerNode, Ogre::Vector3 direction, Ogre::AnimationState *animState)
//{
//	Ogre::Animation *anim;
//	if (playerType == NON_NPC){
//		if (animState){
//			mSceneMgr->destroyAnimation("nonnpcmoving");
//		}
//		anim = mSceneMgr->createAnimation("nonnpcmoving", speed);
//		nonNPCAnimState = mSceneMgr->createAnimationState("nonnpcmoving");
//		animState = nonNPCAnimState;
//	} else {
//		if (animState){
//			mSceneMgr->destroyAnimation("npcmoving");
//		}
//		anim = mSceneMgr->createAnimation("npcmoving", speed);
//		NPCAnimState = mSceneMgr->createAnimationState("npcmoving");
//		animState = NPCAnimState;
//	}
//	anim->setInterpolationMode(Ogre::Animation::IM_LINEAR);
//	Ogre::NodeAnimationTrack *track = anim->createNodeTrack(0, playerNode);
//	Ogre::TransformKeyFrame *key = track->createNodeKeyFrame(0);
//	Ogre::Vector3 currentPos = playerNode->getPosition();
//	key->setTranslate(currentPos);
//	key->setScale(Ogre::Vector3(2.0, 2.0, 2.0));
//	key = track->createNodeKeyFrame(speed);
//	currentPos += direction;
//	key->setTranslate(currentPos);
//	key->setScale(Ogre::Vector3(2.0, 2.0, 2.0));
//
//	animState->setEnabled(true);
//	animState->setLoop(false);
//}

//void GameScene::updatePlayerPos(void)
//{
//	if (nonNPCAnimState->getTimePosition() >= nonNPCPlayer.speed/2){
//		if (map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) != MAP_BOMB)
//			map.setMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y, MAP_NONE);
//		nonNPCPlayer.pos = nonNPCPlayer.destinationPos;
//		if (map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) < 0){
//			if (map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) == MAP_ADD_BOMB){
//				nonNPCPlayer.bombAvailable++;
//			} else if (map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) == MAP_ADD_POWER){
//				nonNPCPlayer.power++;
//			} else if (map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) == MAP_ADD_SPEED){
//				nonNPCPlayer.speed *= 0.6;
//			} else if (map.getMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y) == MAP_ADD_HEALTH){
//				if (nonNPCPlayer.health < 3)
//					nonNPCPlayer.health++;
//			}
//			int x = nonNPCPlayer.pos.x;
//			int y = nonNPCPlayer.pos.y;
//			//mSceneMgr->destroySceneNode(mapNode[x][y]);
//		}
//		map.setMapAtPos(nonNPCPlayer.pos.x, nonNPCPlayer.pos.y, MAP_NON_NPC);
//	}
//}
//
//void GameScene::updateNPCPlayerPos(void)
//{
//	if (NPCAnimState->getTimePosition() >= NPCPlayer.speed/2){
//		if (map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) != MAP_BOMB)
//			map.setMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y, MAP_NONE);
//		NPCPlayer.pos = NPCPlayer.destinationPos;
//		if (map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) < 0){
//			if (map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) == MAP_ADD_BOMB){
//				NPCPlayer.bombAvailable++;
//			} else if (map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) == MAP_ADD_POWER){
//				NPCPlayer.power++;
//			} else if (map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) == MAP_ADD_SPEED){
//				NPCPlayer.speed *= 0.6;
//			} else if (map.getMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y) == MAP_ADD_HEALTH){
//				if (NPCPlayer.health < 3)
//					NPCPlayer.health++;
//			}
//			int x = NPCPlayer.pos.x;
//			int y = NPCPlayer.pos.y;
//			//mSceneMgr->destroySceneNode(mapNode[x][y]);
//		}
//		map.setMapAtPos(NPCPlayer.pos.x, NPCPlayer.pos.y, MAP_NPC);
//	}
//}
//
//int GameScene::thromBomb(BMPlayer &player)
//{
//	Bomb tempBomb;
//	if (player.playerType == NON_NPC)
//		tempBomb.bombType = NON_NPC_BOMB;
//	else
//		tempBomb.bombType = NPC_BOME;
//	tempBomb.countDown = 3.0;
//	tempBomb.pos = player.pos;
//	tempBomb.power = player.power;
//	tempBomb.node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
//	tempBomb.node->setPosition(getWorldCoord(tempBomb.pos));
//	Ogre::Entity *bombEntity = mSceneMgr->createEntity("cube.mesh");
//	bombEntity->setCastShadows(true);
//	tempBomb.node->attachObject(bombEntity);
//	int x = tempBomb.pos.x;
//	int y = tempBomb.pos.y;
//	tempBomb.bombIdx = bombIndex;
//	mapNode[x][y] = tempBomb.node;
//	bombPool.insert(std::pair<int, Bomb>(bombIndex++, tempBomb));
//	map.setMapAtPos(tempBomb.pos.x, tempBomb.pos.y, MAP_BOMB);
//	player.bombAvailable--;
//	return (bombIndex-1);
//}
//
//void GameScene::explodeBomb(int bombIdx)
//{
//	std::map<int, Bomb>::iterator iter = bombPool.find(bombIdx);
//
//	mSceneMgr->destroySceneNode(iter->second.node);
//	map.setMapAtPos(iter->second.pos.x, iter->second.pos.y, MAP_NONE);
//
//	calculateBombArea(iter->second);
//
//	if (iter->second.bombType == NON_NPC_BOMB){
//		nonNPCPlayer.bombAvailable++;
//	} else {
//		NPCPlayer.bombAvailable++;
//	}
//	bombPool.erase(iter);
//}
//
//void GameScene::calculateBombArea(Bomb &bomb)
//{
//	// middle
//	if (nonNPCPlayer.pos == bomb.pos){
//		if (nonNPCPlayer.invincible <= 0){
//			nonNPCPlayer.invincible = 3.0f;
//			nonNPCPlayer.health--;
//		}
//	}
//	if (NPCPlayer.pos == bomb.pos){
//		if (NPCPlayer.invincible <= 0){
//			NPCPlayer.invincible = 3.0f;
//			NPCPlayer.health--;
//		}
//	}
//	// DOWN
//	for (int i = 1; i <= bomb.power; ++i){
//		if (map.getMapAtPos(bomb.pos.x, bomb.pos.y+i) == MAP_INDESTROYABLE){
//			break;
//		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y+i) == MAP_DESTROYABLE){
//			int x = bomb.pos.x;
//			int y = bomb.pos.y+i;
//			mSceneMgr->destroySceneNode(mapNode[x][y]);
//			map.setMapAtPos(bomb.pos.x, bomb.pos.y+i, MAP_NONE);
//			addBonus(Ogre::Vector2(x,y));
//			break;
//		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y+i) == MAP_NON_NPC){
//			if (nonNPCPlayer.invincible <= 0){
//				nonNPCPlayer.invincible = 3.0f;
//				nonNPCPlayer.health--;
//			}
//		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y+i) == MAP_NPC){
//			if (NPCPlayer.invincible <= 0){
//				NPCPlayer.invincible = 3.0f;
//				NPCPlayer.health--;
//			}
//		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y+i) == MAP_BOMB){
//			for (std::map<int, Bomb>::iterator iter = bombPool.begin(); iter != bombPool.end(); ++iter){
//				if(iter->second.pos.x == bomb.pos.x && iter->second.pos.y == bomb.pos.y+i){
//					explodeBomb(iter->first);
//					break;
//				}
//			}
//		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y+i) < 0){
//			int x = bomb.pos.x;
//			int y = bomb.pos.y+i;
//			mSceneMgr->destroySceneNode(mapNode[x][y]);
//			map.setMapAtPos(x, y, MAP_NONE);
//		}
//	}
//	// UP
//	for (int i = 1; i <= bomb.power; ++i){
//		if (map.getMapAtPos(bomb.pos.x, bomb.pos.y-i) == MAP_INDESTROYABLE){
//			break;
//		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y-i) == MAP_DESTROYABLE){
//			int x = bomb.pos.x;
//			int y = bomb.pos.y-i;
//			mSceneMgr->destroySceneNode(mapNode[x][y]);
//			map.setMapAtPos(bomb.pos.x, bomb.pos.y-i, MAP_NONE);
//			addBonus(Ogre::Vector2(x,y));
//			break;
//		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y-i) == MAP_NON_NPC){
//			if (nonNPCPlayer.invincible <= 0){
//				nonNPCPlayer.invincible = 3.0f;
//				nonNPCPlayer.health--;
//			}
//		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y-i) == MAP_NPC){
//			if (NPCPlayer.invincible <= 0){
//				NPCPlayer.invincible = 3.0f;
//				NPCPlayer.health--;
//			}
//		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y-i) == MAP_BOMB){
//			for (std::map<int, Bomb>::iterator iter = bombPool.begin(); iter != bombPool.end(); ++iter){
//				if(iter->second.pos.x == bomb.pos.x && iter->second.pos.y == bomb.pos.y-i){
//					explodeBomb(iter->first);
//					break;
//				}
//			}
//		} else if (map.getMapAtPos(bomb.pos.x, bomb.pos.y-i) < 0){
//			int x = bomb.pos.x;
//			int y = bomb.pos.y-i;
//
//			mSceneMgr->destroySceneNode(mapNode[x][y]);
//			map.setMapAtPos(x, y, MAP_NONE);
//		}
//	}
//	// RIGHT
//	for (int i = 1; i <= bomb.power; ++i){
//		if (map.getMapAtPos(bomb.pos.x+i, bomb.pos.y) == MAP_INDESTROYABLE){
//			break;
//		} else if (map.getMapAtPos(bomb.pos.x+i, bomb.pos.y) == MAP_DESTROYABLE){
//			int x = bomb.pos.x+i;
//			int y = bomb.pos.y;
//			mSceneMgr->destroySceneNode(mapNode[x][y]);
//			map.setMapAtPos(bomb.pos.x+i, bomb.pos.y, MAP_NONE);
//			addBonus(Ogre::Vector2(x,y));
//			break;
//		} else if (map.getMapAtPos(bomb.pos.x+i, bomb.pos.y) == MAP_NON_NPC){
//			if (nonNPCPlayer.invincible <= 0){
//				nonNPCPlayer.invincible = 3.0f;
//				nonNPCPlayer.health--;
//			}
//		} else if (map.getMapAtPos(bomb.pos.x+i, bomb.pos.y) == MAP_NPC){
//			if (NPCPlayer.invincible <= 0){
//				NPCPlayer.invincible = 3.0f;
//				NPCPlayer.health--;
//			}
//		} else if (map.getMapAtPos(bomb.pos.x+i, bomb.pos.y) == MAP_BOMB){
//			for (std::map<int, Bomb>::iterator iter = bombPool.begin(); iter != bombPool.end(); ++iter){
//				if(iter->second.pos.x == bomb.pos.x+i && iter->second.pos.y == bomb.pos.y){
//					explodeBomb(iter->first);
//					break;
//				}
//			}
//		} else if (map.getMapAtPos(bomb.pos.x+i, bomb.pos.y) < 0){
//			int x = bomb.pos.x+i;
//			int y = bomb.pos.y;
//			mSceneMgr->destroySceneNode(mapNode[x][y]);
//			map.setMapAtPos(x, y, MAP_NONE);
//		}
//	}
//	// LEFT
//	for (int i = 1; i <= bomb.power; ++i){
//		if (map.getMapAtPos(bomb.pos.x-i, bomb.pos.y) == MAP_INDESTROYABLE){
//			break;
//		} else if (map.getMapAtPos(bomb.pos.x-i, bomb.pos.y) == MAP_DESTROYABLE){
//			int x = bomb.pos.x-i;
//			int y = bomb.pos.y;
//			mSceneMgr->destroySceneNode(mapNode[x][y]);
//			map.setMapAtPos(bomb.pos.x-i, bomb.pos.y, MAP_NONE);
//			addBonus(Ogre::Vector2(x,y));
//			break;
//		} else if (map.getMapAtPos(bomb.pos.x-i, bomb.pos.y) == MAP_NON_NPC){
//			if (nonNPCPlayer.invincible <= 0){
//				nonNPCPlayer.invincible = 3.0f;
//				nonNPCPlayer.health--;
//			}
//		} else if (map.getMapAtPos(bomb.pos.x-i, bomb.pos.y) == MAP_NPC){
//			if (NPCPlayer.invincible <= 0){
//				NPCPlayer.invincible = 3.0f;
//				NPCPlayer.health--;
//			}
//		} else if (map.getMapAtPos(bomb.pos.x-i, bomb.pos.y) == MAP_BOMB){
//			for (std::map<int, Bomb>::iterator iter = bombPool.begin(); iter != bombPool.end(); ++iter){
//				if(iter->second.pos.x == bomb.pos.x-i && iter->second.pos.y == bomb.pos.y){
//					explodeBomb(iter->first);
//					break;
//				}
//			}
//		} else if (map.getMapAtPos(bomb.pos.x-i, bomb.pos.y) < 0){
//			int x = bomb.pos.x-i;
//			int y = bomb.pos.y;
//			mSceneMgr->destroySceneNode(mapNode[x][y]);
//			map.setMapAtPos(x, y, MAP_NONE);
//		}
//	}
//}
//
//void GameScene::updateBombInfo(const Ogre::FrameEvent &evt)
//{
//	for (std::map<int, Bomb>::iterator iter = bombPool.begin(); iter != bombPool.end();){
//		iter->second.countDown-=evt.timeSinceLastFrame;
//		if (iter->second.countDown <= 0){
//			std::map<int, Bomb>::iterator tempIter = iter;
//			explodeBomb(tempIter->first);
//			break;
//		} else {
//			iter++;
//		}
//	}
//}

//int GameScene::updatePlayerInfo(const Ogre::FrameEvent& evt)
//{
//	if (NPCPlayer.invincible > 0){
//		NPCPlayer.invincible -= evt.timeSinceLastFrame;
//	}
//	if (NPCPlayer.health <= 0){
//		//.....
//		return 1;
//	}
//	if (nonNPCPlayer.invincible > 0){
//		nonNPCPlayer.invincible -= evt.timeSinceLastFrame;
//	}
//	if (nonNPCPlayer.health <= 0){
//		//....
//		return 1;
//	}
//	return 0;
//}

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

void GameScene::updateMapGridType()
{
	directionType playerDirection = player->getPlayerDirection();
	if (player->_currentGridType == GRID_NORMAL)
	{
		if (playerDirection == LEFT_DIRECTION && player->_rightGridType != GRID_BOMB)
		{
			gameMap->setMapTypeAtGridPos(player->getPlayerXPos()+1,player->getPlayerYPos(),GRID_NORMAL);
		}
		else if (playerDirection == RIGHT_DIRECTION && player->_leftGridType != GRID_BOMB)
		{
			gameMap->setMapTypeAtGridPos(player->getPlayerXPos()-1,player->getPlayerYPos(),GRID_NORMAL);
		}
		else if (playerDirection == UP_DIRECTION && player->_downGridType != GRID_BOMB)
		{
			gameMap->setMapTypeAtGridPos(player->getPlayerXPos(),player->getPlayerYPos()+1,GRID_NORMAL);
		}
		else if (playerDirection == DOWN_DIRECTION && player->_upGridType != GRID_BOMB)
		{
			gameMap->setMapTypeAtGridPos(player->getPlayerXPos(),player->getPlayerYPos()-1,GRID_NORMAL);
		}
	}
	gameMap->setMapTypeAtGridPos(player->getPlayerXPos(),player->getPlayerYPos(),GRID_PLAYER);
}