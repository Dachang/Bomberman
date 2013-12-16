#include "AIBrain.h"
#include <iostream>
#include <stack>
AIBrain::AIBrain(void)
{
	//AIState = AI_STAT_WAIT;
	myCurrenType=AI_WAIT;

	playerOriginalWeight=100;
	bonusOriginalWeight=60;
	attenuationOfWeight=5;
}

AIBrain::~AIBrain(void)
{
}

EnemyAIMotionType AIBrain::Update(EnemyAIMotionType newType,GameMap* map,Ogre::Vector2 position)
{
	//update type
	myCurrenType=newType;

	//get new map
	gameMap=map;

	//update myself player bonus positions
	UpdateAllPosition(position);

	//think what should do
	Thinking();
	
	//save old type
	myLastType=myCurrenType;
	//test
	//int temp=rand() % 5;
	//switch(temp)
	//{
	//	case 0:
	//		myCurrenType=AI_WAIT;
	//		break;
	//	case 1:
	//		myCurrenType=AI_LEFT;
	//		break;
	//	case 2:
	//		myCurrenType=AI_RIGHT;
	//		break;
	//	case 3:
	//		myCurrenType=AI_UP;
	//		break;
	//	case 4:
	//		myCurrenType=AI_DOWN;
	//		break;

	//}
	//return myCurrenType;
	return myCurrenType;
}
void AIBrain::UpdateAllPosition(Ogre::Vector2 position)
{
	myPosition=position;
	SetOthersPosition();

	//init flood map and Weight Table
	for(int x=0;x<MAP_WIDTH;x++ )
	{
		for (int y=0;y<MAP_HEIGHT;y++)
		{
			FloodMapForPlayer[x][y]=0;
			FloodMapForBonus[x][y]=0;
			WeightTable[x][y]=-1;
		}

	}

	// init level
	PlayerFillLevel=0;
	BonusFillLevel=0;

}
void AIBrain::SetOthersPosition()
{
	for(int x=0;x<MAP_WIDTH;x++ )
	{
		for (int y=0;y<MAP_HEIGHT;y++)
		{
			gridType tempType=gameMap->getMapTypeAtGridPos(x,y);
			if(tempType==GRID_PLAYER)
			{
				playerPosition.x=x;
				playerPosition.y=y;
			}

			else if(tempType==GRID_ADD_BOMB||
					tempType==GRID_ADD_HEALTH||
					tempType==GRID_ADD_SPEED)
			{
				bonusPosition.x=x;
				bonusPosition.y=y;
			}
		}
		
	}
}

void AIBrain::Thinking()
{
	//get the distance between enemy and player
	double distanceToPlayer =Distance(myPosition,playerPosition);
	double distanceToBonus=Distance(myPosition,bonusPosition);
	//attach mode
	if(distanceToPlayer<4 )
	{
		myCurrenType=AI_BOMB;
	}
	else// if(distanceToPlayer>4 )
	{
		//get the nearest bonus position and the player position
		//compare two distances

		//use floodfill and calculate two weights respectively
		FloodFillForPlayer(myPosition.x,myPosition.y,1);
		traceBack(playerPosition.x,playerPosition.y);
		//and add two weights ( to decide which direction to go)

		//if in the best direction  there is wall or box (although in the first step , if there is a wall or box it will flood it)
		//and they will change the weight
		//wall weight = -1 box set a bomb 

		//judge the dangerous level of directions
		//dangerous=-1

		// finally choose the best direction and action
		ChooseType();
	}
}

//flood fill 
 void AIBrain::FloodFillForPlayer(int x,int y,int level)
{
	if (x < 0 || y < 0 || x > MAP_WIDTH || y > MAP_HEIGHT){
		return;
	}
	FloodMapForPlayer[x][y] = level;
	if(x==playerPosition.x&&y==playerPosition.y)
	{
		//find the player
		//keep the level
		if(PlayerFillLevel==0)
		{
			PlayerFillLevel=level;
		}
		else
		{
			if(level<PlayerFillLevel)
			{
				PlayerFillLevel=level;
			}
		}
	}
	if(FloodMapForPlayer[x-1][y]==0||FloodMapForPlayer[x-1][y]>level+1)
	{
		FloodFillForPlayer(x-1,y,level+1);
	}
	//else if(FloodMapForPlayer[x-1][y]>level+1)
	//{
	//	
	//}
	if(FloodMapForPlayer[x][y-1]==0||FloodMapForPlayer[x][y-1]>level+1)
	{
		FloodFillForPlayer(x,y-1,level+1);
	}
	//else if(FloodMapForPlayer[x][y-1]>level+1)
	//{

	//}
	if(FloodMapForPlayer[x+1][y]==0||FloodMapForPlayer[x+1][y]>level+1)
	{
		FloodFillForPlayer(x+1,y,level+1);
	}
	//else if(FloodMapForPlayer[x+1][y]>level+1)
	//{

	//}
	if(FloodMapForPlayer[x][y+1]==0||FloodMapForPlayer[x][y+1]>level+1)
	{
		FloodFillForPlayer(x,y+1,level+1);
	}
	//else if(FloodMapForPlayer[x][y+1]>level+1)
	//{

	//}
}

 void AIBrain::traceBack(int _x, int _y)
 {
 	int destinationLevel = FloodMapForPlayer[_x][_y];
	int temp=destinationLevel;
 	int x = _x;
 	int y = _y;
 	for (; temp >= 3; --temp){
 		if (FloodMapForPlayer[x-1][y] == (temp-1)){
 			x = x-1;
 			continue;
 		}
 		if (FloodMapForPlayer[x+1][y] == (temp-1)){
 			x = x+1;
 			continue;
 		}
 		if (FloodMapForPlayer[x][y-1] == (temp-1)){
 			y = y-1;
 			continue;
 		}
 		if (FloodMapForPlayer[x][y+1] == (temp-1)){
 			y = y+1;
 			continue;
 		}
 	}
	if(FloodMapForPlayer[x][y]==2)
	{
		WeightTable[x][y] =playerOriginalWeight-  destinationLevel*attenuationOfWeight;
	}
 	//if (getValue(x-1,y) == 1){
 	//	return AI_RIGHT;
 	//}
 	//if (getValue(x+1,y) == 1){
 	//	return AI_LEFT;
 	//}
 	//if (getValue(x,y-1) == 1){
 	//	return AI_DOWN;
 	//}
 	//if (getValue(x,y+1) == 1){
 	//	return AI_UP;
 	//}
 	//return AI_WAIT;
 }

 void AIBrain::ChooseType()
 {
	 int currentWeight=WeightTable[(int)myPosition.x-1][(int)myPosition.y];
	 myCurrenType=AI_LEFT;

	 if(WeightTable[(int)myPosition.x+1][(int)myPosition.y]>currentWeight)
	 {
		 currentWeight=WeightTable[(int)myPosition.x+1][(int)myPosition.y];
		 myCurrenType=AI_RIGHT;
	 }
	 if (WeightTable[(int)myPosition.x][(int)myPosition.y-1]>currentWeight)
	 {
		 currentWeight=WeightTable[(int)myPosition.x][(int)myPosition.y-1];
		 myCurrenType=AI_UP;
	 }
	 if (WeightTable[(int)myPosition.x][(int)myPosition.y+1]>currentWeight)
	 {
		 currentWeight=WeightTable[(int)myPosition.x][(int)myPosition.y+1];
		 myCurrenType=AI_DOWN;
	 }

 }

double AIBrain::Distance(Ogre::Vector2 from,Ogre::Vector2 to)
{
	return sqrt((double)(((from.x-to.x)*(from.x-to.x))+((from.y-to.y)*(from.y-to.y))));
}
Ogre::Vector2 AIBrain::convertWorldPosToGridPos(Ogre::Vector3 pos)
{
	Ogre::Vector2 rtn;
	rtn.x = pos.x/120 + 12;
	rtn.y = pos.z/120 + 10;
	return rtn;
}
//void AIBrain::getFarthestInfo()
//{
//	int farthestLevel = 0;
//	for (int x = 0; x < 15; ++x){
//		for (int y = 0; y < 13; ++y){
//			if (mapFlood[x][y] > farthestLevel){
//				farthestLevel = mapFlood[x][y];
//				farthestX = x;
//				farthestY = y;
//			}
//		}
//	}
//}
//int AIBrain::calculateNPCNextEvent(BMPlayer &nonNPC, BMPlayer &npc, Map &map)
//{
//	npcPosX = npc.pos.x;
//	npcPosY = npc.pos.y;
//
//	for (int x = 0; x < 15; ++x){
//		for (int y = 0; y < 13; ++y){
//			if (map.getMapAtPos(x, y) >= 3) {
//				mapFlood[x][y] = -1;
//			} else {
//				mapFlood[x][y] = 0;
//			}
//		}
//	}
//
//	if (AIState == AI_STAT_WAIT){
//		
//		calculateRecursive(npc.pos.x, npc.pos.y, 1);
//		getFarthestInfo();
//		//output();
//		if (npc.bombAvailable > 0){
//			AIState = AI_STAT_NPC;
//		} else {
//			AIState = AI_STAT_FARTHER;
//		}
//		return AI_BOMB;
//	} else if (AIState == AI_STAT_NPC){
//		if (nonNPC.pos == npc.pos){
//			AIState = AI_STAT_WAIT;
//			return AI_WAIT;
//		}
//		calculateRecursive(npc.pos.x, npc.pos.y, 1);
//		int x = nonNPC.pos.x;
//		int y = nonNPC.pos.y;
//		if (mapFlood[x][y] == 0){
//			AIState = AI_STAT_FARTHER;
//			return AI_WAIT;
//		}
//		//output();
//		return traceBack(nonNPC.pos.x, nonNPC.pos.y);
//	} else if (AIState == AI_STAT_FARTHER){
//		if (npc.pos.x == farthestX && npc.pos.y == farthestY){
//			AIState = AI_STAT_WAIT;
//			return AI_WAIT;
//		}
//		calculateRecursive(npc.pos.x, npc.pos.y, 1);
//		//output();
//		return traceBack(farthestX, farthestY);
//	}
//
//	return AI_WAIT;
//}
//
//void AIBrain::calculateRecursive(int x, int y, int level)
//{
//	if (x < 0 || y < 0 || x > 14 || y > 12){
//		return;
//	}
//	mapFlood[x][y] = level;
//	if (getValue(x-1,y) != -1){
//		if (getValue(x-1,y) == 0){
//			calculateRecursive(x-1, y, level+1);
//		} else if (getValue(x-1,y) > (level+1)){
//			calculateRecursive(x-1, y, level+1);
//		}
//	}
//	if (getValue(x,y-1) != -1){
//		if (getValue(x,y-1) == 0){
//			calculateRecursive(x, y-1, level+1);
//		} else if (getValue(x,y-1) > (level+1)){
//			calculateRecursive(x, y-1, level+1);
//		}
//	}
//	if (getValue(x+1,y) != -1){
//		if (getValue(x+1,y) == 0){
//			calculateRecursive(x+1, y, level+1);
//		} else if (getValue(x+1,y) > (level+1)){
//			calculateRecursive(x+1, y, level+1);
//		}
//	}
//	if (mapFlood[x][y+1] != -1){
//		if (mapFlood[x][y+1] == 0){
//			calculateRecursive(x, y+1, level+1);
//		} else if (mapFlood[x][y+1] > (level+1)){
//			calculateRecursive(x, y+1, level+1);
//		}
//	}
//}
//
//void AIBrain::output()
//{
//	for (int y = 0; y < 13; ++y){
//		for (int x = 0; x < 15; ++x){
//			std::cout << mapFlood[x][y] << " ";
//
//		}
//		std::cout << std::endl;
//	}
//	std::cout << std::endl << std::endl << std::endl;
//}
//
//int AIBrain::getValue(int x, int y)
//{
//	if (x >= 0 && x <= 14 && y >= 0 && y <= 12)
//		return mapFlood[x][y];
//	else
//		return -1;
//}
//

//
