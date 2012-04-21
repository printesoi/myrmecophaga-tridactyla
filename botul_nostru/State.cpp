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

#include "State.h"

/* Resets all non-water squares to land and clears the bots ant vector. */
void State::reset()
{
    myAnts.clear();
    myHills.clear();
    enemyAnts.clear();
    enemyHills.clear();
    food.clear();

    myAntsNew.clear();
    myHillsNew.clear();
    enemyAntsNew.clear();
    enemyHillsNew.clear();
    foodNew.clear();

    borderTiles.clear();

    for(int row = 0; row < gparam::mapRows; row++)
        for(int col = 0; col < gparam::mapCols; col++)
            grid[row][col].reset();
}

Square *State::square(const Location loc)
{
    return &grid[loc.row][loc.col];
}

/* Returns the Manhattan distance between two locations. */
int State::manhattan(const Location loc1,const Location loc2)
{
    int rez = 0,
        min = loc2.row - loc1.row,
        max = loc1.row - loc2.row;
    if (min < 0)
        min += gparam::mapRows;
    else
        max += gparam::mapRows;
    if (min < max)
        rez += min;
    else
        rez += max;

    min = loc2.col - loc1.col;
    max = loc1.col - loc2.col;
    if (min < 0)
        min += gparam::mapCols;
    else
        max += gparam::mapCols;
    if (min < max)
        rez += min;
    else
        rez += max;

    return rez;
}

/* Returns the square of Euclid distance between two locations. */
double State::distance(const Location loc1,const Location loc2)
{
    int d11 = loc1.row - loc2.row;
    int d1 = d11 < 0 ? -d11 : d11;
    int d22 = loc1.col - loc2.col;
    int d2 = d22 < 0 ? -d22 : d22;
    int dr = d1 < (gparam::mapRows - d1) ? d1 : (gparam::mapRows - d1);
    int dc = d2 < (gparam::mapCols - d2) ? d2 : (gparam::mapCols - d2);
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
    while (!squares.empty())
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
    while (!changed.empty())
    {
        x = changed.front();
        f = &grid[x.row][x.col];
        f->isMarked = -1;
        f->exploreIndex = 0;
        changed.pop_front();
    }
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

    while (!squares.empty())
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

    while (!squares.empty())
    {
        x = squares.front();
        f = &grid[x.row][x.col];
        rez += f->exploreIndex;
        squares.pop_front();
    }

    while (!changed.empty())
    {
        x = changed.front();
        f = &grid[x.row][x.col];
        f->isMarked = -1;
        changed.pop_front();
    }

    return rez;
}

/* The A* algorithm. */
int State::Astar(Location from,Location to)
{
    std::vector<Square*> open;
    std::list<Square*> changed;

    int tentative_g_score;
    bool tentative_is_better;

    Location curr = from;

    Square *fr = square(curr),*cu,*ne;
    fr->g = 0;
    fr->h = manhattan(from,to);
    fr->f = fr->g + fr->h;
    fr->dir = -1;

    fr->isMarked = 0;
    changed.push_back(fr);

    open.push_back(fr);
    while (!open.empty())
    {
        make_heap(open.begin(),open.end(),Compare());
        cu = open[0];

        if (Location(*cu) == to)
            break;

        pop_heap(open.begin(),open.end(),Compare());
        open.pop_back();

        cu->isMarked = -2;
        for (int dir = 0; dir < 4; dir++)
        {
            ne = square(Location(*cu).move(dir));
            if (ne->isMarked == -2 || ne->isWater)
                continue;

            tentative_g_score = cu->g + 1;

            if (ne->isMarked == -1)
            {
                ne->isMarked = 0;
                ne->h = manhattan(Location(*ne),to);

                changed.push_back(ne);
                open.push_back(ne);
                push_heap(open.begin(),open.end(),Compare());

                tentative_is_better = true;
            }
            else
                tentative_is_better = tentative_g_score < ne->g;

            if (tentative_is_better)
            {
                ne->dir = ((dir + 2) > 3 ? dir - 2 : dir + 2);
                ne->g = tentative_g_score;
                ne->f = ne->g + ne->h;
            }
        }
    }

    int rez = -1;

    cu = square(to);
    while (cu->dir != -1)
    {
        rez = (cu->dir + 2) > 3 ? cu->dir - 2 : cu->dir + 2;
        cu = square(Location(*cu).move(cu->dir));
    }

    while (!changed.empty())
    {
        cu = changed.front();

        cu->isMarked = -1;
        cu->f = -1;
        cu->g = -1;
        cu->h = -1;
        cu->dir = -1;
        changed.pop_front();
    }

    return rez;
}

/** Initializes the grid (map variable). */
void State::initGrid()
{
    for (int i = 0; i < gparam::mapRows; i++)
    {
        grid.push_back(std::vector<Square>());
        for (int j = 0; j < gparam::mapCols; j++)
            grid[i].push_back(Square(i,j));
    }
}

/** Initialize the neighbours of the squares in the grid. */
void State::initNeighbours()
{
    /** Initialize n0 neighbour. */
    for (int i = 0; i < gparam::mapCols; i++)
        grid[0][i].neigh.push_back(&grid[gparam::mapRows - 1][i]);
    for (int i = 1; i < gparam::mapRows; i++)
        for (int j = 0; j < gparam::mapCols; j++)
            grid[i][j].neigh.push_back(&grid[i - 1][j]);

    /** Initialize n1 neighbour. */
    for (int i = 0; i < gparam::mapRows; i++)
        grid[i][gparam::mapCols - 1].neigh.push_back(&grid[i][0]);
    for (int i = 0; i < gparam::mapRows; i++)
        for (int j = 0; j < gparam::mapCols - 1; j++)
            grid[i][j].neigh.push_back(&grid[i][j + 1]);

    /** Initialize n2 neighbour. */
    for (int i = 0; i < gparam::mapCols; i++)
        grid[gparam::mapRows - 1][i].neigh.push_back(&grid[0][i]);
    for (int i = 0; i < gparam::mapRows - 1; i++)
        for (int j = 0; j < gparam::mapCols; j++)
            grid[i][j].neigh.push_back(&grid[i + 1][j]);

    /** Initialize n3 neighbour. */
    for (int i = 0; i < gparam::mapRows; i++)
        grid[i][0].neigh.push_back(&grid[i][gparam::mapCols - 1]);
    for (int i = 0; i < gparam::mapRows; i++)
        for (int j = 1; j < gparam::mapCols; j++)
            grid[i][j].neigh.push_back(&grid[i][j - 1]);
}

/* Input functions. */
std::istream& operator>>(std::istream &is,State &state)
{
    int row,col,player;
    std::string inputType,junk;

    /* Read in input type. */
    while (is >> inputType)
    {
        if (inputType == "end")
        {
            state.gameOver = true;
            break;
        }
        else if (inputType == "turn")
        {
            is >> state.currentTurnNumber;
            break;
        }
        else
        {
            getline(is,junk);
        }
    }

    if (state.currentTurnNumber == 0)
    {
        /* If we are at the beginning of the game,read in the parameters. */
        while (is >> inputType)
        {
            if (inputType == "loadtime")
            {
                is >> gparam::loadTime;
            }
            else if (inputType == "turntime")
            {
                is >> gparam::turnTime;
            }
            else if (inputType == "rows")
            {
                is >> gparam::mapRows;
            }
            else if (inputType == "cols")
            {
                is >> gparam::mapCols;
                /** Initialize the grid. */
                state.initGrid();
                state.initNeighbours();
            }
            else if (inputType == "turns")
            {
                is >> gparam::totalTurnsNumber;
            }
            else if (inputType == "player_seed")
            {
                is >> gparam::seed;
                srand((unsigned int)gparam::seed);
            }
            else if (inputType == "viewradius2")
            {
                is >> gparam::viewRadius;
            }
            else if (inputType == "attackradius2")
            {
                is >> gparam::attackRadius;
            }
            else if (inputType == "spawnradius2")
            {
                is >> gparam::spawnRadius;
            }
            else if (inputType == "ready")
            {
                /* This is the end of the parameter input. */
                state.timer.start();
                break;
            }
            else
            {
                getline(is,junk);
            }
        }
    }
    else
    {
        /* Reads in information about the current turn. */
        while (is >> inputType)
        {
            if (inputType == "w")
            {
                /* Water square. */
                is >> row >> col;
                state.grid[row][col].isWater = 1;
            }
            else if (inputType == "f")
            {
                /* Food square. */
                is >> row >> col;
                state.grid[row][col].isFood = 1;
                state.food.push_back(Location(row,col));
                state.foodNew.push_back(&state.grid[row][col]);
            }
            else if (inputType == "a")
            {
                /* Live ant square. */
                is >> row >> col >> player;
                state.grid[row][col].antPlayer = player;
                if (player == 0)
                {
                    state.grid[row][col].myAntNumber = state.myAnts.size();
                    state.myAnts.push_back(Location(row,col));
                    state.myAntsNew.push_back(&state.grid[row][col]);
                }
                else
                {
                    state.enemyAnts.push_back(Location(row,col));
                    state.enemyAntsNew.push_back(&state.grid[row][col]);
                }
            }
            else if (inputType == "d")
            {
                /* Dead ant squares. */
                is >> row >> col >> player;
            }
            else if (inputType == "h")
            {
                /* Hill square. */
                is >> row >> col >> player;
                state.grid[row][col].isHill = 1;
                state.grid[row][col].hillPlayer = player;
                if (player == 0)
                {
                    state.myHills.push_back(Location(row,col));
                    state.myHillsNew.push_back(&state.grid[row][col]);
                }
                else
                {
                    state.enemyHills.push_back(Location(row,col));
                    state.enemyHillsNew.push_back(&state.grid[row][col]);
                }
            }
            else if (inputType == "players")
            {
                /* Information about the players. */
                is >> gparam::numberPlayers;
            }
            else if (inputType == "scores")
            {
                /* Information about the scores. */
                state.scores = std::vector<double>(gparam::numberPlayers,0.0);
                for(int p = 0; p < gparam::numberPlayers; p++)
                {
                    is >> state.scores[p];
                }
            }
            else if (inputType == "go")
            {
                /* Finished input. */
                if (state.gameOver)
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
                getline(is,junk);
            }
        }
    }
    return is;
}

