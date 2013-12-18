#ifndef __GAMEMAP_H_
#define __GAMEMAP_H_
#include <Ogre.h>
#include <fstream>

#define MAP_WIDTH 25
#define MAP_HEIGHT 20
#define MAP_GRID_SIZE 120

typedef enum gridType
{
	GRID_ADD_HEALTH = 0,
	GRID_ADD_SPEED,
	GRID_ADD_BOMB,
	GRID_NORMAL,
	GRID_WALL,
	GRID_DISTORYABLE_WALL,
	GRID_ENEMY,
	GRID_PLAYER,
	GRID_BOMB,
	GRID_BOMB_POWER
};

typedef struct grid
{
	gridType typeOfGrid;
	gridType typeOfLastGrid;
	Ogre::Vector3 worldPos;
	Ogre::Vector2 gridPos;
	Ogre::Entity* mapEntity;
	Ogre::SceneNode* mapNode;
};

class GameMap
{
public:
	GameMap(Ogre::SceneManager* sceneMgr);
	~GameMap(void);
	gridType getMapTypeAtGridPos(int x, int y);
	void setMapTypeAtGridPos(int x, int y, gridType typeOfGrid);
	void Update();
protected:
private:
	Ogre::SceneManager* _sceneMgr;
	grid mapGrid[MAP_WIDTH][MAP_HEIGHT];
	int tempGridData[MAP_WIDTH][MAP_HEIGHT];
	void loadMapFile();
	Ogre::Vector3 convertGridPosToWorldPos(Ogre::Vector2 pos);
	void generateMapAtScene();
};

#endif