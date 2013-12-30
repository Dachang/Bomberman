#ifndef __GAMEMAP_H_
#define __GAMEMAP_H_
#include <Ogre.h>
#include <fstream>
#include <vector>
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

	GRID_ADD_BOMB_POWER,


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
	Ogre::ParticleSystem* ps;
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
	void Update(const Ogre::FrameEvent& evt);
	Ogre::Vector2  getPlayerInitPos();
	bool  getEnemyInitPos(Ogre::Vector2& vec);
	void setParticleEffectAtGrid(int x, int y, const Ogre::FrameEvent& evt);
	void destroyParticleEffectAtGrid(int x, int y);
protected:
private:
	Ogre::SceneManager* _sceneMgr;
	Ogre::Vector2 playerInitPos;
	std::vector<Ogre::Vector2> enemyListPos;
	grid mapGrid[MAP_WIDTH][MAP_HEIGHT];
	int tempGridData[MAP_WIDTH][MAP_HEIGHT];
	float _destoryParticleDuration;

	void loadMapFile();
	Ogre::Vector3 convertGridPosToWorldPos(Ogre::Vector2 pos);
	void generateMapAtScene();


	int PowerMap[MAP_WIDTH][MAP_HEIGHT];

	bool GetFolder(std::string& folderpath);
};

#endif