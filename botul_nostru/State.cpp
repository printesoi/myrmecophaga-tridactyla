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

/* Returns the Manhattan distance between two locations. */
int State::manhattan(Square *loc1,Square *loc2)
{
    int rez = 0,
        min = loc2->x - loc1->x,
        max = loc1->x - loc2->x;
    if (min < 0)
        min += gparam::mapRows;
    else
        max += gparam::mapRows;
    if (min < max)
        rez += min;
    else
        rez += max;

    min = loc2->y - loc1->y;
    max = loc1->y - loc2->y;
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

/** Resets the exploreIndex of the "visible" squares to 0. */
void State::mark_explored()
{
    std::queue<Square *> open;
    std::queue<Square *> closed;

    Square *f,*t;
    for (unsigned int ant = 0; ant < myAntsNew.size(); ant++)
    {
        myAntsNew[ant]->isMarked = 0;
        open.push(myAntsNew[ant]);
        closed.push(myAntsNew[ant]);
    }
    while (!open.empty())
    {
        f = open.front();
        open.pop();

        if (f->isMarked == VIEW_RADIUS)
            break;

        for (int dir = 0; dir < 4; dir++)
        {
            t = f->neigh[dir];

            if (!t->isWater && t->isMarked == -1)
            {
                t->isMarked = f->isMarked + 1;
                open.push(t);
                closed.push(t);
            }
        }
    }
    while (!closed.empty())
    {
        f = closed.front();
        f->isMarked = -1;
        f->exploreIndex = 0;
        closed.pop();
    }
}

/** Calculates a sum which represents a fog indicator. */
int State::unexplored_index(Square *from)
{
    std::queue<Square *> squares;
    std::queue<Square *> changed;

    Square *f,*t;

    int rez = 0;

    from->isMarked = 0;
    changed.push(from);
    squares.push(from);

    while (!squares.empty())
    {
        f = squares.front();
        squares.pop();

        if (f->isMarked == VIEW_RADIUS)
        {
            rez = f->exploreIndex;
            break;
        }

        for (int dir = 0; dir < 4; dir++)
        {
            t = f->neigh[dir];

            if (!t->isWater && t->isMarked == -1)
            {
                t->isMarked = f->isMarked + 1;
                changed.push(t);
                squares.push(t);
            }
        }
    }

    while (!squares.empty())
    {
        f = squares.front();
        rez += f->exploreIndex;
        squares.pop();
    }

    while (!changed.empty())
    {
        f = changed.front();
        f->isMarked = -1;
        changed.pop();
    }

    return rez;
}

/* The A* algorithm. */
int State::Astar(Square *from,Square *to)
{
    std::vector<Square *> open;
    std::list<Square *> changed;

    int tentative_g_score;
    bool tentative_is_better;

    Square *curr = from;

    Square *fr = curr,*cu,*ne;
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

        if (cu->x == to->x && cu->y == to->y)
            break;

        pop_heap(open.begin(),open.end(),Compare());
        open.pop_back();

        cu->isMarked = -2;
        for (int dir = 0; dir < 4; dir++)
        {
            ne = cu->neigh[dir];
            if (ne->isMarked == -2 || ne->isWater)
                continue;

            tentative_g_score = cu->g + 1;

            if (ne->isMarked == -1)
            {
                ne->isMarked = 0;
                ne->h = manhattan(ne,to);

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

    cu = to;
    while (cu->dir != -1)
    {
        rez = (cu->dir + 2) > 3 ? cu->dir - 2 : cu->dir + 2;
        cu = cu->neigh[cu->dir];
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
                LOG("Atack radius " << gparam::atackRadius);
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
                state.foodNew.push_back(&state.grid[row][col]);
            }
            else if (inputType == "a")
            {
                /* Live ant square. */
                is >> row >> col >> player;
                state.grid[row][col].antPlayer = player;
                if (player == 0)
                {
                    state.grid[row][col].myAntNumber = state.myAntsNew.size();
                    state.myAntsNew.push_back(&state.grid[row][col]);
                }
                else
                {
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
                    state.myHillsNew.push_back(&state.grid[row][col]);
                }
                else
                {
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

