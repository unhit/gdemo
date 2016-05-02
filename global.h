#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define NAME		"GDemo"
#define VERSION		"4.2.023"
#define AUTHOR          "fruk"
#define CONTACT         "#<gd> @ qnet"

#include "code/qcommon/q_shared.h"
#include "code/client/client.h"

#define C_BLACK                 "\033[22;30m"
#define C_RED                   "\033[22;31m"
#define C_GREEN                 "\033[22;32m"
#define C_BROWN                 "\033[22;33m"
#define C_BLUE                  "\033[22;34m"
#define C_MAGENTA               "\033[22;35m"
#define C_CYAN                  "\033[22;36m"
#define C_GRAY                  "\033[22;37m"
#define C_DARKGRAY              "\033[01;30m"
#define C_LIGHTRED              "\033[01;31m"
#define C_LIGHTGREEN            "\033[01;32m"
#define C_YELLOW                "\033[01;33m"
#define C_LIGHTBLUE             "\033[01;34m"
#define C_LIGHTMAGENTA          "\033[01;35m"
#define C_LIGHTCYAN             "\033[01;36m"
#define C_WHITE                 "\033[01;37m"

#define C_BG_BLACK              "\033[22;40m"
#define C_BG_RED                "\033[22;41m"
#define C_BG_GREEN              "\033[22;42m"
#define C_BG_YELLOW             "\033[22;43m"
#define C_BG_BLUE               "\033[22;44m"
#define C_BG_MAGENTA            "\033[22;45m"
#define C_BG_CYAN               "\033[22;46m"
#define C_BG_WHITE              "\033[22;47m"
#define C_BG_RESET              "\033[22;49m"

#define C_RESET                 "\033[0m"

#define COLORIZE(str, color)           (opts.coloredOutput ? color str C_RESET C_BG_RESET : str)

#define GD_FLAG_ATBASE          0
#define GD_FLAG_TAKEN           1
#define GD_FLAG_DROPPED         2

#define CS_ROUND                1001
#define CS_TIMEOUT              1004

#define GD_ROUND_START          0
#define GD_ROUND_END            1

#define GD_GAMETYPE_TS          4
#define GD_GAMETYPE_CTF         7
#define GD_GAMETYPE_BOMB        8

// for delta compression of snapshots
#define MAX_SNAPSHOTS           32
#define SNAPSHOT_MASK           (MAX_SNAPSHOTS-1)

// for keeping reliable text commands not acknowledged by receiver yet
#define MAX_SERVERCMDS          64
#define SERVERCMD_MASK          (MAX_SERVERCMDS-1)

// for keeping all entityStates for delta encoding
//#define MAX_PARSE_ENTITIES      (MAX_GENTITIES*2)
#define PARSE_ENTITIES_MASK     (MAX_PARSE_ENTITIES-1)

#define GD_MAX_WEAPONS          26

typedef struct gdServerInfo_s {
    char serverName[MAX_STRING_CHARS];
    char mapName[MAX_STRING_CHARS];
    char serverAdmin[MAX_STRING_CHARS];
    char serverEmail[MAX_STRING_CHARS];
    int auth;
    int timelimit;
    int startServerTime;
    float roundTime;
    int startRoundTime;
    int gametype;
} gdServerInfo_t;

typedef struct {
    FILE *demofile;
    FILE *anglefile;
    FILE *positionfile;

    char version[MAX_STRING_CHARS];
    
    int demoMessageSequence;
    int gameStatesParsed;
    int lastMessageType;
    int timeoutCalled;
    
    gdServerInfo_t serverInfo;
    
    int snapCount;
    
    int mapCount;
    char mapFolderName[MAX_STRING_CHARS];
} demoFileState_t;

typedef struct weaponStats_s {
    int hits;
    int missedHits;
    int damage;
    int kills;
    int deaths;
} weaponStats_t;

typedef struct demoClientState_s {
    qboolean valid;
    char name[MAX_STRING_CHARS];
    char coloredName[MAX_STRING_CHARS];
    int team;
    int weapon;
    weaponStats_t weaponStats[GD_MAX_WEAPONS];
    int serverTime;
} demoClientState_t;

typedef struct centity_s {
    entityState_t currentState;
    qboolean currentValid;
    int previousEvent;
} centity_t;

typedef struct demoFlag_s {
    int status;
    int carrier;
} demoFlag_t;

demoFileState_t demo;

clientActive_t cl;

clientConnection_t clc;

huffman_t huff;

demoClientState_t dcs[MAX_CLIENTS];

centity_t centities[MAX_GENTITIES];

demoFlag_t flagStatus[TEAM_NUM_TEAMS];

#define EPS             0.00001

void GD_ResetClients(void);
void GD_OpenDemoFile(void);
void GD_OpenAngleFile(void);
void GD_OpenPositionFile(void);
void GD_InitHuffmanTree(void);

#endif
