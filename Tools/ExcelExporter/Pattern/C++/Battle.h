#ifndef CFG_BATTLE_H
#define CFG_BATTLE_H

/************************************************************************/
/* Author: daijunhua                                                    */
/************************************************************************/
#include "HawkUtil.h"

#pragma pack(push)
#pragma pack(1)

struct Battle
{
	//ID
	int ID;
	//战场ID
	int BossBattle;
	//容纳最大玩家
	int MaxPlayer;
	//最高等级差
	int LvlDiff;
	//星期
	int WeekDay;
	//开启小时
	int Hour;
	//开启分钟
	int Minute;
	//提前进场秒数
	int PrepareTime;
	//持续秒数
	int BossPeriod;
	//战斗冷却秒数
	int BattleCD;
	//清除CD花费元宝
	int CDMoney;
	//战力伤害系数
	float FightDmgRate;
	//前三奖励
	unsigned char RankAward[64];
	//普通奖励
	int CommonAward;
	//最后一击奖励
	int KillerAward;
	//替身奖励
	int SubstituteAward;
};

#pragma pack(pop)

#endif
