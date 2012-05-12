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
Square::Square(int a, int b)
{
    isVisible = isWater = isHill = isFood = seen = isBorder = false;
    hillPlayer = antPlayer = myAntNumber = isMarked = foodIndex = -1;
    f = g = h = dir = -1;
    dd = di = 0;
    exploreIndex = 500;
    x = a;
    y = b;
}

/** Resets the information for the square except water information. */
void Square::reset()
{
    isVisible = isHill = isFood = isBorder = false;
    hillPlayer = antPlayer = myAntNumber = -1;
    dd = di = 0;
    exploreIndex++;
}

/** Returns the square of euclidian distance relative to a square. */
int Square::dist(Square *y)
{
    /** Row difference. */
    int x1 = (this->x - y->x + gparam::mapRows) % gparam::mapRows;
    int x2 = (y->x - this->x + gparam::mapRows) % gparam::mapRows;
    if (x1 > x2)
        x1 = x2;

    /** Column difference. */
    int y1 = (this->y - y->y + gparam::mapCols) % gparam::mapCols;
    int y2 = (y->y - this->y + gparam::mapCols) % gparam::mapCols;
    if (y1 > y2)
        y1 = y2;

    return x1 * x1 + y1 * y1;
}

/** Checks if the square is not dangered. */
bool Square::notDangered(const vector<Square *> &v)
{
    for (unsigned i = 0; i < v.size(); i++)
        if (this->dist(v[i]) <= 17)
            return false;
    return true;
}

/** Checks if the square is indirectly dangered. */
bool Square::indirectDangered(const vector<Square *> &v)
{
    for (unsigned i = 0; i < v.size(); i++)
    {
        int dist = this->dist(v[i]);
        if (dist > 5 && dist <= 10)
            return false;
    }
    return true;
}

/** Checks if the square is directly dangered. */
bool Square::directDangered(const vector<Square *> &v)
{
    for (unsigned i = 0; i < v.size(); i++)
        if (this->dist(v[i]) <= 5)
            return false;
    return true;
}
