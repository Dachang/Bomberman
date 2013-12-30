#include "GameMap.h"
#include "shlobj.h"  
#include <STRING>

float assumeTime = 0;

GameMap::GameMap(Ogre::SceneManager* sceneMgr)
{
	_sceneMgr = sceneMgr;
	_destoryParticleDuration = 2.0;
	loadMapFile();
	generateMapAtScene();
}

GameMap::~GameMap()
{
}

gridType GameMap::getMapTypeAtGridPos(int x, int y)
{
	if((x<MAP_WIDTH&&x>=0)
		&&(y<MAP_HEIGHT&&y>=0))
	{
		return mapGrid[x][y].typeOfGrid;
	}
	return GRID_NOTHING;
	
}

gridType GameMap::getLastMapTypeAtGridPos(int x, int y)
{
	return mapGrid[x][y].lastTypeOfGrid;
}

void GameMap::setMapTypeAtGridPos(int x, int y, gridType typeOfGrid)
{
	if((x<MAP_WIDTH&&x>=0)
		&&(y<MAP_HEIGHT&&y>=0))
	{
		gridType tempGridType = mapGrid[x][y].typeOfGrid;
		mapGrid[x][y].typeOfGrid = typeOfGrid;
		mapGrid[x][y].lastTypeOfGrid = tempGridType;
		mapGrid[x][y].playerIsInTheGrid = false;
	}
}

int GameMap::getBombNum()
{
	int countNum = 0;
	for (int i=0;i<MAP_WIDTH;i++)
	{
		for (int j=0;j<MAP_HEIGHT;j++)
		{
			if (mapGrid[i][j].typeOfGrid == GRID_BOMB)
			{
				countNum++;
			}
		}
	}
	return countNum;
}
void GameMap::updateGridWithPlayerPosition(int x, int y, bool isPlayerIn)
{
	mapGrid[x][y].playerIsInTheGrid = isPlayerIn;
}

bool GameMap::isPlayerInTheGrid(int x, int y)
{
	return mapGrid[x][y].playerIsInTheGrid;
}
void GameMap::Update(const Ogre::FrameEvent& evt)
{
	for (int i=0; i<MAP_WIDTH; ++i)
	{
		for (int j=0; j<MAP_HEIGHT; ++j)
		{
			if (mapGrid[i][j].typeOfGrid != mapGrid[i][j].lastTypeOfGrid && mapGrid[i][j].gridTypeIsAbleToChange)
			{
				switch(mapGrid[i][j].typeOfGrid)
				{
				case GRID_WALL:
					mapGrid[i][j].mapEntity = _sceneMgr->createEntity("cube.mesh");
					mapGrid[i][j].mapEntity->setCastShadows(false);
					mapGrid[i][j].mapEntity->setMaterialName("Examples/Rockwall");
					mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
					mapGrid[i][j].mapNode->setScale(1.2,1.2,1.2);
					mapGrid[i][j].mapNode->setPosition(0,0,0);
					mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
					mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
					mapGrid[i][j].playerIsInTheGrid = false;
					break;
				case GRID_BOMB:
					mapGrid[i][j].mapEntity = _sceneMgr->createEntity("bombSphere01.mesh");
					mapGrid[i][j].mapEntity->setCastShadows(false);
					mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
					mapGrid[i][j].mapNode->setScale(100,100,100);
					mapGrid[i][j].mapNode->setPosition(0,0,0);
					mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
					mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
					mapGrid[i][j].gridTypeIsAbleToChange = true;
					mapGrid[i][j].playerIsInTheGrid = false;
					break;
				case GRID_NORMAL:
					//_sceneMgr->destroySceneNode(mapGrid[i][j].mapNode);
					mapGrid[i][j].mapNode->detachAllObjects();

					//_sceneMgr->destroyEntity(mapGrid[i][j].mapEntity);
					mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
					mapGrid[i][j].gridTypeIsAbleToChange = true;
					mapGrid[i][j].playerIsInTheGrid = false;
					break;
				case GRID_BOMB_POWER:
					assumeTime += evt.timeSinceLastFrame;
					mapGrid[i][j].ps = _sceneMgr->createParticleSystem("ExplodeEffectPower"+ Ogre::StringConverter::toString(assumeTime),"Examples/Smoke");
					//mapGrid[i][j].mapEntity = _sceneMgr->createEntity("sphere.mesh");
					//mapGrid[i][j].mapEntity->setCastShadows(false);
					//mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
					mapGrid[i][j].mapNode->setScale(0.5,0.5,0.5);
					mapGrid[i][j].mapNode->setPosition(0,0,0);
					mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
					mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].ps);
					//mapGrid[i][j].mapNode->setPosition(convertGridPosToWorldPos(Ogre::Vector2(mapGrid[i][j].gridPos.x,mapGrid[i][j].gridPos.y)));
					mapGrid[i][j].gridTypeIsAbleToChange = true;
					mapGrid[i][j].playerIsInTheGrid = false;
					break;
					/*case GRID_ADD_SPEED:
					mapGrid[i][j].mapEntity = _sceneMgr->createEntity("ShaderSystem.mesh");
					mapGrid[i][j].mapEntity->setCastShadows(false);
					mapGrid[i][j].gridTypeIsAbleToChange = true;
					mapGrid[i][j].mapNode->setScale(0.5,0.5,0.5);
					mapGrid[i][j].mapNode->setPosition(0,0,0);
					mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
					mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
					mapGrid[i][j].playerIsInTheGrid = false;
					break;*/

				case GRID_ADD_HEALTH:
					mapGrid[i][j].mapEntity = _sceneMgr->createEntity("heart_____HyperNURBS__1.mesh");
					mapGrid[i][j].mapEntity->setCastShadows(false);
					mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
					mapGrid[i][j].gridTypeIsAbleToChange = true;
					mapGrid[i][j].mapNode->setScale(30,30,30);
					mapGrid[i][j].mapNode->setPosition(0,0,0);
					mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
					mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
					mapGrid[i][j].playerIsInTheGrid = false;
					break;
				case GRID_ADD_SPEED:
					mapGrid[i][j].mapEntity = _sceneMgr->createEntity("BonusShoes_a08.mesh");
					mapGrid[i][j].mapEntity->setCastShadows(false);
					mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
					mapGrid[i][j].gridTypeIsAbleToChange = true;
					mapGrid[i][j].mapNode->setScale(2.5,2.5,2.5);
					//mapGrid[i][j].mapNode->setOrientation(0,0,0.5,0.5);
					mapGrid[i][j].mapNode->yaw( Ogre::Degree(90),Ogre::Node::TS_WORLD);
					//mapGrid[i][j].mapNode->roll(Ogre::Degree(90));
					mapGrid[i][j].mapNode->pitch(Ogre::Degree(-90),Ogre::Node::TS_WORLD);

					mapGrid[i][j].mapNode->setPosition(0,0,0);
					mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
					mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
					mapGrid[i][j].playerIsInTheGrid = false;
					break;
				case GRID_ADD_BOMB_POWER:
					mapGrid[i][j].mapEntity = _sceneMgr->createEntity("powerbonusDrawCall_110.mesh");
					mapGrid[i][j].mapEntity->setCastShadows(false);
					mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
					mapGrid[i][j].gridTypeIsAbleToChange = true;
					mapGrid[i][j].mapNode->setScale(70,70,70);
					mapGrid[i][j].mapNode->setPosition(0,0,0);
					mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
					mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
					mapGrid[i][j].playerIsInTheGrid = false;
					break;
				case GRID_ADD_BOMB:
					mapGrid[i][j].mapEntity = _sceneMgr->createEntity("BombBonusSphere004.mesh");
					mapGrid[i][j].mapEntity->setCastShadows(false);
					mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
					mapGrid[i][j].gridTypeIsAbleToChange = true;
					mapGrid[i][j].mapNode->setScale(65,65,65);
					mapGrid[i][j].mapNode->setPosition(0,0,0);
					mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
					mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
					mapGrid[i][j].playerIsInTheGrid = false;
					break;
				default:
					break;
				}
			}
			mapGrid[i][j].lastTypeOfGrid = mapGrid[i][j].typeOfGrid;
		}
	}
	//checkGridParticles(evt);
}

void GameMap::loadMapFile()
{
	std::string szPath("");  
	std::ifstream file;
	//file.open("map.map");
	if(GetFolder(szPath))
	{
		file.open(szPath);
		if (!file)
		{
			for (int i=0; i<MAP_WIDTH; i++)
			{
				for (int j=0; j<MAP_HEIGHT; j++)
				{
					mapGrid[i][j].gridPos = Ogre::Vector2(i,j);
					mapGrid[i][j].worldPos = convertGridPosToWorldPos(mapGrid[i][j].gridPos);
					if (i==0 || i==MAP_WIDTH - 1 || j==0 || j== MAP_HEIGHT - 1)
					{
						mapGrid[i][j].typeOfGrid = GRID_WALL;
						mapGrid[i][j].lastTypeOfGrid = GRID_WALL;
					}
					else if((j==3 && i<=5) || (j==4 && i>=12) || (j==15 && i>=4) && (j==15 && i<=18) || (i==7 && j>=6) && (i==7 && j<=13))
					{
						mapGrid[i][j].typeOfGrid = GRID_WALL;
					}
					else if ((j==9 && i<=12)&&(j==9 && i>=6))
					{
						mapGrid[i][j].typeOfGrid = GRID_DISTORYABLE_WALL;
					}
					else
					{
						mapGrid[i][j].typeOfGrid = GRID_NORMAL;
						mapGrid[i][j].lastTypeOfGrid = GRID_NORMAL;
					}

				}
			}
		}
		else
		{
			int temp=0;
			file>>temp;

			int i=0;
			int j=0;

			while(!file.eof())
			{
				mapGrid[i][j].gridPos = Ogre::Vector2(i,j);
				mapGrid[i][j].worldPos = convertGridPosToWorldPos(mapGrid[i][j].gridPos);
				tempGridData[i][j]=temp;
				if((gridType)tempGridData[i][j]==GRID_PLAYER)
				{
					playerInitPos.x=i;
					playerInitPos.y=j;

					mapGrid[i][j].typeOfGrid = GRID_NORMAL;
				}
				else if((gridType)tempGridData[i][j]==GRID_ENEMY)
				{
					Ogre::Vector2 tempV;
					tempV.x=i;
					tempV.y=j;

					enemyListPos.push_back(tempV);

					mapGrid[i][j].typeOfGrid = GRID_NORMAL;
				}
				else
				{
					mapGrid[i][j].typeOfGrid = (gridType)tempGridData[i][j];
				}
				file>>temp;

				//
				i++;
				if (i >= MAP_WIDTH)
				{
					i = 0;
					j++;
				}


			}
			file.close();
		}
	}
	
	
}

bool GameMap::GetFolder(std::string& folderpath)//, const char* szCaption , HWND hOwner)  
{  
	//bool retVal = false;  

	char szBuffer[800] = {0}; 
	OPENFILENAME ofn= {0}; 
	ofn.lStructSize = sizeof(ofn); 
	ofn.hwndOwner = NULL; 
	char Filter[] ="地图文件(*.map)\0*.map\0所有文件(*.*)\0*.*\0";
	ofn.lpstrFilter = Filter;//要选择的文件后缀 
	char InitialDir[]="D:\\Program Files";
	ofn.lpstrInitialDir = InitialDir;//默认的文件路径 
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区 
	ofn.nMaxFile = sizeof(szBuffer)/sizeof(*szBuffer); 
	ofn.nFilterIndex = 0; 
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER ;//标志如果是多选要加上OFN_ALLOWMULTISELECT
	bool bSel = GetOpenFileName(&ofn); 

	folderpath=szBuffer;

	return bSel;  
} 
Ogre::Vector2 GameMap::getPlayerInitPos()
{
	return playerInitPos;
}
bool  GameMap::getEnemyInitPos(Ogre::Vector2& vec)
{
	if(enemyListPos.size()<=0)
	{
		return false;
	}
	else
	{
		vec=enemyListPos.back();
		enemyListPos.pop_back();
		return true;
	}
}
Ogre::Vector3 GameMap::convertGridPosToWorldPos(Ogre::Vector2 pos)
{
	Ogre::Vector3 rtn;
	rtn.y = 50;
	rtn.x = MAP_GRID_SIZE * (pos.x - 12)+60;
	rtn.z = MAP_GRID_SIZE * (pos.y - 10)+60;
	return rtn;
}
void GameMap::setParticleEffectAtGrid(int x, int y, const Ogre::FrameEvent& evt)
{
	/*Ogre::ParticleSystem::setDefaultNonVisibleUpdateTimeout(1);*/
	assumeTime += evt.timeSinceLastFrame;
	mapGrid[x][y].ps = _sceneMgr->createParticleSystem("ExplodeEffect"+ Ogre::StringConverter::toString(assumeTime),"Examples/Smoke");
	mapGrid[x][y].mapNode->attachObject(mapGrid[x][y].ps);
	mapGrid[x][y].mapNode->setPosition(convertGridPosToWorldPos(Ogre::Vector2(mapGrid[x][y].gridPos.x,mapGrid[x][y].gridPos.y)));
}

void GameMap::destroyParticleEffectAtGrid(int x, int y)
{
	mapGrid[x][y].mapNode->detachAllObjects();
}

void GameMap::generateMapAtScene()
{
	for (int i=0; i < MAP_WIDTH; ++i)
	{
		for (int j=0; j < MAP_HEIGHT; ++j)
		{
			gridType gt = getMapTypeAtGridPos(i,j);
			mapGrid[i][j].lastTypeOfGrid=gt;
			switch (gt)
			{
			case GRID_WALL:
				mapGrid[i][j].mapEntity = _sceneMgr->createEntity("cube.mesh");
				mapGrid[i][j].mapEntity->setCastShadows(false);
				mapGrid[i][j].mapEntity->setMaterialName("Examples/Rockwall");
				mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
				mapGrid[i][j].mapNode->setScale(1.2,1.2,1.2);
				mapGrid[i][j].mapNode->setPosition(0,0,0);
				mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
				mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
				mapGrid[i][j].gridTypeIsAbleToChange = false;
				mapGrid[i][j].playerIsInTheGrid = false;
				break;
			case GRID_NORMAL:
				mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
				mapGrid[i][j].gridTypeIsAbleToChange = true;
				break;
			case GRID_ADD_HEALTH:
				mapGrid[i][j].mapEntity = _sceneMgr->createEntity("heart_____HyperNURBS__1.mesh");
				mapGrid[i][j].mapEntity->setCastShadows(false);
				mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
				mapGrid[i][j].gridTypeIsAbleToChange = true;
				mapGrid[i][j].mapNode->setPosition(0,0,0);
				mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
				mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
				mapGrid[i][j].playerIsInTheGrid = false;
				break;
			case GRID_ADD_SPEED:
				mapGrid[i][j].mapEntity = _sceneMgr->createEntity("ShaderSystem.mesh");
				mapGrid[i][j].mapEntity->setCastShadows(false);
				mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
				mapGrid[i][j].gridTypeIsAbleToChange = true;
				mapGrid[i][j].mapNode->setPosition(0,0,0);
				mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
				mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
				mapGrid[i][j].playerIsInTheGrid = false;
				break;
			case GRID_ADD_BOMB_POWER:
				mapGrid[i][j].mapEntity = _sceneMgr->createEntity("tudorhouse.mesh");
				mapGrid[i][j].mapEntity->setCastShadows(false);
				mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
				mapGrid[i][j].gridTypeIsAbleToChange = true;
				mapGrid[i][j].mapNode->setPosition(0,0,0);
				mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
				mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
				mapGrid[i][j].playerIsInTheGrid = false;
				break;
			case GRID_ADD_BOMB:
				mapGrid[i][j].mapEntity = _sceneMgr->createEntity("ShaderSystem.mesh");
				mapGrid[i][j].mapEntity->setCastShadows(false);
				mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
				mapGrid[i][j].gridTypeIsAbleToChange = true;
				mapGrid[i][j].mapNode->setPosition(0,0,0);
				mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
				mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
				mapGrid[i][j].playerIsInTheGrid = false;
				break;
			case GRID_DISTORYABLE_WALL:
				mapGrid[i][j].mapEntity = _sceneMgr->createEntity("cube.mesh");
				mapGrid[i][j].mapEntity->setCastShadows(false);
				mapGrid[i][j].mapEntity->setMaterialName("WoodPallet");
				mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
				mapGrid[i][j].mapNode->setScale(1.2,1.2,1.2);
				mapGrid[i][j].mapNode->setPosition(0,0,0);
				mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
				mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
				mapGrid[i][j].gridTypeIsAbleToChange = true;
				mapGrid[i][j].playerIsInTheGrid = false;
				break;

			default:
				break;
			}
		}
	}
}