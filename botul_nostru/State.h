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

// STD includes

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdint.h>
#include <map>
#include <list>
#include <algorithm>

// Local includes

#include "global.h"
#include "Logging.h"
#include "Timer.h"
#include "Square.h"
#include "Location.h"

struct State
{
    /** False while we keep playing. */

    bool gameOver;
    
    int currentTurnNumber;

    /** Score for each of the current players. */
    std::vector<double> scores;

    /** See definition of Square for further details. */
    std::vector<std::vector<Square> > grid;

    std::vector<Location> myAnts;
    std::vector<Location> enemyAnts;
    std::vector<Location> myHills;
    std::vector<Location> enemyHills;
    std::vector<Location> food;

    /** This could have been global, but there you go... */
    Timer timer;

    /** Constructor creates the map proper. */
    State() : gameOver(false), currentTurnNumber(0)
    {
        for (int i = 0; i < MAXIMUM_MAP_SIZE; ++i)
        {
            grid.push_back(std::vector<Square>(MAXIMUM_MAP_SIZE, Square()));
        }
    }

    /** Clears non-persistent informatin from the grid after a step. */
    void reset();

    /** Marks visible cells. */
    void mark_visible();
    
    /** Marks the explored cells. */
    void mark_explored();

    /** A primitive "hunting" method. */
    void mark_enemy_hills();

    /** Calculates the unexplored index. */
    int unexplored_index(Location from);
    
    /** This is just square of Euclid distance. */
    double distance(const Location loc1, const Location loc2);
    
    /** Return the square that a location points to. */
    Square* square(const Location loc)
    {
        return &grid[loc.row][loc.col];
    }

    /** A* algorithm. */
    int Astar( Location from, Location to );

    /** Marks the radius of sight. */
    void mark_seen(Location from);
};

/** Method that helps do the IO. */
std::istream& operator>>(std::istream &is, State &state);


#endif

