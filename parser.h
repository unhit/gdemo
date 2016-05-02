#ifndef _PARSER_H_
#define _PARSER_H_

void GD_GameStateParsed(void);
void GD_SnapshotParsed(void);

void GD_UpdateClientData(int, char *);
char * GD_GetConfigString(gameState_t *, int);

void GD_ParseEntity(centity_t *);

void GD_ReadDemoHeader(void);

qboolean GD_ParseNextMessage(void);

void GD_ParseHit(void);
void GD_ParseMissHit(void);
void GD_ParseKill(void);
void GD_ParseWeaponChange(void);
void GD_ParseFlagStatus(char *);
void GD_ParseLevelStartTime(char *);
void GD_ParseWarmup(char *);
void GD_ParseRound(char *);
void GD_ParseTimeout(void);
void GD_ParseFollowPowerup(int);
void GD_MapChanged(void);
void GD_CheckWeaponChange(int, int);

#endif
