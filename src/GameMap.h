#ifndef __GAMEMAP_H_
#define __GAMEMAP_H_
#include <Ogre.h>
#include <fstream>
#include <OgreParticleSystem.h>

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
	GRID_BOMB_POWER,
	GRID_ADD_POWER,
	GRID_REDUCE_POWER,
	GRID_NOTHING,

};

typedef struct grid
{
	gridType typeOfGrid;
	gridType lastTypeOfGrid;
	Ogre::Vector3 worldPos;
	Ogre::Vector2 gridPos;
	Ogre::Entity* mapEntity;
	Ogre::SceneNode* mapNode;
	bool gridTypeIsAbleToChange;
	bool playerIsInTheGrid;
};

class GameMap
{
public:
	GameMap(Ogre::SceneManager* sceneMgr);
	~GameMap(void);
	gridType getMapTypeAtGridPos(int x, int y);
	gridType getLastMapTypeAtGridPos(int x, int y);
	void setMapTypeAtGridPos(int x, int y, gridType typeOfGrid);
	void updateGridWithPlayerPosition(int x, int y, bool isPlayerIn);
	bool isPlayerInTheGrid(int x, int y);
	int getBombNum();
	void Update();
protected:
private:
	Ogre::SceneManager* _sceneMgr;
	grid mapGrid[MAP_WIDTH][MAP_HEIGHT];
	int tempGridData[MAP_WIDTH][MAP_HEIGHT];
	void loadMapFile();
	Ogre::Vector3 convertGridPosToWorldPos(Ogre::Vector2 pos);
	void generateMapAtScene();


	int PowerMap[MAP_WIDTH][MAP_HEIGHT];
};

#endif