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

		//Direction 0
		y = x.move(0);
		t = &state.grid[y.row][y.col];

		if (t->antPlayer == 0)
			if (jobs[t->myAntNumber] == -1)
			{
				active[f->foodIndex] = false;
				jobs[t->myAntNumber] = 2;
			}

		if (!t->isWater && t->isMarked == -1)
		{
			t->isMarked = f->isMarked + 1;
			t->foodIndex = f->foodIndex;
			changed.push_back(y);
			squares.push_back(y);
		}

		//Direction 1
		y = x.move(1);
		t = &state.grid[y.row][y.col];
		if (t->antPlayer == 0)
			if (jobs[t->myAntNumber] == -1)
			{
				LOG(active.size() << " " << t->foodIndex);
				active[f->foodIndex] = false;
				jobs[t->myAntNumber] = 3;
			}
		if (!t->isWater && t->isMarked == -1)
		{
			t->isMarked = f->isMarked + 1;
			t->foodIndex = f->foodIndex;
			changed.push_back(y);
			squares.push_back(y);
		}

		//Direction 2
		y = x.move(2);
		t = &state.grid[y.row][y.col];

		if (t->antPlayer == 0)
			if (jobs[t->myAntNumber] == -1)
			{
				active[f->foodIndex] = false;
				jobs[t->myAntNumber] = 0;
			}

		if (!t->isWater && t->isMarked == -1)
		{
			t->isMarked = f->isMarked + 1;
			t->foodIndex = f->foodIndex;
			changed.push_back(y);
			squares.push_back(y);
		}

		//Direction 3
		y = x.move(3);
		t = &state.grid[y.row][y.col];

		if (t->antPlayer == 0)
			if (jobs[t->myAntNumber] == -1)
			{
				active[f->foodIndex] = false;
				jobs[t->myAntNumber] = 1;
			}

		if (!t->isWater && t->isMarked == -1)
		{
			t->isMarked = f->isMarked + 1;
			t->foodIndex = f->foodIndex;
			changed.push_back(y);
			squares.push_back(y);
		}
	}
	while (changed.size())
	{
		x = changed.front();
		f = &state.grid[x.row][x.col];
		f->isMarked = -1;
		f->foodIndex = -1;
		changed.pop_front();
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
