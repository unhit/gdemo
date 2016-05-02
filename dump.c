#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "dump.h"
#include "opts.h"

extern options_t opts;

void Dump_GameState(gameState_t *gs) {
    int i;

    Com_Printf("=== gameState_t ===\n");

    for (i = 0; i < MAX_CONFIGSTRINGS; i++) {
        Com_Printf("stringOffsets[%d] = 0x%x\n", i, gs->stringOffsets[i]);
        Com_Printf("stringData[%d] = %s\n", i, gs->stringData + gs->stringOffsets[i]);
    }
}

void Dump_Snap(clSnapshot_t *snap) {
    Com_Printf("=== clSnapshot_t ===\n");
    Com_Printf("valid=0x%x\n", snap->valid);
    Com_Printf("snapFlags=0x%x\n", snap->snapFlags);
    Com_Printf("serverTime=0x%x\n", snap->serverTime);
    Com_Printf("messageNum=0x%x\n", snap->messageNum);
    Com_Printf("deltaNum=0x%x\n", snap->deltaNum);
    Com_Printf("ping=%d\n", snap->ping);
}

void Dump_PlayerState(playerState_t *ps) {
    Com_Printf("=== playerState_t ===\n");
    Com_Printf("pm_type=0x%x pm_flags=0x%x pm_time=0x%x\n", ps->pm_type, ps->pm_flags, ps->pm_time);
    Com_Printf("origin=[%.2f %.2f %.2f]\n", ps->origin[0], ps->origin[1], ps->origin[2]);
    Com_Printf("velocity=[%.2f %.2f %.2f]\n", ps->velocity[0], ps->velocity[1], ps->velocity[2]);
    Com_Printf("view=[%.2f %.2f %.2f]\n", ps->viewangles[0], ps->viewangles[1], ps->viewangles[2]);
    Com_Printf("speed=%d\n", ps->speed);
    Com_Printf("groundEntityNum=%d\n", ps->groundEntityNum);
    Com_Printf("weapon=%d\n", ps->weapon);
    Com_Printf("generic1=%d\n", ps->generic1);
}

void Dump_EntityState(entityState_t *e) {
     Com_Printf("number=%d eType=0x%x eFlags=0x%x source=%d target=%d generic1=%d clientNum=%d weapon=%d groundEntityNum=%d event=%d eventParm=%d otherEntityNum=%d otherEntityNum2=%d\n",
        e->number, e->eType, e->eFlags, e->otherEntityNum, e->otherEntityNum2, e->generic1, e->clientNum, e->weapon, e->groundEntityNum, (e->event & ~EV_EVENT_BITS), e->eventParm, e->otherEntityNum, e->otherEntityNum2);
}

void Dump_CreateOutputFolder(void) {
    struct stat st = {0};
    
    if (!opts.dumpAll) {
        return;
    }
    
    if (stat(opts.generalOutputFolder, &st) == -1) {
        if (mkdir(opts.generalOutputFolder, 0700) != 0) {
            Com_Error(ERR_FATAL, "Could not create %s: %s\n",
                opts.generalOutputFolder,
                strerror(errno)
            );
        }
    } else {
        Com_Error(ERR_FATAL, "Could not create %s: %s\n",
            opts.generalOutputFolder,
            strerror(errno)
        );
    }
}

void Dump_MapChanged(void) {
    if (!opts.dumpAll) {
        return;
    }
    
    Dump_CreateMapFolder();
    Dump_CreateMapFile();
    demo.mapCount++;
}

void Dump_CreateMapFolder(void) {
    sprintf(demo.mapFolderName, "%s/%d", opts.generalOutputFolder, demo.mapCount);
    mkdir(demo.mapFolderName, 0700);
}

void Dump_CreateMapFile(void) {
    FILE *mapFile;
    char mapFileName[MAX_STRING_CHARS];
    
    sprintf(mapFileName, "%s/%s", demo.mapFolderName, GD_MAP_FILE);
    mapFile = fopen(mapFileName, "w");
    fprintf(mapFile, "%s", demo.serverInfo.mapName);
    fclose(mapFile);
}
