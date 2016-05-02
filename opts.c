#include <stdio.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>

#include "opts.h"
#include "global.h"
#include "print.h"
#include "variable.h"

options_t opts;

void GD_ParseOptions(int argc, char **argv) {
    int c;
    
    if (argc < 2) {
	GD_Help(argv[0]);
    }
    
    while ((c = getopt(argc, argv, "i:tgsdhHkcCwrfxv:Vp:PaA:T")) != -1) {
	switch (c) {
	    case 'i':
	        Q_strncpyz(opts.demoFileName, optarg, 255);
	        break;
	    case 't':
		opts.dontStripColors = 1;
		break;
	    case 'g':
		opts.showGameState = 1;
		break;
	    case 's':
	        opts.showStats = 1;
	        break;
	    case 'd':
	        opts.showDetailedStats = 1;
	        break;
	    case 'h':
	        opts.showHits = 1;
	        break;
	    case 'H':
	        opts.showMissedHits = 1;
	        break;
	    case 'k':
	        opts.showKills = 1;
	        break;
	    case 'c':
	        opts.showChat = 1;
	        break;
	    case 'C':
	        opts.showTeamChat = 1;
	        break;
	    case 'w':
	        opts.showWeaponChanges = 1;
	        break;
	    case 'r':
	        opts.showRadioCalls = 1;
	        break;
	    case 'f':
	        opts.showFlagEvents = 1;
	        break;
	    case 'x':
	        opts.coloredOutput = 1;
	        break;
	    case 'v':
	        opts.dumpAngles = 1;
	        Q_strncpyz(opts.angleFileName, optarg, 255);
	        break;
	    case 'V':
	        opts.dropZeroAngles = 1;
	        break;
	    case 'p':
	        opts.dumpPosition = 1;
	        Q_strncpyz(opts.positionFileName, optarg, 255);
	        break;
	    case 'P':
	        opts.dropZeroPositions = 1;
	        break;
	    case 'a':
	        opts.showAll = 1;
	        break;
	    case 'A':
	        opts.dumpAll = 1;
	        Q_strncpyz(opts.generalOutputFolder, optarg, 255);
	        break;
	    case 'T':
	        opts.showTimestamps = 1;
	        break;
	}
    }
}

void GD_Help(const char *str) {
    GD_Print(NAME " " VERSION " - Urban Terror " GAME_VER " demo parser\n");
    GD_Print("Author: " AUTHOR " - " CONTACT "\n");
    GD_Print("Usage: %s [-tgsdhHkcCwrfxVPaT] [-v <anglefile>] [-p <positionfile>] [-A <outputFolder>] -i <demofile>\n", str);
    GD_Print(
        "\t-i\tspecify input demo file name\n"
        "\t-v\tspecify output (CSV) angles file name\n"
        "\t-p\tspecify output (CSV) position file name\n"
	"\t-t\tdont strip colors and special characters\n"
	"\t-g\tshow gamestate\n"
	"\t-s\tshow stats\n"
	"\t-d\tshow detailed stats\n"
	"\t-h\tshow hits\n"
	"\t-H\tshow missed hits\n"
	"\t-k\tshow kills\n"
	"\t-c\tshow chat\n"
	"\t-C\tshow team chat\n"
	"\t-w\tshow weapon changes\n"
	"\t-r\tshow radio calls\n"
	"\t-f\tshow flag events\n"
	"\t-a\tshow everything\n"
	"\t-V\tdrop zero angles\n"
	"\t-P\tdrop zero positions\n"
	"\t-A\tdump all the stuff into files (not implemented yet)\n"
	"\t-x\tcolored output\n"
	"\t-T\tshow timestamps\n"
    );
    GD_Print("Example: %s -axT -i 2013_04_26_fruk.urtdemo\n", str);
    exit(-1);
}
