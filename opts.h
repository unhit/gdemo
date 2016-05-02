#ifndef _OPTS_H_
#define _OPTS_H_

typedef struct options_s {
    int dontStripColors;
    int showGameState;
    int showStats;
    int showDetailedStats;
    int showHits;
    int showMissedHits;
    int showKills;
    int showChat;
    int showTeamChat;
    int showWeaponChanges;
    int showRadioCalls;
    int showFlagEvents;
    int showAll;
    
    int coloredOutput;
    
    int dumpAngles;
    int dropZeroAngles;
    
    int dumpPosition;
    int dropZeroPositions;
    
    int dumpAll;
    
    int showTimestamps;
    
    char demoFileName[256];
    char angleFileName[256];
    char positionFileName[256];
    char generalOutputFolder[256];
} options_t;

void GD_ParseOptions(int, char **);
void GD_Help(const char *);

#endif
