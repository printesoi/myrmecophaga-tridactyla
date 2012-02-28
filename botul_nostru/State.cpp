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

/* Marks which squares on the map are visible. This function can be greatly
 * improved! */
void State::mark_visible()
{
    for (int row = 0; row < gparam::mapRows; ++row)
    {
        for (int col = 0; col < gparam::mapColumns; ++col)
        {
            /* Check of any of the ants sees this. */
            for (int ant = 0; ant < (int)myAnts.size(); ++ant)
            {
                if (distance(Location(row, col), myAnts[ant]) <= gparam::viewRadius)
                {
                    grid[row][col].isVisible = true;
                    break;
                }
            }
        }
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
		grid[food[ant].row][food[ant].col].isMarked = 0;
		changed.push_back(food[ant]);
		squares.push_back(food[ant]);
	}

	while (squares.size())
	{
		x = squares.front();
		f = &grid[x.row][x.col];
		squares.pop_front();
		
		if (f->isMarked > 15)
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
}

/** Outputs the exploreIndexes to LOGFILE. */
void State::explore_log()
{
	for (int i = 0; i < gparam::mapRows; i++)
	{
		for (int j = 0; j < gparam::mapColumns; j++)
			if (grid[i][j].exploreIndex)
				LOG_NEOLN("X");
			else
				LOG_NEOLN(" ");
		LOG_NEOLN("\n");
	}
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

