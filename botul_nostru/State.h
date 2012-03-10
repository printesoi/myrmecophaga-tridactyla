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

// Local includes

#include "Square.h"
#include "Location.h"

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
    State()
    {
		gameOver = false;
		currentTurnNumber = 0;
        for (int i = 0; i < MAXIMUM_MAP_SIZE; ++i)
		{
			grid.push_back(std::vector<Square>());
			for (int j = 0; j < MAXIMUM_MAP_SIZE; ++j)
	            grid[i].push_back(Square(i,j));
		}
    }

    /** Clears non-persistent informatin from the grid after a step. */
    void reset();

    /** Marks visible cells. */
    void mark_visible();
    
    /** Marks the explored cells. */
    void mark_explored();

    /** Calculates the unexplored index. */
    int unexplored_index(Location from);
    
    /** This is just square of Euclid distance. */
    double distance(const Location loc1, const Location loc2);

	/** This is just Manhattan distance. */
	int manhattan (const Location loc1, const Location loc2);
    
    /** Return the square that a location points to. */
    Square* square(const Location loc);

    /** A* algorithm. */
    int Astar(Location from, Location to);

    /** Marks the radius of sight. */
    void mark_seen(Location from);
};

/** Method that helps do the IO. */
std::istream& operator>>(std::istream &is, State &state);

#endif
