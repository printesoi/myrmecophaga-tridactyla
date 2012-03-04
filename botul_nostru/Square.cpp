/*
 * =============================================================================
 *
 *       Filename:  Square.cpp
 *        Authors:  Dodon Victor <dodonvictor at gmail dot com>
 *                  Cernov Vladimir <gg.kaspersky@gmail.com>
 *                  Maldur Vitalie <maldur.vitalik@yahoo.com>
 *                  Straticiuc Vicu <straticiuc_vicu@yahoo.com>
 *        Created:  02/25/2012
 *    Description:  
 *
 * =============================================================================
 */

#include "Square.h"

/** Struct for representing a square in the grid. */
Square::Square() : isVisible(false), isWater(false), isHill(false), isFood(false)
{
    hillPlayer = antPlayer = myAntNumber = isMarked = foodIndex = expandIndex = -1;
	exploreIndex = 500;
	curiosity = 0;
}

    /** Resets the information for the square except water information. */
void Square::reset()
{
    isVisible = isHill = isFood = false;
    hillPlayer = antPlayer = myAntNumber = expandIndex = -1;
	exploreIndex++;
}
