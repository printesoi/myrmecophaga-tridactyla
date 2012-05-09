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

#include <vector>

#include "global.h"
#include "Logging.h"
#include "Timer.h"

/** Class for representing a square in the grid. */
class Square
{
    public:
        /** Coordinates of the square. */
        int x;
        int y;
        bool isVisible;
        bool isWater;
        bool isHill;
        bool isFood;
        bool isBorder;
        bool seen;
        int isMarked;
        int hillPlayer;
        int antPlayer;
        int myAntNumber;
        int exploreIndex;
        int foodIndex;
        /** Markers needed for A*. */
        int f,g,h,dir;
        /** Markes for danger status. */
        int dd,di;

        /** Links to neighbours. */
        std::vector<Square *> neigh;

        /** Direct dangered tiles. */
        std::vector<Square *> ddir;

        /** Indirect dangered tiles. */
        std::vector<Square *> dind;

        /** Constructor. */
        Square(int a, int b);

        /** Resets some information for the square. */
        void reset();
};

#endif
