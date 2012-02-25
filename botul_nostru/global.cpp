/*
 * =============================================================================
 *
 *       Filename:  global.cpp
 *        Authors:  Dodon Victor <dodonvictor at gmail dot com>
 *                  Cernov Vladimir <kaspersky_gg@gmail.com>
 *                  Maldur Vitalie <maldur.vitalik@yahoo.com>
 *                  Straticiuc Vicu <straticiuc_vicu@yahoo.com>
 *        Created:  02/25/2012
 *    Description:  
 *
 * =============================================================================
 */

#ifndef GPARAM_H_
#define GPARAM_H_

#include <stdint.h>

#include <iostream>
#include <fstream>

#ifndef LOGFILENAME
#define LOGFILENAME "logFile.log"
#endif

namespace gparam {

int mapRows;
int mapColumns;

int numberPlayers;

int64_t seed;

int totalTurnsNumber;

double attackRadius;
double spawnRadius;
double viewRadius;

double loadTime;
double turnTime;

std::ofstream logFile(LOGFILENAME);

}

#endif

