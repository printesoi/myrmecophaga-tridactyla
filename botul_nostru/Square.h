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

#ifndef SQUARE_H_
#define SQUARE_H_

// Local includes

#include "global.h"
#include "Logging.h"
#include "Timer.h"

/** Class for representing a square in the grid. */
class Square
{

public:
	int x;
	int y;
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
	
	/** Constructor. */
	Square(int a, int b);

	/** Resets the information for the square except water information. */
    void reset();
};

#endif
