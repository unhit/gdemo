#include <unistd.h>

#include "variable.h"
#include "global.h"
#include "parser.h"
#include "opts.h"
#include "helper.h"
#include "dump.h"
#include "stats.h"

extern options_t opts;

void GD_GameStateParsed(void) {
    int i;
    char *configString;
    const char *serverInfo;
    
    for (i = 0; i < MAX_CLIENTS; i++) {
        configString = GD_GetConfigString(&cl.gameState, CS_PLAYERS + i);
        GD_UpdateClientData(i, configString);
    }
    
    serverInfo = cl.gameState.stringData + cl.gameState.stringOffsets[ CS_SERVERINFO ];
    
    Q_strncpyz(demo.serverInfo.serverName, Info_ValueForKey(serverInfo, "sv_hostname"), MAX_STRING_CHARS-1);
    demo.serverInfo.auth = atoi(Info_ValueForKey(serverInfo, "auth"));
    
    if (Q_stricmp(demo.serverInfo.mapName, Info_ValueForKey(serverInfo, "mapname")) && demo.serverInfo.mapName[0]) {
        Q_strncpyz(demo.serverInfo.mapName, Info_ValueForKey(serverInfo, "mapname"), MAX_STRING_CHARS-1);
        GD_MapChanged();
    }
    
    Q_strncpyz(demo.serverInfo.mapName, Info_ValueForKey(serverInfo, "mapname"), MAX_STRING_CHARS-1);
    Q_strncpyz(demo.serverInfo.serverAdmin, Info_ValueForKey(serverInfo, " Admin"), MAX_STRING_CHARS-1);
    Q_strncpyz(demo.serverInfo.serverEmail, Info_ValueForKey(serverInfo, " Email"), MAX_STRING_CHARS-1);
    demo.serverInfo.timelimit = atoi(Info_ValueForKey(serverInfo, "timelimit"));
    demo.serverInfo.roundTime = atof(Info_ValueForKey(serverInfo, "g_roundtime"));
    demo.serverInfo.gametype = atoi(Info_ValueForKey(serverInfo, "g_gametype"));

    demo.serverInfo.startServerTime = cl.snap.serverTime;
    demo.serverInfo.startRoundTime = cl.snap.serverTime;
    
    demo.timeoutCalled = 0;
}

void GD_SnapshotParsed(void) {
    int i, clientNum, radio1, radio2, csNum;
    char *s, buffer[MAX_STRING_CHARS];
    entityState_t *es;

    for (i = clc.lastExecutedServerCommand + 1; i <= clc.serverCommandSequence; i++) {
        s = clc.serverCommands[i & (MAX_RELIABLE_COMMANDS - 1)];
        Q_strncpyz(buffer, s, sizeof(buffer));

        if (!opts.dontStripColors) {
            GD_StripColors(buffer, strlen(buffer));
        }

        Cmd_TokenizeString(buffer);
#ifdef DEBUG
        Com_Printf("%s %s\n", Cmd_Argv(0), Cmd_Args());
#endif
        // parse config strings
        if (!Q_stricmp(Cmd_Argv(0), "cs")) {
            csNum = atoi(Cmd_Argv(1));
            
            if (csNum >= CS_PLAYERS && csNum < CS_PLAYERS + MAX_CLIENTS) {
                GD_UpdateClientData(csNum - CS_PLAYERS, Cmd_Argv(2));
            } else if (csNum == CS_FLAGSTATUS) {
                GD_ParseFlagStatus(Cmd_Argv(2));
            } else if (csNum == CS_LEVEL_START_TIME) {
                GD_ParseLevelStartTime(Cmd_Argv(2));
            } else if (csNum == CS_WARMUP) {
                GD_ParseWarmup(Cmd_Argv(2));
            } else if (csNum == CS_ROUND) {
                GD_ParseRound(Cmd_Argv(2));
            } else if (csNum == CS_TIMEOUT) {
                GD_ParseTimeout();
            }
        // parse global chat
        } else if (!Q_stricmp(Cmd_Argv(0), "cchat")) {
            clientNum = atoi(Cmd_Argv(1));
            GD_ShowChat(clientNum, Cmd_Argv(2));
        // parse team chat
        } else if (!Q_stricmp(Cmd_Argv(0), "tcchat")) {
            clientNum = atoi(Cmd_Argv(1));
            GD_ShowTeamChat(clientNum, Cmd_Argv(2));
        // parse radio calls
        } else if (!Q_stricmp(Cmd_Argv(0), "rsay")) {
            clientNum = atoi(Cmd_Argv(1));
            radio1 = atoi(Cmd_Argv(2));
            radio2 = atoi(Cmd_Argv(3));
            GD_ShowRadioCall(clientNum, radio1, radio2, Cmd_Argv(4));
        // parse powerups (flag carriers?)
        } else if (!Q_stricmp(Cmd_Argv(0), "followpowerup")) {
            GD_ParseFollowPowerup(atoi(Cmd_Argv(1)));
        }
    }

    clc.lastExecutedServerCommand = clc.serverCommandSequence;

    if (!cl.snap.valid) {
        return;
    }

    for (i = 0; i < MAX_GENTITIES; i++) {
        centities[i].currentValid = qfalse;
    }

    for (i = 0; i < cl.snap.numEntities; i++) {
        es = &cl.parseEntities[(cl.snap.parseEntitiesNum + i) & (MAX_PARSE_ENTITIES-1)];
        memcpy(&centities[es->number].currentState, es, sizeof(entityState_t));
        centities[es->number].currentValid = qtrue;
        GD_ParseEntity(&centities[es->number]);
    }

    for (i = 0; i < MAX_GENTITIES; i++) {
        if (!centities[i].currentValid) {
            centities[i].previousEvent = 0;
        }
    }
    
    GD_ShowViewAngles();
    GD_ShowPosition();
}

void GD_UpdateClientData(int index, char *info) {
    char *name;
    
    if (info && info[0]) {
        name = Info_ValueForKey(info, "n");
        
        if (Q_stricmp(dcs[index].name, name) && dcs[index].name[0]) {
            GD_ShowNameChange(index, name);
        }
        
        Q_strncpyz(dcs[index].name, name, sizeof(dcs[0].name));
            
        dcs[index].team = atoi(Info_ValueForKey(info, "t"));
        GD_ColorName(index);
        
        GD_ShowInfo(index);
        
        if (!dcs[index].valid) {
            dcs[index].serverTime = cl.snap.serverTime;
        }
        
        dcs[index].valid = qtrue;
    } else {
        if (dcs[index].valid) {
            GD_ShowIndividualStats(index);
            GD_ShowDisconnect(index);
            GD_ResetClientData(index);
        }
    }
}

char * GD_GetConfigString(gameState_t *gs, int index) {
    if (index < 0 || index >= MAX_CONFIGSTRINGS) {
        Com_Printf("CG_ConfigString: bad index: %i", index);
    }

    return gs->stringData + gs->stringOffsets[index];
}

void GD_ParseEntity(centity_t *cent) {
    int source, target, event, weapon;
    entityState_t *es = &cent->currentState;

    if (es->eType > ET_EVENTS) {
        if (cent->previousEvent) {
            return;
        }

        cent->previousEvent = 1;
        es->event = es->eType - ET_EVENTS;
    } else {
        if (es->event == cent->previousEvent) {
            return;
        }

        cent->previousEvent = es->event;

        if ((es->event & ~EV_EVENT_BITS) == 0) {
            return;
        }
    }

    event = es->event & ~EV_EVENT_BITS;

#ifdef DEBUG
    Dump_EntityState(es);
#endif

    source = es->otherEntityNum;
    target = es->otherEntityNum2;

    if (source >= MAX_CLIENTS || target >= MAX_CLIENTS) {
	return; // show entity (glass?) kill?
    }

    GD_CheckWeaponChange(es->eType == GD_ET_0x01 ? es->clientNum : source, es->weapon);

    switch (es->eType) {
        case GD_ET_HIT:
            weapon = dcs[source].weapon;
        
            GD_ShowHit(source, target, es->generic1, weapon);
        
            dcs[source].weaponStats[weapon].hits++;
            dcs[source].weaponStats[weapon].damage += es->generic1;
            break;
        case GD_ET_MISS_HIT:
            weapon = dcs[source].weapon;
        
            GD_ShowMissedHit(source, weapon);

            dcs[source].weaponStats[weapon].missedHits++;
            break;
            
        case GD_ET_KILL:
            weapon = dcs[target].weapon;
        
            GD_ShowKill(source, target, weapon);
        
            dcs[source].weaponStats[weapon].deaths++;
            dcs[target].weaponStats[weapon].kills++;
            break;
    }
}

void GD_CheckWeaponChange(int clientNum, int weapon) {
    if (weapon > 0) {
        if (weapon != dcs[clientNum].weapon) {
            GD_ShowWeaponChange(clientNum, weapon);
        }

        dcs[clientNum].weapon = weapon;
    }
}

void GD_ParseFlagStatus(char *status) {
    int redFlag = status[0] - '0';
    int blueFlag = status[1] - '0';
    demoFlag_t *fs = NULL;
    int *teamFlag;
    int team;
    
    if (flagStatus[TEAM_RED].status != redFlag) {
        fs = &flagStatus[TEAM_RED];
        teamFlag = &redFlag;
        team = TEAM_RED;
    } else if (flagStatus[TEAM_BLUE].status != blueFlag) {
        fs = &flagStatus[TEAM_BLUE];
        teamFlag = &blueFlag;
        team = TEAM_BLUE;
    }
    
    if (fs == NULL) {
        flagStatus[TEAM_RED].status = redFlag;
        flagStatus[TEAM_BLUE].status = blueFlag;
        return;
    }
    
    switch (*teamFlag) {
        // flag is taken, just save the status, we will extract carrier from powerupfollow
        case GD_FLAG_TAKEN:
            fs->status = GD_FLAG_TAKEN;
            break;
            
        // flag is dropped, we should already know the carrier
        case GD_FLAG_DROPPED:
            GD_ShowFlagDropped(fs->carrier, team);
            fs->status = GD_FLAG_DROPPED;
            break;
            
        // flag is now at base
        case GD_FLAG_ATBASE:
            switch (fs->status) {
                // if it was taken before, it must have been capture
                case GD_FLAG_TAKEN:
                    GD_ShowFlagCaptured(fs->carrier, team);
                    break;
                // if it was dropped, the flag is returned
                case GD_FLAG_DROPPED:
                    GD_ShowFlagReturned(team);
                    break;
            }
            
            fs->status = GD_FLAG_ATBASE;
            break;
    }
    
    flagStatus[TEAM_RED].status = redFlag;
    flagStatus[TEAM_BLUE].status = blueFlag;
    
}

void GD_ParseFollowPowerup(int clientNum) {
    if (flagStatus[TEAM_RED].status == GD_FLAG_TAKEN && GD_GetTeam(clientNum) != TEAM_RED) {
        flagStatus[TEAM_RED].carrier = clientNum;
        GD_ShowFlagTaken(clientNum, TEAM_RED);
    } else if (flagStatus[TEAM_BLUE].status == GD_FLAG_TAKEN && GD_GetTeam(clientNum) != TEAM_BLUE) {
        flagStatus[TEAM_BLUE].carrier = clientNum;
        GD_ShowFlagTaken(clientNum, TEAM_BLUE);
    }
}

void GD_ParseLevelStartTime(char *time) {
    int tm = atoi(time);
    
    demo.serverInfo.startServerTime = tm;
    demo.serverInfo.startRoundTime = tm;
    GD_ShowLevelStart();
}

void GD_ParseWarmup(char *time) {
    int tm = atoi(time);
    int diff = tm - cl.snap.serverTime; // msec
    GD_ShowWarmup(diff);
}

void GD_ParseRound(char *msg) {
    if (Q_stricmp(msg, "") == 0) {
        demo.serverInfo.startRoundTime = cl.snap.serverTime;
        GD_ShowRound(GD_ROUND_START, "");
    } else {
        GD_ShowRound(GD_ROUND_END, msg);
    }
}

void GD_ParseTimeout(void) {
    //demo.timeoutCalled = 1 - demo.timeoutCalled;
    GD_ShowTimeout();
}

void GD_ReadDemoHeader(void) {
    int r, len;
    char *version;

    r = fread(&len, 1, 4, demo.demofile);
    
    if (r != 4) {
        Com_Error(ERR_FATAL, "Invalid demo");
        _exit(-1);
    }
    
    // read game version
    len = LittleLong(len);
    version = malloc(len + 1);
    r = fread(version, 1, len, demo.demofile);
    version[len] = '\0';
    
    Q_strncpyz(demo.version, version, MAX_STRING_CHARS - 1);
    
    if (Q_strncmp(demo.version, GAME_VER, len)) {
	Com_Error(ERR_FATAL, "This utility supports demos from " GAME_VER " version only.");
	_exit(-1);
    }

    // we don't need this stuff..
    fread(&len, 1, 4, demo.demofile);
    fread(&len, 1, 4, demo.demofile);
    fread(&len, 1, 4, demo.demofile);
    
    free(version);
}

qboolean GD_ParseNextMessage(void) {
    msg_t       msg;
    byte        buffer[MAX_MSGLEN];
    int         len;
    int         seq;
    int         r;

    if (fread(&seq, 1, 4, demo.demofile) != 4) {
        Com_Error(ERR_FATAL, "Demo file was truncated.\n");
        return qfalse;
    }

    if (fread(&len, 1, 4, demo.demofile) != 4) {
        Com_Error(ERR_FATAL, "Demo file was truncated.\n");
        return qfalse;
    }

    if (seq == -1 || len == -1) {
        return qfalse;
    }

    MSG_Init(&msg, buffer, sizeof(buffer));

    demo.demoMessageSequence = LittleLong(seq);
    msg.cursize = LittleLong(len);
    if (msg.cursize <= 0 || msg.cursize >= msg.maxsize) {
        Com_Error(ERR_DROP, "Illegal demo message length.");
    }

    if (fread(msg.data, 1, msg.cursize, demo.demofile) != msg.cursize) {
        Com_Printf("Demo file was truncated.\n");
        return qfalse;
    }

    // read length backwards - 4.2.x only
    r = fread(&len, 1, 4, demo.demofile);
    CL_ParseServerMessage(&msg);
    return qtrue;
}

void GD_MapChanged(void) {
    int i;

    for (i = 0; i < MAX_CLIENTS; i++) {
        if (dcs[i].valid) {
            GD_ShowIndividualStats(i);
        }
    }
    
    GD_ResetAllClients();
}
