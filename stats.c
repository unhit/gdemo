#include "global.h"
#include "stats.h"
#include "opts.h"
#include "helper.h"
#include "print.h"

extern options_t opts;

void GD_ShowIndividualStats(int clientNum) {
    int i;
    int sumHits;
    int sumMissedHits;
    int sumDamage;
    int sumKills;
    int sumDeaths;
    double accuracy;
    double kd;
    weaponStats_t *ws;
    
    if (!opts.showStats && !opts.showAll) {
        return;
    }
    
    
    if (opts.showDetailedStats || opts.showAll) {
        for (i = 0; i < GD_MAX_WEAPONS; i++) {
            ws = &dcs[clientNum].weaponStats[i];
            
            if (ws->hits != 0 || ws->missedHits != 0 || ws->damage != 0 || ws->kills != 0 || ws->deaths != 0) {
                GD_ShowWeaponStats(clientNum, i);
            }
        }
    }
    
    sumHits = GD_GetHitsSum(clientNum);
    sumMissedHits = GD_GetMissedHitsSum(clientNum);
    sumDamage = GD_GetDamageSum(clientNum);
    sumKills = GD_GetKillsSum(clientNum);
    sumDeaths = GD_GetDeathsSum(clientNum);
    accuracy = GD_GetAccuracy(sumHits, sumMissedHits);
    kd = GD_KDRatio(sumKills, sumDeaths);
    
    GD_Print("%s", COLORIZE("stats:" C_BG_RESET " ", C_BG_GREEN C_BLACK));
    GD_Print("id=%d name=%s hits=%d missed=%d acc=%.2f%% damage=%d kills=%d deaths=%d kd=%.2f\n",
        clientNum,
        GD_GetName(clientNum),
        //(cl.snap.serverTime - dcs[clientNum].serverTime) / 1000,
        sumHits,
        sumMissedHits,
        accuracy,
        sumDamage,
        sumKills,
        sumDeaths,
        kd
    );
}

void GD_ShowWeaponStats(int clientNum, int weapon) {
    weaponStats_t *ws;
    double accuracy;
    double kd;
    
    ws = &dcs[clientNum].weaponStats[weapon];

    accuracy = GD_GetAccuracy(ws->hits, ws->missedHits);
    kd = GD_KDRatio(ws->kills, ws->deaths);
    
    GD_Print("%s", COLORIZE("wstats:" C_BG_RESET " ", C_BG_RED C_BLACK));
    GD_Print("cid=%d wid=%d name=%s hits=%d missed=%d acc=%.2f%% damage=%d kills=%d deaths=%d kd=%.2f\n",
        clientNum,
        weapon,
        GD_GetWeaponName(weapon),
        ws->hits,
        ws->missedHits,
        accuracy,
        ws->damage,
        ws->kills,
        ws->deaths,
        kd
    );
}

float GD_GetAccuracy(int hits, int missedHits) {
    float acc = 0.0;
    
    if (hits > 0 || missedHits > 0) {
        acc = ((float)hits / ((float)hits + missedHits)) * 100;
    }
    
    return acc;
}

float GD_KDRatio(int kills, int deaths) {
    float kd = kills;
    
    if (deaths > 0) {
        kd = ((float)kills / deaths);
    }
    
    return kd;
}

int GD_GetKillsSum(int clientNum) {
    int i, sum = 0;
    
    for (i = 0; i < GD_MAX_WEAPONS; i++) {
        sum += dcs[clientNum].weaponStats[i].kills;
    }
    
    return sum;
}

int GD_GetDeathsSum(int clientNum) {
    int i, sum = 0;
    
    for (i = 0; i < GD_MAX_WEAPONS; i++) {
        sum += dcs[clientNum].weaponStats[i].deaths;
    }
    
    return sum;
}

int GD_GetHitsSum(int clientNum) {
    int i, sum = 0;
    
    for (i = 0; i < GD_MAX_WEAPONS; i++) {
        sum += dcs[clientNum].weaponStats[i].hits;
    }
    
    return sum;
}

int GD_GetMissedHitsSum(int clientNum) {
    int i, sum = 0;
    
    for (i = 0; i < GD_MAX_WEAPONS; i++) {
        sum += dcs[clientNum].weaponStats[i].missedHits;
    }
    
    return sum;
}

int GD_GetDamageSum(int clientNum) {
    int i, sum = 0;
    
    for (i = 0; i < GD_MAX_WEAPONS; i++) {
        sum += dcs[clientNum].weaponStats[i].damage;
    }
    
    return sum;
}
