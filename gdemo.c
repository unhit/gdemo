#include <unistd.h>

#include "global.h"
#include "opts.h"
#include "parser.h"
#include "stats.h"
#include "helper.h"
#include "dump.h"
#include "print.h"

extern options_t opts;

int main(int argc, char **argv) {
    GD_ParseOptions(argc, argv);
    GD_ResetClients();
    GD_OpenDemoFile();
    GD_ResetAllClients();
    GD_InitColoredWeapons();
    
    Dump_CreateOutputFolder();
    GD_OpenAngleFile();
    GD_OpenPositionFile();
    
    GD_ReadDemoHeader();
    
    GD_Print("Demo version: %s\n", demo.version);
    
    GD_InitHuffmanTree();

    demo.snapCount = 0;

    while (1) {
	if (!GD_ParseNextMessage()) {
	    break;
	}
	
	switch (demo.lastMessageType) {
	    case svc_gamestate:
		GD_GameStateParsed();
		Dump_MapChanged();
		GD_ShowServerInfo();
		clc.lastExecutedServerCommand = clc.serverCommandSequence;
		break;
	    case svc_snapshot:
		GD_SnapshotParsed();
		demo.snapCount++;
		break;
	    default:
		Com_Error(ERR_DROP, "Unsupported command type: %d\n", demo.lastMessageType);
	}
    }
    
    fclose(demo.demofile);

    if (opts.dumpAngles) {
        fclose(demo.anglefile);
    }
    
    if (opts.dumpPosition) {
        fclose(demo.positionfile);
    }
    
    int i;
    
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (dcs[i].valid) {
            GD_ShowIndividualStats(i);
        }
    }
    
    GD_FreeColoredWeapons();
    return 0;
}

void GD_ResetClients(void) {
    int i;
    
    for (i = 0; i < MAX_CLIENTS; i++) {
        dcs[i].valid = qfalse;
    }
}

void GD_OpenDemoFile(void) {
    if (!opts.demoFileName[0] || !(demo.demofile = fopen(opts.demoFileName, "rb"))) {
	Com_Error(ERR_FATAL, "Couldn't open demofile: %s", opts.demoFileName);
	_exit(-1);
    }
}

void GD_OpenAngleFile(void) {
    if (opts.dumpAngles) {
        if (!opts.angleFileName[0] || !(demo.anglefile = fopen(opts.angleFileName, "w"))) {
            Com_Error(ERR_FATAL, "Couldn't open angle file: %s", opts.angleFileName);
            _exit(-1);
        }
        
        fprintf(demo.anglefile, "pitch,yaw\n");
    }
}

void GD_OpenPositionFile(void) {
    if (opts.dumpPosition) {
        if (!opts.positionFileName[0] || !(demo.positionfile = fopen(opts.positionFileName, "w"))) {
            Com_Error(ERR_FATAL, "Couldn't open position file: %s", opts.positionFileName);
            _exit(-1);
        }
    }
}

void GD_InitHuffmanTree(void) {
    Huff_Init(&huff);
}
