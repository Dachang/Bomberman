#ifndef __ENEMYAIMOTIONTYPE_H__
#define __ENEMYAIMOTIONTYPE_H__

typedef enum EnemyAIMotionType
{
	AI_WAIT=0,
	AI_LEFT,
	AI_RIGHT,
	AI_UP,
	AI_DOWN,
	AI_LEFT_AND_BOMB,
	AI_RIGHT_AND_BOMB,
	AI_UP_AND_BOMB,
	AI_DOWN_AND_BOMB,
	AI_BOMB,
};
#endif