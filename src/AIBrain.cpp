#include "AIBrain.h"
#include <iostream>
#include <stack>
#include <time.h> 
AIBrain::AIBrain(void)
{
	//AIState = AI_STAT_WAIT;
	myCurrenType=AI_WAIT;
	motionMode=AI_FIND;
	playerOriginalWeight=400;
	bonusOriginalWeight=400;
	attenuationOfWeight=5;

	lastDistanceToBonus=100000;

	minDistanceToPlayer=rand()%5;
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
	lastDistanceToBonus=100000;
}
void AIBrain::SetOthersPosition()
{
	bonusPosition.x=0;
	bonusPosition.y=0;
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

			if(tempType==GRID_ADD_BOMB
				||tempType==GRID_ADD_HEALTH
				||tempType==GRID_ADD_SPEED)
			{
				//bonusPosition.x=x;
				//bonusPosition.y=y;

				///get the nearest bonus position
				double tempDistance=Distance(myPosition,Ogre::Vector2(x,y));
				if(tempDistance<=lastDistanceToBonus)
				{
					lastDistanceToBonus=tempDistance;
					bonusPosition.x=x;
					bonusPosition.y=y;
				}
			}
		}
	}
}

void AIBrain::Thinking()
{
	//get the distance between enemy and player
	double distanceToPlayer =Distance(myPosition,playerPosition);
	double distanceToBonus=Distance(myPosition,bonusPosition);

	//attack mode
	if(distanceToPlayer<minDistanceToPlayer||motionMode==AI_ATTACK )
	{
		ThinkingForAttacking();
	}
	else// if(distanceToPlayer>4 )
	{
		ThinkingForMoving();
	}
}
void AIBrain::ThinkingForAttacking()
{
	if(motionMode==AI_FIND)
	{
		//首次进入
		motionMode=AI_ATTACK;

		srand(time(0));
		motionSeed=rand()%5;

		srand(motionSeed);

		timeToLiveOfmotion=rand()%150;
		if(timeToLiveOfmotion<50)
		{
			timeToLiveOfmotion+=200;
		}
	}

	else if(motionMode==AI_ATTACK)
	{
		AttackMode();
		if(timeToLiveOfmotion<=0)
		{
			//attack motion finished
			motionMode=AI_FIND;
		}
		timeToLiveOfmotion--;

		if(timeToLiveOfmotion==100)
		{
			motionSeed=(motionSeed+1)%5;
		}
	}



}

void AIBrain::AttackMode()
{

	switch(motionSeed)
	{
	case 0:
		myCurrenType=AI_BOMB;
		motionSeed++;
		break;
	case 1:
		WeightTable[(int)myPosition.x-1][(int)myPosition.y]+=50;
		break;
	case 2:
		WeightTable[(int)myPosition.x+1][(int)myPosition.y]+=50;
		break;
	case 3:
		WeightTable[(int)myPosition.x][(int)myPosition.y-1]+=50;
		break;
	case 4:
		WeightTable[(int)myPosition.x][(int)myPosition.y+1]+=50;
		break;
	default:
		break;

	}

	ChooseType();
}



void AIBrain::ThinkingForMoving()
{
	//ai logic (fin player)
	//use floodfill and calculate two weights respectively
	//and add two weights ( to decide which direction to go)
	//if in the best direction  there is wall or box (although in the first step , if there is a wall or box it will flood it)
	//and they will change the weight
	// finally choose the best direction and action

	motionMode=AI_FIND;

	//if there is a bonus
	if(bonusPosition.x!=0)
	{
		FloodFillForBonus(myPosition.x,myPosition.y,1);
		traceBack(bonusPosition.x,bonusPosition.y,FloodMapForBonus,1);
	}
	FloodFillForPlayer(myPosition.x,myPosition.y,1);
	traceBack(playerPosition.x,playerPosition.y,FloodMapForPlayer,0);

	ChooseType();
}

//flood fill 
void AIBrain::FloodFillForPlayer(int x,int y,int level)
{
	if (x < 0 || y < 0 || x > MAP_WIDTH || y > MAP_HEIGHT){
		return;
	}

	//if this is a wall, return
	//
	//maybe add another type : bomb ...
	//
	if(gameMap->getMapTypeAtGridPos(x,y)==GRID_WALL||gameMap->getMapTypeAtGridPos(x,y)==GRID_DISTORYABLE_WALL)
	{
		FloodMapForPlayer[x][y] = -100;
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
	if(FloodMapForPlayer[x][y-1]==0||FloodMapForPlayer[x][y-1]>level+1)
	{
		FloodFillForPlayer(x,y-1,level+1);
	}
	if(FloodMapForPlayer[x+1][y]==0||FloodMapForPlayer[x+1][y]>level+1)
	{
		FloodFillForPlayer(x+1,y,level+1);
	}
	if(FloodMapForPlayer[x][y+1]==0||FloodMapForPlayer[x][y+1]>level+1)
	{
		FloodFillForPlayer(x,y+1,level+1);
	}
}

//flood fill 
void AIBrain::FloodFillForBonus(int x,int y,int level)
{
	if (x < 0 || y < 0 || x > MAP_WIDTH || y > MAP_HEIGHT){
		return;
	}
	//if this is a wall, return
	//
	//maybe add another type : bomb ...
	//
	if(gameMap->getMapTypeAtGridPos(x,y)==GRID_WALL||gameMap->getMapTypeAtGridPos(x,y)==GRID_DISTORYABLE_WALL)
	{
		FloodMapForBonus[x][y] = -100;
		return;
	}

	FloodMapForBonus[x][y] = level;
	if(x==bonusPosition.x&&y==bonusPosition.y)
	{
		//find the bonus
		//keep the level
		if(BonusFillLevel==0)
		{
			BonusFillLevel=level;
		}
		else
		{
			if(level<BonusFillLevel)
			{
				BonusFillLevel=level;
			}
		}
	}
	if(FloodMapForBonus[x-1][y]>=0&&(FloodMapForBonus[x-1][y]==0||FloodMapForBonus[x-1][y]>level+1))
	{
		FloodFillForBonus(x-1,y,level+1);
	}
	if(FloodMapForBonus[x][y-1]>=0&&(FloodMapForBonus[x][y-1]==0||FloodMapForBonus[x][y-1]>level+1))
	{
		FloodFillForBonus(x,y-1,level+1);
	}
	if(FloodMapForBonus[x+1][y]>=0&&(FloodMapForBonus[x+1][y]==0||FloodMapForBonus[x+1][y]>level+1))
	{
		FloodFillForBonus(x+1,y,level+1);
	}
	if(FloodMapForBonus[x][y+1]>=0&&(FloodMapForBonus[x][y+1]==0||FloodMapForBonus[x][y+1]>level+1))
	{
		FloodFillForBonus(x,y+1,level+1);
	}
}

//PlayerOrBonus 0 = player
//PlayerOrBonus 1= bonus
void AIBrain::traceBack(int _x, int _y,const int map[MAP_WIDTH][MAP_HEIGHT],int PlayerOrBonus)
{
	int destinationLevel = map[_x][_y];
	int temp=destinationLevel;
	int x = _x;
	int y = _y;
	for (; temp >= 3; --temp){
		if (map[x-1][y] == (temp-1)){
			x = x-1;
			continue;
		}
		if (map[x+1][y] == (temp-1)){
			x = x+1;
			continue;
		}
		if (map[x][y-1] == (temp-1)){
			y = y-1;
			continue;
		}
		if (map[x][y+1] == (temp-1)){
			y = y+1;
			continue;
		}
	}
	if(map[x][y]==2)
	{
		if(PlayerOrBonus==0)
		{
			WeightTable[x][y] +=playerOriginalWeight - destinationLevel*attenuationOfWeight;
		}
		else
		{
			WeightTable[x][y] +=bonusOriginalWeight - destinationLevel*attenuationOfWeight;
		}
	}
}
void AIBrain::DetectionForThingsUnableAcross()
{
	//met with wall or destroryable wall
	//
	//maybe add another type : bomb ...
	//
	if(gameMap->getMapTypeAtGridPos(myPosition.x-1,myPosition.y)==GRID_WALL||
		gameMap->getMapTypeAtGridPos(myPosition.x-1,myPosition.y)==GRID_DISTORYABLE_WALL)
	{
		WeightTable[(int)myPosition.x-1][(int)myPosition.y]+=-100; 
		timeToLiveOfmotion=0;
	}
	if(gameMap->getMapTypeAtGridPos(myPosition.x+1,myPosition.y)==GRID_WALL||
		gameMap->getMapTypeAtGridPos(myPosition.x+1,myPosition.y)==GRID_DISTORYABLE_WALL)
	{
		WeightTable[(int)myPosition.x+1][(int)myPosition.y]+=-100; 
		timeToLiveOfmotion=0;
	}
	if(gameMap->getMapTypeAtGridPos(myPosition.x,myPosition.y-1)==GRID_WALL||
		gameMap->getMapTypeAtGridPos(myPosition.x,myPosition.y-1)==GRID_DISTORYABLE_WALL)
	{
		WeightTable[(int)myPosition.x][(int)myPosition.y-1]+=-100; 
		timeToLiveOfmotion=0;
	}
	if(gameMap->getMapTypeAtGridPos(myPosition.x,myPosition.y+1)==GRID_WALL||
		gameMap->getMapTypeAtGridPos(myPosition.x,myPosition.y+1)==GRID_DISTORYABLE_WALL)
	{
		WeightTable[(int)myPosition.x][(int)myPosition.y+1]+=-100; 
		timeToLiveOfmotion=0;
	}

	//add weight in the direction without wall 
	if(gameMap->getMapTypeAtGridPos(myPosition.x-1,myPosition.y)!=GRID_WALL&&
		gameMap->getMapTypeAtGridPos(myPosition.x-1,myPosition.y)!=GRID_DISTORYABLE_WALL)
	{
		srand((unsigned)time(0));
		WeightTable[(int)myPosition.x-1][(int)myPosition.y]+= (rand()&7)*1;
	}
	if(gameMap->getMapTypeAtGridPos(myPosition.x+1,myPosition.y)!=GRID_WALL
		&&gameMap->getMapTypeAtGridPos(myPosition.x+1,myPosition.y)!=GRID_DISTORYABLE_WALL)
	{
		srand(2);
		WeightTable[(int)myPosition.x+1][(int)myPosition.y]+= (rand()&7)*1;
	}
	if(gameMap->getMapTypeAtGridPos(myPosition.x,myPosition.y-1)!=GRID_WALL&&
		gameMap->getMapTypeAtGridPos(myPosition.x,myPosition.y-1)!=GRID_DISTORYABLE_WALL)
	{
		srand(3);
		WeightTable[(int)myPosition.x][(int)myPosition.y-1]+= (rand()&7)*1;
	}
	if(gameMap->getMapTypeAtGridPos(myPosition.x,myPosition.y+1)!=GRID_WALL&&
		gameMap->getMapTypeAtGridPos(myPosition.x,myPosition.y+1)!=GRID_DISTORYABLE_WALL)
	{
		srand(4);
		WeightTable[(int)myPosition.x][(int)myPosition.y+1]+= (rand()&4)*1;
	}
}

void AIBrain::DetectionForDestroyableWall()
{
	//if there are destroyable wall , update the state to bomb
	if(gameMap->getMapTypeAtGridPos(myPosition.x-1,myPosition.y)==GRID_DISTORYABLE_WALL
		||gameMap->getMapTypeAtGridPos(myPosition.x+1,myPosition.y)==GRID_DISTORYABLE_WALL
		||gameMap->getMapTypeAtGridPos(myPosition.x,myPosition.y-1)==GRID_DISTORYABLE_WALL
		||gameMap->getMapTypeAtGridPos(myPosition.x,myPosition.y+1)==GRID_DISTORYABLE_WALL)
	{
		//myCurrenType+=1;

		switch(myCurrenType)
		{
		case AI_UP:
			myCurrenType=AI_UP_AND_BOMB;
			break;
		case AI_RIGHT:
			myCurrenType=AI_RIGHT_AND_BOMB;
			break;
		case AI_DOWN:
			myCurrenType=AI_DOWN_AND_BOMB;
			break;
		case AI_LEFT:
			myCurrenType=AI_LEFT_AND_BOMB;
			break;
		}
	}
}

void AIBrain::ChooseType()
{
	//decte
	DetectionForThingsUnableAcross();
	int currentWeight=0;
	if(motionMode==AI_FIND)
	{
		if(WeightTable[(int)myPosition.x-1][(int)myPosition.y]>currentWeight)
		{
			currentWeight=WeightTable[(int)myPosition.x-1][(int)myPosition.y];
			myCurrenType=AI_LEFT;
		}
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

		//判断是否放置炸弹 破坏墙
		DetectionForDestroyableWall();
	}
	else if(motionMode==AI_ATTACK)
	{
		currentWeight=0;
		if(WeightTable[(int)myPosition.x-1][(int)myPosition.y]>currentWeight)
		{
			currentWeight=WeightTable[(int)myPosition.x-1][(int)myPosition.y];
			myCurrenType=AI_LEFT_AND_BOMB;
		}
		if(WeightTable[(int)myPosition.x+1][(int)myPosition.y]>currentWeight)
		{
			currentWeight=WeightTable[(int)myPosition.x+1][(int)myPosition.y];
			myCurrenType=AI_RIGHT_AND_BOMB;
		}
		if (WeightTable[(int)myPosition.x][(int)myPosition.y-1]>currentWeight)
		{
			currentWeight=WeightTable[(int)myPosition.x][(int)myPosition.y-1];
			myCurrenType=AI_UP_AND_BOMB;
		}
		if (WeightTable[(int)myPosition.x][(int)myPosition.y+1]>currentWeight)
		{
			currentWeight=WeightTable[(int)myPosition.x][(int)myPosition.y+1];
			myCurrenType=AI_DOWN_AND_BOMB;
		}
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

