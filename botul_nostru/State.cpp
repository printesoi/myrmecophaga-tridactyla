/*
 * =============================================================================
 *
 *       Filename:  State.cpp
 *        Authors:  Dodon Victor <dodonvictor at gmail dot com>
 *                  Cernov Vladimir <gg.kaspersky@gmail.com>
 *                  Maldur Vitalie <maldur.vitalik@yahoo.com>
 *                  Straticiuc Vicu <straticiuc_vicu@yahoo.com>
 *        Created:  02/25/2012
 *    Description:  
 *
 * =============================================================================
 */

#include <cstdlib>
#include <iomanip>
#include <cmath>

#include "State.h"

/* Resets all non-water squares to land and clears the bots ant vector. */
void State::reset()
{
    myAnts.clear();
    myHills.clear();
    enemyAnts.clear();
    enemyHills.clear();
    food.clear();

    for(int row = 0; row < gparam::mapRows; row++)
        for(int col = 0; col < gparam::mapColumns; col++)
			grid[row][col].reset();
}

/* Returns the square of Euclid distance between two locations with the edges
 * wrapped. */
double State::distance(const Location loc1, const Location loc2)
{
    int d11 = loc1.row - loc2.row;
    int d1 = d11 < 0 ? -d11 : d11;
    int d22 = loc1.col - loc2.col;
    int d2 = d22 < 0 ? -d22 : d22;
    int dr = d1 < (gparam::mapRows - d1) ? d1 : (gparam::mapRows - d1);
    int dc = d2 < (gparam::mapColumns - d2) ? d2 : (gparam::mapColumns - d2);
    return dr*dr + dc*dc;
}

/* Marks which squares on the map are visible. */
void State::mark_visible()
{
	for (unsigned int ant = 0; ant < myAnts.size(); ++ant)
		mark_seen(myAnts[ant]);
}

/** Marks which squares are visible from an ant Location. */
void State::mark_seen(Location ant)
{
	Location to = ant.move(8,-3);
	for(int i = 0;i < 7; ++i)
	{
		to = to.move(1);
		grid[to.row][to.col].seen = true;
	}
	
	to = ant.move(7,-5);
	for(int i = 0;i < 11; ++i)
	{
		to = to.move(1);
		grid[to.row][to.col].seen = true;	
	}
	
	to = ant.move(6,-6);
	for(int i = 0;i < 13; ++i)
	{
		to = to.move(1);
		grid[to.row][to.col].seen = true;
	}
	
	to = ant.move(5,-7);
	for(int i = 0;i < 15; ++i)
	{
		to = to.move(1);
		grid[to.row][to.col].seen = true;
	}
	
	to = ant.move(4,-7);
	for(int i = 0; i < 15; ++i)
	{
		to = to.move(1);
		grid[to.row][to.col].seen = true;
	}
	
	for(int j = -3; j < 4; j++)
	{
		to = ant.move(j,-8);
		for(int i = 0;i < 17; ++i)
		{
			to = to.move(1);
			grid[to.row][to.col].seen = true;
		}
	}
	
	to = ant.move(-8,-3);
	for(int i = 0;i < 7; ++i)
	{
		to = to.move(1);
		grid[to.row][to.col].seen = true;
	}
	
	to = ant.move(-7,-5);
	for(int i = 0;i < 11; ++i)
	{
		to = to.move(1);
		grid[to.row][to.col].seen = true;	
	}
	
	to = ant.move(-6,-6);
	for(int i = 0;i < 13; ++i)
	{
		to = to.move(1);
		grid[to.row][to.col].seen = true;
	}
	
	to = ant.move(-5,-7);
	for(int i = 0;i < 15; ++i)
	{
		to = to.move(1);
		grid[to.row][to.col].seen = true;
	}
	
	to = ant.move(-4,-7);
	for(int i = 0;i < 15; ++i)
	{
		to = to.move(1);
		grid[to.row][to.col].seen = true;
	}
}

/** Resets the exploreIndex of the "visible" squares to 0. */
void State::mark_explored()
{
	std::list<Location> squares;
	std::list<Location> changed;
	
	Location x,y;
	Square *f,*t;
	for (unsigned int ant = 0; ant < myAnts.size(); ant++)
	{
		grid[myAnts[ant].row][myAnts[ant].col].isMarked = 0;
		changed.push_back(myAnts[ant]);
		squares.push_back(myAnts[ant]);
	}
	while (squares.size())
	{
		x = squares.front();
		f = &grid[x.row][x.col];
		squares.pop_front();
		
		if (f->isMarked == VIEW_RADIUS)
			break;

		for (int dir = 0; dir < 4; dir++)
		{
			y = x.move(dir);
			t = &grid[y.row][y.col];

			if (!t->isWater && t->isMarked == -1)
			{
				t->isMarked = f->isMarked + 1;
				changed.push_back(y);
				squares.push_back(y);
			}
		}
	}
	while (changed.size())
	{
		x = changed.front();
		f = &grid[x.row][x.col];
		f->isMarked = -1;
		f->exploreIndex = 0;
		changed.pop_front();
	}
	squares.clear();
}

/** A primitive "hunting" method. */
void State::mark_enemy_hills()
{
	if (enemyHills.size() == 0)
		return;

	std::list<Location> squares;
	std::list<Location> changed;
	
	Location x,y;
	Square *f,*t;
	for (unsigned int eHill = 0; eHill < enemyHills.size(); eHill++)
	{
		grid[enemyHills[eHill].row][enemyHills[eHill].col].isMarked = 0;
		changed.push_back(enemyHills[eHill]);
		squares.push_back(enemyHills[eHill]);
	}
	while (squares.size())
	{
		x = squares.front();
		f = &grid[x.row][x.col];
		squares.pop_front();
		
		if (f->isMarked == 2 * VIEW_RADIUS)
			break;

		for (int dir = 0; dir < 4; dir++)
		{
			y = x.move(dir);
			t = &grid[y.row][y.col];

			if (!t->isWater && t->isMarked == -1)
			{
				t->isMarked = f->isMarked + 1;
				changed.push_back(y);
				squares.push_back(y);
			}
		}
	}
	while (changed.size())
	{
		x = changed.front();
		f = &grid[x.row][x.col];
		f->exploreIndex = 1000 - f->isMarked;
		f->isMarked = -1;
		changed.pop_front();
	}
	squares.clear();
}

/** Calculates a sum which represents a fog indicator. */
int State::unexplored_index(Location from)
{
	std::list<Location> squares;
	std::list<Location> changed;
	
	Location x,y;
	Square *f,*t;

	int rez = 0;
	
	grid[from.row][from.col].isMarked = 0;
	changed.push_back(from);
	squares.push_back(from);
	
	while (squares.size())
	{
		x = squares.front();
		f = &grid[x.row][x.col];
		squares.pop_front();
		
		if (f->isMarked == VIEW_RADIUS)
		{
			rez = f->exploreIndex;
			break;
		}

		for (int dir = 0; dir < 4; dir++)
		{
			y = x.move(dir);
			t = &grid[y.row][y.col];

			if (!t->isWater && t->isMarked == -1)
			{
				t->isMarked = f->isMarked + 1;
				changed.push_back(y);
				squares.push_back(y);
			}
		}
	}

	while (squares.size())
	{
		x = squares.front();
		f = &grid[x.row][x.col];
		rez += f->exploreIndex;
		squares.pop_front();
	}

	while (changed.size())
	{
		x = changed.front();
		f = &grid[x.row][x.col];
		f->isMarked = -1;
		changed.pop_front();
	}

	return rez;
}

/* Comparator for sorting a list of locations */
bool comparator( Location one, Location two ){
	return (one.f < two.f);
}

/* A* algorithm. */
int State::Astar( Location from, Location to )
{
	Location tmp;
	std::list<Location> open;
	open.push_back(from);
	from.g = 0;
	from.dir = -1;
	grid[from.row][from.col].isMarked = 0;
	while(!open.empty())
	{
		open.sort(comparator);
		tmp = open.front();
		open.pop_front();
		
		if( tmp == to )
			break;

		for( int i=0; i<4; i++ )
		{
			Location aux = tmp.move(i);
			Square x = grid[aux.row][aux.col];
			
			if( x.isMarked == -1 && !x.isWater )
			{
				
				if(	tmp.dir == -1 ) 
					aux.dir = i;
				else
					aux.dir = tmp.dir;
				
				aux.g = 10 + tmp.g;
				aux.f = aux.g + 10 * (abs(aux.row - to.row) + abs(aux.col - to.col));
				open.push_back(aux);
			}
		}
	}
	return tmp.dir;
}

/* Input functions. */
std::istream& operator>>(std::istream &is, State &state)
{
    int row, col, player;
    std::string inputType, junk;

    /* Read in input type. */
    while(is >> inputType)
    {
        if(inputType == "end")
        {
            state.gameOver = true;
            break;
        }
        else if(inputType == "turn") 
        {
            is >> state.currentTurnNumber;
            break;
        } 
        else 
        {
            getline(is, junk);
        }
    }

    if(state.currentTurnNumber == 0)
    {
        /* If we are at the beginning of the game, read in the parameters. */
        while(is >> inputType)
        {
            if(inputType == "loadtime") 
            {
                is >> gparam::loadTime;
            } 
            else if(inputType == "turntime") 
            {
                is >> gparam::turnTime;
            } 
            else if(inputType == "rows") 
            {
                is >> gparam::mapRows;
            }
            else if(inputType == "cols") 
            {
                is >> gparam::mapColumns;
            } 
            else if(inputType == "turns")
            {
                is >> gparam::totalTurnsNumber;
            } 
            else if(inputType == "player_seed")
            {
                is >> gparam::seed;
                srand((unsigned int)gparam::seed);
            }
            else if(inputType == "viewradius2")
            {
                is >> gparam::viewRadius;
            }
            else if(inputType == "attackradius2")
            {
                is >> gparam::attackRadius;
            } 
            else if(inputType == "spawnradius2")
            {
                is >> gparam::spawnRadius;
            } 
            else if(inputType == "ready")
            {
                /* This is the end of the parameter input. */
                state.timer.start();
                break;
            } 
            else 
            {
                getline(is, junk);
            }
        }
    } 
    else 
    {
        /* Reads in information about the current turn. */
        while(is >> inputType)
        {
            if(inputType == "w")
            {
                /* Water square. */
                is >> row >> col;
                state.grid[row][col].isWater = 1;
            }
            else if(inputType == "f")
            {
                /* Food square. */
                is >> row >> col;
                state.grid[row][col].isFood = 1;
                state.food.push_back(Location(row, col));
            } 
            else if(inputType == "a")
            {
                /* Live ant square. */
                is >> row >> col >> player;
                state.grid[row][col].antPlayer = player;
                if(player == 0)
                {
					state.grid[row][col].myAntNumber = state.myAnts.size ();
                    state.myAnts.push_back(Location(row, col));
                }
                else
                {
                    state.enemyAnts.push_back(Location(row, col));
                }
            }
            else if(inputType == "d")
            {
                /* Dead ant squares. */
                is >> row >> col >> player;
            } 
            else if(inputType == "h")
            {
                /* Hill square. */
                is >> row >> col >> player;
                state.grid[row][col].isHill = 1;
                state.grid[row][col].hillPlayer = player;
                if(player == 0)
                {
                    state.myHills.push_back(Location(row, col));
                }
                else
                {
                    state.enemyHills.push_back(Location(row, col));
                }
            }
            else if(inputType == "players") 
            {
                /* Information about the players. */
                is >> gparam::numberPlayers;
            }
            else if(inputType == "scores")
            {
                /* Information about the scores. */
                state.scores = std::vector<double>(gparam::numberPlayers, 0.0);
                for(int p = 0; p < gparam::numberPlayers; p++)
                {
                    is >> state.scores[p];
                }
            }
            else if(inputType == "go")
            {
                /* Finished input. */
                if(state.gameOver)
                {
                    LOG("Received end of game message.");
                    is.setstate(std::ios::failbit);
                }
                else 
                {
                    state.timer.start();
                }
                break;
            } 
            else 
            {
                getline(is, junk);
            }
        }
    }
    return is;
}

