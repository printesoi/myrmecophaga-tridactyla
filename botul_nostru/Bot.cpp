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
    
    srand((unsigned int)gparam::seed);

    /* Continues to make moves until game is over. */
    while(std::cin >> state)
    {
        LOG("turn " << state.currentTurnNumber << ":");
		
		//Reset ants' jobs
		jobs.clear ();
		for (unsigned int i = 0; i < state.myAnts.size(); i++)
			jobs.push_back(-1);
		state.mark_explored();
		gatherFood();
		explore();
        makeMoves();

        endTurn();
        LOG("Time taken: " << state.timer.getTime() << "ms");
        LOG("Alocated Time: " << gparam::turnTime<<"ms" << std::endl);
    }
}

void Bot::makeMoves()
{
    for (int ant = 0; ant < (int)state.myAnts.size(); ++ant)
    {
		int direction = jobs[ant];
		if (direction == -1)
        	/* Try moving this ant in some random direction. */
        	direction = rand() % 4;
        Location newLocation = state.myAnts[ant].move(direction);
        /* Destination shouldn't be water and shouldn't be an ant. */
        if (!state.grid[newLocation.row][newLocation.col].isWater &&
                state.grid[newLocation.row][newLocation.col].antPlayer == -1)
        {
            /* Move ant. */
            state.grid[newLocation.row][newLocation.col].antPlayer = 0;
            state.grid[state.myAnts[ant].row][state.myAnts[ant].col].antPlayer = -1;
            /* Outputs move information correctly to the engine. */
            std::cout << "o" << " " << state.myAnts[ant].row << " " <<
                    state.myAnts[ant].col << " " << DIRECTION_LETTER[direction] << std::endl; 
        }
    }
}

void Bot::endTurn()
{
    LOG("Sending endTurn()");

    /* If this wasn't the start game, reset the board. */
    if(state.currentTurnNumber > 0)
		state.reset();

    /* Move to next turn. */
    state.currentTurnNumber++;

    std::cout << "go" << std::endl;
}

void Bot::gatherFood()
{
	std::list<Location> squares;
	std::list<Location> changed;
	std::vector<bool> active;

	Location x,y;
	Square *f,*t;

	for (unsigned int hill = 0; hill < state.enemyHills.size(); hill++)
		state.food.push_back(state.enemyHills[hill]);

	int nr = 0;
	for (unsigned int i = 0; i < state.food.size(); i++)
		if (state.grid[state.food[i].row][state.food[i].col].exploreIndex == 0)
		{
			state.grid[state.food[i].row][state.food[i].col].isMarked = 0;
			state.grid[state.food[i].row][state.food[i].col].foodIndex = nr++;
			changed.push_back(state.food[i]);
			squares.push_back(state.food[i]);
			active.push_back(true);
		}

	while (squares.size())
	{
		x = squares.front();
		f = &state.grid[x.row][x.col];
		squares.pop_front();
		
		if (f->isMarked > VIEW_RADIUS)
			break;
		
		if (!active[f->foodIndex])
			continue;

		for (int dir = 0; dir < 4; dir++)
		{
			y = x.move(dir);
			t = &state.grid[y.row][y.col];

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
				changed.push_back(y);
				squares.push_back(y);
			}
		}
	}

	squares.clear();

	while (changed.size())
	{
		x = changed.front();
		f = &state.grid[x.row][x.col];
		f->isMarked = -1;
		f->foodIndex = -1;
		changed.pop_front();
	}
}

void Bot::explore()
{
	for (unsigned int ant = 0; ant < jobs.size(); ant++)
		if (jobs[ant] == -1)
		{
			int dir = -1;
			int sum0 = 0;
			Location y;
			for (int i = 0; i < 4; i++)
			{
				y = state.myAnts[ant].move(i);
				if (!state.grid[y.row][y.col].isWater)
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

int Bot::freeAntsNumber()
{
	int rez = 0;
	for (unsigned i = 0; i < jobs.size(); i++)
		if (jobs[i] == -1)
			rez++;
	return rez;
}
