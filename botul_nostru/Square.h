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

using namespace std;

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
        /** Markers for danger status. */
        int dd,di;

        /** Links to neighbours. */
        vector<Square *> neigh;

        /** Direct dangered tiles. */
        vector<Square *> ddir;

        /** Indirect dangered tiles. */
        vector<Square *> dind;

        /** Constructor. */
        Square(int a, int b);

        bool operator== (Square y)
        {
            return (this->x == y.x && this->y == y.y);
        }

        /** Returns the square of euclidian distance relative to a square. */
        int dist(Square *y);

        /** Checks if the square is not dangered. */
        bool notDangered(const vector<Square *> &v);

        /** Checks if the square is indirectly dangered. */
        bool indirectDangered(const vector<Square *> &v);

        /** Checks if the square is directly dangered. */
        bool directDangered(const vector<Square *> &v);

        /** Resets some information for the square. */
        void reset();
};

#endif
