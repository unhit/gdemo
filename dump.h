#ifndef _DUMP_H_
#define _DUMP_H_

#include "global.h"

#define GD_MAP_FILE             "map"

void Dump_Snap(clSnapshot_t *);
void Dump_PlayerState(playerState_t *);
void Dump_EntityState(entityState_t *);
void Dump_GameState(gameState_t *);

void Dump_CreateOutputFolder(void);
void Dump_MapChanged(void);
void Dump_CreateMapFolder(void);
void Dump_CreateMapFile(void);

#endif
