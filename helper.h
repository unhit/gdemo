#ifndef _HELPER_H_
#define _HELPER_H_

#include "global.h"

void GD_StripColors(char *, int);

int GD_GetTeam(int);
const char * GD_GetTeamName(int);
const char * GD_GetStaticTeamName(int);

char * GD_GetName(int);
const char * GD_GetWeaponName(int);
void GD_InitColoredWeapons(void);
void GD_FreeColoredWeapons(void);
const char * GD_GetFlagStatusName(int);

void GD_ShowServerInfo(void);
void GD_ShowInfo(int);
void GD_ShowDisconnect(int);
void GD_ShowWeaponChange(int, int);
void GD_ShowHit(int, int, int, int);
void GD_ShowMissedHit(int, int);
void GD_ShowKill(int, int, int);
void GD_ShowChat(int, char *);
void GD_ShowTeamChat(int, char *);
void GD_ShowRadioCall(int, int, int, char *);
void GD_ShowNameChange(int, char *);
void GD_ShowWarmup(int);
void GD_ShowLevelStart(void);
void GD_ShowRound(int, char *);
void GD_ShowTimeout(void);

void GD_ShowFlagTaken(int, int);
void GD_ShowFlagDropped(int, int);
void GD_ShowFlagReturned(int);
void GD_ShowFlagCaptured(int, int);

void GD_ResetClientData(int);
void GD_ResetAllClients(void);
void GD_ResetWeaponStats(weaponStats_t *);

void GD_ColorName(int);

void GD_ShowViewAngles(void);
void GD_ShowPosition(void);

int GD_IsZero(float);
int GD_GetMapTime(void);
int GD_GetRoundTime(void);

int GD_IsRoundGametype(void);

#endif
