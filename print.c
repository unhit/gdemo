#include <stdio.h>
#include <time.h>
#include <string.h>

#include "print.h"
#include "helper.h"
#include "opts.h"
extern options_t opts;

void GD_Print(const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stdout, fmt, argp);
    va_end(argp);
}

void GD_PrintTimestamp(void) {
    if (opts.showTimestamps) {
        int mapMins = GD_GetMapTime() / 60;
        int mapSecs = GD_GetMapTime() % 60;
        
        if (mapMins >= 0 && mapSecs >= 0) {
            fprintf(stdout, "[%02d:%02d]", mapMins, mapSecs);
        } else {
            fprintf(stdout, "[  :  ]");
        }
        
        if (GD_IsRoundGametype()) {
            int roundMins = GD_GetRoundTime() / 60;
            int roundSecs = GD_GetRoundTime() % 60;
            
            if (roundMins >= 0 && roundSecs >= 0) {
                fprintf(stdout, "[%02d:%02d]", roundMins, roundSecs);
            } else {
                fprintf(stdout, "[  :  ]");
            }
        }
        
        fprintf(stdout, " ");
    }
}
