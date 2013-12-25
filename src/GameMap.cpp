#include "GameMap.h"

GameMap::GameMap(Ogre::SceneManager* sceneMgr)
{
	_sceneMgr = sceneMgr;
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
void GameMap::Update()
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
					mapGrid[i][j].mapEntity->setMaterialName("Examples/Rockwall");
					mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
					mapGrid[i][j].mapNode->setPosition(0,0,0);
					mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
					mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
					mapGrid[i][j].playerIsInTheGrid = false;
					break;
				case GRID_BOMB:
					mapGrid[i][j].mapEntity = _sceneMgr->createEntity("sphere.mesh");
					mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
					mapGrid[i][j].mapNode->setScale(0.5,0.5,0.5);
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
					mapGrid[i][j].mapEntity = _sceneMgr->createEntity("sphere.mesh");
					//mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
					mapGrid[i][j].mapNode->setScale(0.5,0.5,0.5);
					mapGrid[i][j].mapNode->setPosition(0,0,0);
					mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
					mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
					mapGrid[i][j].gridTypeIsAbleToChange = true;
					mapGrid[i][j].playerIsInTheGrid = false;
					break;
				case GRID_ADD_SPEED:
					mapGrid[i][j].mapEntity = _sceneMgr->createEntity("ShaderSystem.mesh");
					mapGrid[i][j].gridTypeIsAbleToChange = true;
					mapGrid[i][j].mapNode->setScale(0.5,0.5,0.5);
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
}

void GameMap::loadMapFile()
{
	std::ifstream file;
	file.open("map2.txt");
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
		//for (int i=0; i<MAP_WIDTH; i++)
		//{
		//	for (int j=0; j<MAP_HEIGHT; j++)
		//	{
		//		mapGrid[i][j].gridPos = Ogre::Vector2(i,j);
		//		mapGrid[i][j].worldPos = convertGridPosToWorldPos(mapGrid[i][j].gridPos);
		//		if (i==0 || i==MAP_WIDTH - 1 || j==0 || j== MAP_HEIGHT - 1)
		//		{
		//			mapGrid[i][j].typeOfGrid = GRID_WALL;
		//			mapGrid[i][j].lastTypeOfGrid = GRID_WALL;
		//		}
		//		else
		//		{
		//			file >> tempGridData[i][j];
		//			mapGrid[i][j].typeOfGrid = (gridType)tempGridData[i][j];
		//		}
		//	}
		//}
		int temp=0;
		file>>temp;

		int i=0;
		int j=0;

		while(!file.eof())
		{
			mapGrid[i][j].gridPos = Ogre::Vector2(i,j);
			mapGrid[i][j].worldPos = convertGridPosToWorldPos(mapGrid[i][j].gridPos);
			tempGridData[i][j]=temp;
			mapGrid[i][j].typeOfGrid = (gridType)tempGridData[i][j];

			file>>temp;

			//
			i++;
			if (i >= MAP_WIDTH)
			{
				i = 0;
				j++;
			}


		}
	}
}

Ogre::Vector3 GameMap::convertGridPosToWorldPos(Ogre::Vector2 pos)
{
	Ogre::Vector3 rtn;
	rtn.y = 0;
	rtn.x = MAP_GRID_SIZE * (pos.x - 12);
	rtn.z = MAP_GRID_SIZE * (pos.y - 10);
	return rtn;
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
				mapGrid[i][j].mapEntity->setMaterialName("Examples/Rockwall");
				mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
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
				mapGrid[i][j].mapEntity = _sceneMgr->createEntity("tudorhouse.mesh");
				mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
				mapGrid[i][j].gridTypeIsAbleToChange = true;
				mapGrid[i][j].mapNode->setPosition(0,0,0);
				mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
				mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
				mapGrid[i][j].playerIsInTheGrid = false;
				break;
			case GRID_ADD_SPEED:
				mapGrid[i][j].mapEntity = _sceneMgr->createEntity("ShaderSystem.mesh");
				mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
				mapGrid[i][j].gridTypeIsAbleToChange = true;
				mapGrid[i][j].mapNode->setPosition(0,0,0);
				mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
				mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
				mapGrid[i][j].playerIsInTheGrid = false;
				break;
			case GRID_DISTORYABLE_WALL:
				mapGrid[i][j].mapEntity = _sceneMgr->createEntity("cube.mesh");
				mapGrid[i][j].mapEntity->setMaterialName("WoodPallet");
				mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
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