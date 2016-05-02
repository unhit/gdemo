#ifndef _STATS_H_
#define _STATS_H_

void GD_ShowIndividualStats(int);
void GD_ShowWeaponStats(int, int);

int GD_GetKillsSum(int);
int GD_GetDeathsSum(int);
int GD_GetHitsSum(int);
int GD_GetMissedHitsSum(int);
int GD_GetDamageSum(int);

float GD_GetAccuracy(int, int);
float GD_KDRatio(int, int);

#endif
