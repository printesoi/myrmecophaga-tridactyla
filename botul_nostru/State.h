/*
 * =============================================================================
 *
 *       Filename:  State.h
 *        Authors:  Dodon Victor <dodonvictor at gmail dot com>
 *                  Cernov Vladimir <gg.kaspersky@gmail.com>
 *                  Maldur Vitalie <maldur.vitalik@yahoo.com>
 *                  Straticiuc Vicu <straticiuc_vicu@yahoo.com>
 *        Created:  02/25/2012
 *    Description:
 *
 * =============================================================================
 */

/** Structure for storing information abour the current state of the map. */

#ifndef STATE_H_
#define STATE_H_

#include <string>
#include <cstdlib>
#include <stdint.h>
#include <list>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <functional>
#include <queue>

#include "Square.h"

class Compare
{
    public:
        bool operator() (const Square* a, const Square* b)
        {
            return (a->f > b->f);
        }
};

class State
{

    public:
        /** False while we keep playing. */
        bool gameOver;

        int currentTurnNumber;

        /** Score for each of the current players. */
        std::vector<double> scores;

        /** A matrix that represents the map. */
        std::vector<std::vector<Square> > grid;

        std::vector<Square *> myAntsNew;
        std::vector<Square *> enemyAntsNew;
        std::vector<Square *> myHillsNew;
        std::vector<Square *> enemyHillsNew;
        std::vector<Square *> foodNew;

        /** A vector that keeps the border tiles. */
        std::vector<Square *> borderTiles;

        /** This could have been global, but there you go... */
        Timer timer;

        /** Constructor creates the map proper. */
        State()
        {
            gameOver = false;
            currentTurnNumber = 0;
        }

        /** Clears non-persistent informatin from the grid after a step. */
        void reset();

        /** Marks visible cells. */
        void mark_visible();

        /** Marks the explored cells. */
        void mark_explored();

        /** Calculates the unexplored index. */
        int unexplored_index(Square *from);

        /** This is just Manhattan distance. */
        int manhattan (Square *loc1, Square *loc2);

        /** A* algorithm. */
        int Astar(Square *from, Square *to);

        /** Initialize the grid (the map variable). */
        void initGrid();

        /** Initialize the neighbours of the squares in the grid. */
        void initNeighbours();
};

/** Method that helps do the IO. */
std::istream& operator>>(std::istream &is, State &state);

#endif
