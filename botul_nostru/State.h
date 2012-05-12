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

using namespace std;

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
        vector<double> scores;

        /** A matrix that represents the map. */
        vector<vector<Square> > grid;

        vector<Square *> myAntsNew;
        vector<Square *> enemyAntsNew;
        vector<Square *> myHillsNew;
        vector<Square *> enemyHillsNew;
        vector<Square *> foodNew;

        /** A vector that keeps the border tiles. */
        vector<Square *> borderTiles;

        /** List of combat ants. */
        vector<Square *> combatAnts;

        /** List of moves for combat ans. */
        vector<Square *> movedAnts;

        /** List of combat dependencies. */
        vector<pair<Square *,Square *> > combatLinks;

        /** Groups of own ants and enemy ants fighting. */
        vector<vector<Square *> > myGroups;
        vector<vector<Square *> > enemyGroups;

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

        /** Initialize vector with dangered tiles. */
        void initDanger();

        /** Marks direct dangered tiles by an enemy. */
        void mark_direct_dangered(Square *sq);

        /** Marks indirect dangered tiles by an enemy. */
        void mark_indirect_dangered(Square *sq);

        /** Create the list of combat ants. */
        void getCombatAnts();

        /** Split the ants to groups. */
        void splitCombatAnts();

        /** A group of own ants fighting. */
        pair<vector<Square *>,int> fightMy(vector<Square *> demut,vector<Square *> mut,int ind);

        /** A group of enemy ants fighting. */
        int fightEnemy(const vector<Square *> &my,int ind,vector<Square *> demut,vector<Square *> mut);

        /** Simulate a combat. */
        void combat();

        /** Returns the evaluation score for 2 sets of positions. */
        int evaluate(const vector<Square *> &my,const vector<Square *> &en);

        int distances(const vector<Square *> &my,const vector<Square *> &en);
};

/** Method that helps do the IO. */
istream& operator>>(istream &is, State &state);

#endif
