/*
 * =============================================================================
 *
 *       Filename:  Square.h
 *        Authors:  Dodon Victor <dodonvictor at gmail dot com>
 *                  Cernov Vladimir <gg.kaspersky@gmail.com>
 *                  Maldur Vitalie <maldur.vitalik@yahoo.com>
 *                  Straticiuc Vicu <straticiuc_vicu@yahoo.com>
 *        Created:  02/25/2012
 *    Description:
 *
 * =============================================================================
 */


// Local includes

#include "global.h"
#include "Logging.h"
#include "Timer.h"

/** Class for representing a square in the grid. */
class Square
{

public:
    Square();

    bool isVisible;
    bool isWater;
    bool isHill;
    bool isFood;
    bool seen;
    int isMarked;
    int hillPlayer;
    int antPlayer;
    int myAntNumber;
    int exploreIndex;
    int foodIndex;
	int f,g,h,dir;
    
	/** Resets the information for the square except water information. */
    void reset();
};
