#ifndef CFG_BOSS_H
#define CFG_BOSS_H

/************************************************************************/
/* Author: daijunhua                                                    */
/************************************************************************/
#include "HawkUtil.h"

#pragma pack(push)
#pragma pack(1)

struct Boss
{
	//ID
	int ID;
	//ս��ID
	int BossBattle;
	//����������
	int MaxPlayer;
	//��ߵȼ���
	int LvlDiff;
	//����
	int WeekDay;
	//����Сʱ
	int Hour;
	//��������
	int Minute;
	//��ǰ��������
	int PrepareTime;
	//��������
	int BossPeriod;
	//ս����ȴ����
	int BattleCD;
	//���CD����Ԫ��
	int CDMoney;
	//ս���˺�ϵ��
	float FightDmgRate;
	//ǰ������
	unsigned char RankAward[64];
	//��ͨ����
	int CommonAward;
	//���һ������
	int KillerAward;
	//������
	int SubstituteAward;
};

#pragma pack(pop)

#endif
