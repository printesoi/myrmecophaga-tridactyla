/*
 * =============================================================================
 *
 *       Filename:  Bot.cpp
 *        Authors:  Dodon Victor <dodonvictor at gmail dot com>
 *                  Cernov Vladimir <gg.kaspersky@gmail.com>
 *                  Maldur Vitalie <maldur.vitalik@yahoo.com>
 *                  Straticiuc Vicu <straticiuc_vicu@yahoo.com>
 *        Created:  02/25/2012
 *    Description:
 *
 * =============================================================================
 */

#include "Bot.h"

void Bot::playGame()
{
    /* Reads in game parameters. */
    LOG("Reading initial parameters.");
    std::cin >> state;
    endTurn();

    srand((unsigned)gparam::seed);

    /* Continues to make moves until game is over. */
    while(std::cin >> state)
    {
        LOG("turn " << state.currentTurnNumber << ":");

        initRound();
        state.mark_explored();
        mark_all_direct_dangered();
        mark_all_indirect_dangered();
        get_combatAnts();
        gatherFood();
        explore();
        areas();
        toBorder();
        huntHills();
        makeMoves();

        endTurn();
        LOG("Time taken: " << state.timer.getTime() << "ms");
        LOG("Alocated Time: " << gparam::turnTime<<"ms" << std::endl);
    }
}

void Bot::initRound()
{
    jobs.clear ();
    for (unsigned i = 0; i < state.myAntsNew.size(); i++)
        jobs.push_back(-1);
    combatAnts.clear();
}

void Bot::huntHills()
{
    if (!state.enemyHillsNew.empty())
    {
        for (unsigned ant = 0; ant < state.myAntsNew.size(); ++ant)
            if (jobs[ant] == -1)
                jobs[ant] = state.Astar(state.myAntsNew[ant],state.enemyHillsNew[0]);
    }
}

void Bot::makeMoves()
{
    for (unsigned ant = 0; ant < state.myAntsNew.size(); ++ant)
    {
        int direction = jobs[ant];
        if (direction == -1)
            direction = rand() % 4;

        Square *newSquare = state.myAntsNew[ant]->neigh[direction];
        /* Destination shouldn't be water and shouldn't be an ant. */
        if (!newSquare->isWater && newSquare->antPlayer == -1)
        {
            /* Move ant. */
            newSquare->antPlayer = 0;
            state.myAntsNew[ant]->antPlayer = -1;

            /* Outputs move information correctly to the engine. */
            std::cout << "o" << " " << state.myAntsNew[ant]->x << " " <<
                state.myAntsNew[ant]->y << " " << DIRECTION_LETTER[direction] <<
                std::endl;
        }
    }
    for (unsigned ant = 0; ant < state.myAntsNew.size(); ++ant)
    {
        int direction = jobs[ant];
        if (direction == -1)
            direction = rand() % 4;

        Square *newSquare = state.myAntsNew[ant]->neigh[direction];
        /* Destination shouldn't be water and shouldn't be an ant. */
        if (!newSquare->isWater && newSquare->antPlayer == -1)
        {
            /* Move ant. */
            newSquare->antPlayer = 0;
            state.myAntsNew[ant]->antPlayer = -1;

            /* Outputs move information correctly to the engine. */
            std::cout << "o" << " " << state.myAntsNew[ant]->x << " " <<
                state.myAntsNew[ant]->y << " " << DIRECTION_LETTER[direction] <<
                std::endl;
        }
    }
}

void Bot::endTurn()
{
    LOG("Sending endTurn()");

    if (state.currentTurnNumber > 0)
        state.reset();

    /* Move to next turn. */
    state.currentTurnNumber++;

    std::cout << "go" << std::endl;
}

/** Move free ants to the border. */
void Bot::toBorder()
{
    for (unsigned ant = 0; ant < state.myAntsNew.size(); ++ant)
        if (jobs[ant] == -1 && state.borderTiles.size() > 0)
            jobs[ant] = findBorder(state.myAntsNew[ant]);
}

/** Returns a direction to a border. */
int Bot::findBorder(Square *from)
{
    std::queue<Square *> open;
    std::queue<Square *> changed;
    std::queue<int> where;

    Square *f,*to;
    int rez = -1;

    from->isMarked = 0;
    changed.push(from);

    for (int dir = 0; dir < 4; dir++)
    {
        to = from->neigh[dir];
        if (!to->isWater)
        {
            to->isMarked = from->isMarked + 1;
            open.push(to);
            changed.push(to);
            where.push(dir);
        }
    }

    while (!open.empty())
    {
        f = open.front();
        int d = where.front();
        open.pop();
        where.pop();

        if (f->isBorder)
        {
            f->isBorder = false;
            /** Side-effect to borderTiles vector. */
            state.borderTiles.pop_back();
            rez = d;
            break;
        }

        for (int dir = 0; dir < 4; dir++)
        {
            to = f->neigh[dir];

            if (!to->isWater && to->isMarked == -1)
            {
                to->isMarked = f->isMarked + 1;
                to->foodIndex = f->foodIndex;
                open.push(to);
                changed.push(to);
                where.push(d);
            }
        }
    }

    while (!changed.empty())
    {
        f = changed.front();
        f->isMarked = -1;
        changed.pop();
    }

    return rez;
}

/** Move around and get some food. */
void Bot::gatherFood()
{
    std::queue<Square *> squares;
    std::queue<Square *> changed;
    std::vector<bool> active;

    Square *f,*t;

    /** TODO This is a temporary fix and should be removed. */
    for (unsigned i = 0; i < state.enemyHillsNew.size(); i++)
        state.foodNew.push_back(state.enemyHillsNew[i]);

    int nr = 0;
    for (unsigned i = 0; i < state.foodNew.size(); i++)
        if (state.foodNew[i]->exploreIndex == 0)
        {
            state.foodNew[i]->isMarked = 0;
            state.foodNew[i]->foodIndex = nr++;
            changed.push(state.foodNew[i]);
            squares.push(state.foodNew[i]);
            active.push_back(true);
        }

    while (!squares.empty())
    {
        f = squares.front();
        squares.pop();

        if (f->isMarked > VIEW_RADIUS)
            break;

        if (!active[f->foodIndex])
            continue;

        for (int dir = 0; dir < 4; dir++)
        {
            t = f->neigh[dir];

            if (t->antPlayer == 0)
                if (jobs[t->myAntNumber] == -1)
                {
                    active[f->foodIndex] = false;
                    if (dir + 2 > 3)
                        jobs[t->myAntNumber] = dir - 2;
                    else
                        jobs[t->myAntNumber] = dir + 2;
                }

            if (!t->isWater && t->isMarked == -1)
            {
                t->isMarked = f->isMarked + 1;
                t->foodIndex = f->foodIndex;
                squares.push(t);
                changed.push(t);
            }
        }
    }

    while (!changed.empty())
    {
        f = changed.front();
        f->isMarked = -1;
        f->foodIndex = -1;
        changed.pop();
    }
}

/** Move around to unexplored tiles. */
void Bot::explore()
{
    for (unsigned ant = 0; ant < jobs.size(); ant++)
        if (jobs[ant] == -1)
        {
            int dir = -1;
            int sum0 = 0;
            Square *y;
            for (int i = 0; i < 4; i++)
            {
                y = state.myAntsNew[ant]->neigh[i];
                if (!y->isWater)
                {
                    int sum = state.unexplored_index(y);
                    if (sum > sum0)
                    {
                        sum0 = sum;
                        dir = i;
                    }
                }
            }
            jobs[ant] = dir;
        }
}

/** Calculates the borders of own ants area. */
void Bot::areas()
{
    std::queue<Square *> squares;
    std::queue<Square *> changed;

    Square *f,*t;
    for (unsigned ant = 0; ant < state.myAntsNew.size(); ant++)
    {
        state.grid[state.myAntsNew[ant]->x][state.myAntsNew[ant]->y].isMarked = 0;
        changed.push(state.myAntsNew[ant]);
        squares.push(state.myAntsNew[ant]);
    }
    for (unsigned ant = 0; ant < state.enemyAntsNew.size(); ant++)
    {
        state.grid[state.enemyAntsNew[ant]->x][state.enemyAntsNew[ant]->y].isMarked = -2;
        changed.push(state.enemyAntsNew[ant]);
        squares.push(state.enemyAntsNew[ant]);
    }
    while (!squares.empty())
    {
        f = squares.front();
        squares.pop();

        if (f->isMarked == 2 * VIEW_RADIUS)
            break;

        if (f->isMarked == -2 * VIEW_RADIUS - 2)
            break;

        for (int dir = 0; dir < 4; dir++)
        {
            t = f->neigh[dir];

            if (!t->isWater && t->isMarked == -1)
            {
                if (f->isMarked < 0)
                    t->isMarked = f->isMarked - 1;
                else
                    t->isMarked = f->isMarked + 1;
                changed.push(t);
                squares.push(t);
            }
        }
    }

    /** Calculate the border tiles. */
    for (int i = 0; i < gparam::mapRows; i++)
        for (int j = 0; j < gparam::mapCols; j++)
        {
            int nr = 0;
            for (int dir = 0; dir < 4; dir++)
                if (state.grid[i][j].neigh[dir]->isMarked < 0)
                    if (!state.grid[i][j].neigh[dir]->isWater)
                        nr++;
            if (nr > 0)
            {
                state.grid[i][j].isBorder = true;
                state.borderTiles.push_back(&state.grid[i][j]);
            }
        }

    /** Reset changed information. */
    while (!changed.empty())
    {
        f = changed.front();
        f->isMarked = -1;
        changed.pop();
    }
}

/** Gets the number of free ants. */
int Bot::freeAntsNumber()
{
    int rez = 0;
    for (unsigned i = 0; i < jobs.size(); i++)
        if (jobs[i] == -1)
            rez++;
    return rez;
}

/** Marks direct dangered tiles by an enemy. */
void Bot::mark_direct_dangered(Square *ant)
{
    /** This assumes an atack radius of 5. */
    int x = ant->x;
    int y = ant->y;
    int nx,ny;

    nx = x - 3;
    ny = y;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x - 3;
    ny = y + 1;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x - 2;
    ny = y + 2;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x - 1;
    ny = y + 3;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x;
    ny = y + 3;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x + 1;
    ny = y + 3;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x + 2;
    ny = y + 2;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x + 3;
    ny = y + 1;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x + 3;
    ny = y;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x + 3;
    ny = y - 1;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x + 2;
    ny = y - 2;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x + 1;
    ny = y - 3;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x;
    ny = y - 3;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x - 1;
    ny = y - 3;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x - 2;
    ny = y - 2;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

    nx = x - 3;
    ny = y - 1;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].dd++;

}

/** Marks all the direct dangered tiles. */
void Bot::mark_all_direct_dangered()
{
    /** This assumes an atack radius of 5. */
    for (unsigned eAnt = 0; eAnt < state.enemyAntsNew.size(); eAnt++)
        mark_direct_dangered(state.enemyAntsNew[eAnt]);
    for (int i = 0; i < gparam::mapRows; i++)
    {
        for (int j = 0; j < gparam::mapCols; j++)
            if (state.grid[i][j].antPlayer == 0)
                if (state.grid[i][j].dd)
                    LOG_NEOLN("!");
                else
                    LOG_NEOLN("x");
            else
                if (state.grid[i][j].antPlayer == -1)
                    if (state.grid[i][j].isWater)
                        LOG_NEOLN("M");
                    else
                        LOG_NEOLN(".");
                else
                    LOG_NEOLN("O");
        LOG("");
    }
}

/** Marks indirect dangered tiles by an enemy. */
void Bot::mark_indirect_dangered(Square *ant)
{
    /** This assumes an atack radius of 5. */
    int x = ant->x;
    int y = ant->y;
    int nx,ny;

    nx = x - 4;
    ny = y;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x - 4;
    ny = y + 1;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x - 3;
    ny = y + 2;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x - 2;
    ny = y + 3;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x - 1;
    ny = y + 4;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x;
    ny = y + 4;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x + 1;
    ny = y + 4;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x + 2;
    ny = y + 3;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x + 3;
    ny = y + 2;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x + 4;
    ny = y + 1;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x + 4;
    ny = y;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x + 4;
    ny = y - 1;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x + 3;
    ny = y - 2;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x + 2;
    ny = y - 3;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x + 1;
    ny = y - 4;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x;
    ny = y - 4;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x - 1;
    ny = y - 4;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x - 2;
    ny = y - 3;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x - 3;
    ny = y - 2;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;

    nx = x - 4;
    ny = y - 1;
    nx += gparam::mapRows;
    nx %= gparam::mapRows;
    ny += gparam::mapCols;
    ny %= gparam::mapCols;
    state.grid[nx][ny].di++;
}

/** Marks all the indirect dangered tiles. */
void Bot::mark_all_indirect_dangered()
{
    /** This assumes an atack radius of 5. */
    for (unsigned eAnt = 0; eAnt < state.enemyAntsNew.size(); eAnt++)
        mark_indirect_dangered(state.enemyAntsNew[eAnt]);
    for (int i = 0; i < gparam::mapRows; i++)
    {
        for (int j = 0; j < gparam::mapCols; j++)
            if (state.grid[i][j].dd > 0)
                LOG_NEOLN("T");
            else
                if (state.grid[i][j].di > 0)
                    LOG_NEOLN("H");
                else
                    LOG_NEOLN(".");
        LOG("");
    }
}

/** Finds the ants in combat. */
void Bot::get_combatAnts()
{
    for (unsigned myant = 0; myant < state.myAntsNew.size(); myant++)
        if (state.myAntsNew[myant]->dd || state.myAntsNew[myant]->di)
            combatAnts.push_back(state.myAntsNew[myant]);
    for (unsigned i = 0; i < combatAnts.size(); i++)
        LOG("Combat: " << combatAnts[i]->x << " " << combatAnts[i]->y);
}
