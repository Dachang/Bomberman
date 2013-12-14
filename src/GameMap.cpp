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
	return mapGrid[x][y].typeOfGrid;
}

void GameMap::setMapTypeAtGridPos(int x, int y, gridType typeOfGrid)
{
	mapGrid[x][y].typeOfGrid = typeOfGrid;
}

void GameMap::updateMap()
{
	generateMapAtScene();
}

void GameMap::loadMapFile()
{
	std::ifstream file;
	file.open("map.txt");
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
				}
				else
					mapGrid[i][j].typeOfGrid = GRID_NORMAL;
			}
		}
	}
	else
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
				}
				else
				{
					file >> tempGridData[i][j];
					tempGridData[i][j] = mapGrid[i][j].typeOfGrid;
				}
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
			switch (gt)
			{
			case GRID_WALL:
				mapGrid[i][j].mapEntity = _sceneMgr->createEntity("cube.mesh");
				mapGrid[i][j].mapNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
				mapGrid[i][j].mapNode->setPosition(0,0,0);
				mapGrid[i][j].mapNode->setPosition(mapGrid[i][j].worldPos);
				mapGrid[i][j].mapNode->attachObject(mapGrid[i][j].mapEntity);
				break;
				//其他类型的gridType，待添加
			default:
				break;
			}
		}
	}
}