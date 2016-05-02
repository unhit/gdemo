#include <math.h>

#include "helper.h"
#include "global.h"
#include "print.h"
#include "opts.h"

extern options_t opts;

char *weapons[] = {
    "(UNDEF)",
    "Knife",
    "Beretta",
    "DE",
    "SPAS",
    "MP5",
    "UMP",
    "HK69",
    "LR300",
    "G36",
    "PSG",
    "HE grenades",
    "Flash grenades",
    "Smoke grenades",
    "SR8",
    "AK103",
    "(UNDEF)",
    "Negev",
    "(UNDEF)",
    "M4",
    "Glock",
    "Colt",
    "Mac11",
    "(UNDEF)",
    "(UNDEF)",
    "Knife thrown"
};

char **coloredWeapons;

void GD_StripColors(char *name, int len) {
    char *ptr;
    int i;

    for (i = 0, ptr = name; *ptr && i < len; ) {
        if (*ptr != '^') {
            if (*ptr == 0x19) {
                ptr++;
            } else {
                name[i++] = *ptr++;
            }
        } else {
            ptr += 2;
        }
    }

    for ( ; i < len; i++) {
        name[i] = '\0';
    }
}

int GD_GetTeam(int clientNum) {
    return dcs[clientNum].team;
}

const char * GD_GetTeamName(int clientNum) {
    int team = dcs[clientNum].team;
    return GD_GetStaticTeamName(team);
}

const char * GD_GetStaticTeamName(int team) {
    switch (team) {
        case TEAM_FREE:
            return opts.coloredOutput ? C_WHITE "free" C_RESET : "free";
        case TEAM_RED:
            return opts.coloredOutput ? C_RED "red" C_RESET : "red";
        case TEAM_BLUE:
            return opts.coloredOutput ? C_BLUE "blue" C_RESET : "blue";
        case TEAM_SPECTATOR:
            return opts.coloredOutput ? C_YELLOW "spectator" C_RESET : "spectator";
        default:
            return opts.coloredOutput ? C_WHITE "(UNDEF)" C_RESET : "(UNDEF)";
    }
}

char * GD_GetName(int clientNum) {
    if (opts.coloredOutput) {
        return dcs[clientNum].coloredName;
    } else {
        return dcs[clientNum].name;
    }
}

const char * GD_GetWeaponName(int weapon) {
    if (opts.coloredOutput) {
        return coloredWeapons[weapon];
    } else {
        return weapons[weapon];
    }
}

void GD_ShowServerInfo(void) {
    if (opts.showGameState || opts.showAll) {
        //GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("serverinfo: ", C_MAGENTA));
        GD_Print("name=%s admin=%s email=%s auth=%s map=%s\n",
            demo.serverInfo.serverName,
            demo.serverInfo.serverAdmin,
            demo.serverInfo.serverEmail,
            demo.serverInfo.auth ? "yes" : "no",
            demo.serverInfo.mapName
        );
    }
}

void GD_ShowInfo(int clientNum) {
    if (opts.showGameState || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("info: ", C_GREEN));
        GD_Print("<%d> %s team=%s\n", clientNum, GD_GetName(clientNum), GD_GetTeamName(clientNum));
    }
}

void GD_ShowDisconnect(int clientNum) {
    if (opts.showGameState || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("disconnect: ", C_WHITE));
        GD_Print("<%d> %s\n", clientNum, GD_GetName(clientNum));
    }
}

void GD_ShowWeaponChange(int clientNum, int weapon) {
    if (opts.showWeaponChanges || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("weapon: ", C_CYAN));
        GD_Print("<%d> %s has changed weapon to <%d> %s\n",
            clientNum,
            GD_GetName(clientNum),
            weapon,
            GD_GetWeaponName(weapon)
        );
    }
}

void GD_ShowHit(int source, int target, int damage, int weapon) {
    if (opts.showHits || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("hit: ", C_LIGHTGREEN));
        GD_Print("<%d> %s hit <%d> %s for %d damage with <%d> %s\n",
            source,
            GD_GetName(source),
            target,
            GD_GetName(target),
            damage,
            weapon,
            GD_GetWeaponName(weapon)
        );
    }
}

void GD_ShowMissedHit(int clientNum, int weapon) {
    if (opts.showMissedHits || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("miss: ", C_DARKGRAY));
        GD_Print("<%d> %s has missed a hit with <%d> %s\n",
            clientNum,
            GD_GetName(clientNum),
            weapon,
            GD_GetWeaponName(weapon)
        );
    }
}

void GD_ShowKill(int source, int target, int weapon) {
    if (opts.showKills || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("kill: ", C_LIGHTRED));
        GD_Print("<%d> %s was killed by <%d> %s with <%d> %s\n",
            source,
            GD_GetName(source),
            target,
            GD_GetName(target),
            weapon,
            GD_GetWeaponName(weapon)
        );
    }
}

void GD_ShowChat(int clientNum, char *arg) {
    if (opts.showChat || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("cchat: ", C_LIGHTMAGENTA));
        GD_Print("<%d> %s\n",
            clientNum,
            arg
        );
    }
}

void GD_ShowTeamChat(int clientNum, char *arg) {
    if (opts.showChat || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("tcchat: ", C_BROWN));
        GD_Print("<%d> %s\n",
            clientNum,
            arg
        );
    }
}

void GD_ShowRadioCall(int clientNum, int radio1, int radio2, char *arg) {
    if (opts.showRadioCalls || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("radio: ", C_LIGHTBLUE));
        GD_Print("<%d> %s <%d, %d> %s\n",
            clientNum,
            GD_GetName(clientNum),
            radio1,
            radio2,
            arg
        );
    }
}

void GD_ShowFlagTaken(int clientNum, int flag) {
    if (opts.showFlagEvents || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("flag:" C_BG_RESET " ", C_BLACK C_BG_YELLOW));
        GD_Print("<%d> %s has taken the %s flag\n",
            clientNum,
            GD_GetName(clientNum),
            GD_GetStaticTeamName(flag)
        );
    }
}

void GD_ShowFlagDropped(int clientNum, int flag) {
    if (opts.showFlagEvents || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("flag:" C_BG_RESET " ", C_BLACK C_BG_YELLOW));
        GD_Print("<%d> %s has dropped the %s flag\n",
            clientNum,
            GD_GetName(clientNum),
            GD_GetStaticTeamName(flag)
        );
    }
}

void GD_ShowFlagCaptured(int clientNum, int flag) {
    if (opts.showFlagEvents || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("flag:" C_BG_RESET " ", C_BLACK C_BG_YELLOW));
        GD_Print("<%d> %s has captured the %s flag\n",
            clientNum,
            GD_GetName(clientNum),
            GD_GetStaticTeamName(flag)
        );
    }
}

void GD_ShowFlagReturned(int flag) {
    if (opts.showFlagEvents || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("flag:" C_BG_RESET " ", C_BLACK C_BG_YELLOW));
        GD_Print("The %s flag has returned\n",
            GD_GetStaticTeamName(flag)
        );
    }
}

void GD_ShowNameChange(int clientNum, char *name) {
    if (opts.showGameState || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("name:" C_BG_RESET " ", C_BLACK C_BG_WHITE));
        GD_Print("<%d> %s has changed name to %s\n",
            clientNum,
            GD_GetName(clientNum),
            name
        );
    }
}

void GD_ShowWarmup(int timeDiff) {
    if (opts.showGameState || opts.showAll) {
        //GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("warmup:" C_BG_RESET " ", C_YELLOW C_BG_BLUE));
        GD_Print("secs=%d\n",
            timeDiff / 1000
        );
    }
}

void GD_ShowLevelStart(void) {
    if ((opts.showGameState || opts.showAll) && !demo.timeoutCalled) {
        //GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("start:" C_BG_RESET " ", C_WHITE C_BG_RED));
        GD_Print("map=%s\n",
            demo.serverInfo.mapName
        );
    }
}

void GD_ShowRound(int status, char *msg) {
    if (opts.showGameState || opts.showAll) {
        //GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("round:" C_BG_RESET " ", C_BLACK C_BG_RED));
        GD_Print("status=%s msg=%s\n",
            status == GD_ROUND_START ? "start" : "end",
            msg
        );
    }
}

void GD_ShowTimeout(void) {
    if (opts.showGameState || opts.showAll) {
        GD_PrintTimestamp();
        GD_Print("%s", COLORIZE("timeout:" C_BG_RESET " ", C_WHITE C_BG_CYAN));
        GD_Print("\n");
    }
}

void GD_ResetAllClients(void) {
    int i;
    
    for (i = 0; i < MAX_CLIENTS; i++) {
        GD_ResetClientData(i);
    }
}

void GD_ResetClientData(int clientNum) {
    int i;
    
    dcs[clientNum].valid = qfalse;
    dcs[clientNum].name[0] = '\0';
    dcs[clientNum].team = TEAM_FREE;
    dcs[clientNum].weapon = 0;
    
    for (i = 0; i < GD_MAX_WEAPONS; i++) {
        GD_ResetWeaponStats(&dcs[clientNum].weaponStats[i]);
    }
}

void GD_ResetWeaponStats(weaponStats_t *ws) {
    ws->hits = 0;
    ws->missedHits = 0;
    ws->damage = 0;
    ws->kills = 0;
    ws->deaths = 0;
}

void GD_ColorName(int clientNum) {
    switch (GD_GetTeam(clientNum)) {
        case TEAM_RED:
            sprintf(dcs[clientNum].coloredName, C_RED "%s" C_RESET, dcs[clientNum].name);
            break;
        case TEAM_BLUE:
            sprintf(dcs[clientNum].coloredName, C_BLUE "%s" C_RESET, dcs[clientNum].name);
            break;
        case TEAM_SPECTATOR:
            sprintf(dcs[clientNum].coloredName, C_YELLOW "%s" C_RESET, dcs[clientNum].name);
            break;
        default:
            sprintf(dcs[clientNum].coloredName, C_WHITE "%s" C_RESET, dcs[clientNum].name);
            break;
    }
}

const char * GD_GetFlagStatusName(int status) {
    switch (status) {
        case GD_FLAG_TAKEN:
            return "FLAG_TAKEN";
        case GD_FLAG_ATBASE:
            return "FLAG_ATBASE";
        case GD_FLAG_DROPPED:
            return "FLAG_DROPPED";
        default:
            return "FLAG_UNKNOWN";
    }
}

void GD_ShowViewAngles(void) {
    vec3_t v;
    VectorCopy(cl.snap.ps.viewangles, v);
    
    if (opts.dumpAngles) {
        if (opts.dropZeroAngles && GD_IsZero(v[0]) && GD_IsZero(v[1])) {
            return;
        }
        
        fprintf(demo.anglefile, "%.4f,%.4f\n",
            v[0],
            fabs(v[1])
        );
    }
}

void GD_ShowPosition(void) {
    vec3_t v;
    VectorCopy(cl.snap.ps.origin, v);
    
    if (opts.dumpPosition) {
        if (opts.dropZeroPositions && GD_IsZero(v[0]) && GD_IsZero(v[1]) && GD_IsZero(v[2])) {
            return;
        }
        
        fprintf(demo.positionfile, "%.4f,%.4f,%.4f\n",
            v[0],
            v[1],
            v[2]
        );
    }
}

void GD_InitColoredWeapons(void) {
    int i;
    
    coloredWeapons = (char **)malloc(sizeof(char **) * GD_MAX_WEAPONS);
    
    for (i = 0; i < GD_MAX_WEAPONS; i++) {
        coloredWeapons[i] = (char *)malloc(MAX_STRING_CHARS);
        sprintf(coloredWeapons[i], C_WHITE "%s" C_RESET, weapons[i]);
    }
}

void GD_FreeColoredWeapons(void) {
    int i;
    
    for (i = 0; i < GD_MAX_WEAPONS; i++) {
        free(coloredWeapons[i]);
    }
    
    free(coloredWeapons);
}

int GD_IsZero(float val) {
    return (fabs(val - (int)val) < EPS);
}

int GD_GetMapTime(void) {
    return demo.serverInfo.timelimit*60 - (cl.snap.serverTime - demo.serverInfo.startServerTime)/1000;
}

int GD_GetRoundTime(void) {
    return demo.serverInfo.roundTime*60 - (cl.snap.serverTime - demo.serverInfo.startRoundTime)/1000;
}

int GD_IsRoundGametype(void) {
    switch (demo.serverInfo.gametype) {
        case GD_GAMETYPE_TS:
        case GD_GAMETYPE_BOMB:
            return qtrue;
        default:
            return qfalse;
    }
}
