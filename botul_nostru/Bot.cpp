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

        state.mark_visible();
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

void Bot::gatherFood()
{
	int rez;
	for (unsigned int food = 0; food < state.food.size(); food++)
	{
		if (freeAntsNumber())
		{
			LOG(" THIS IS WHERE FOOD ENDS ");
			rez = bfs(state.food[food]);
		}
		else
			break;
	}
	rez *= 2;
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

int Bot::bfs(Location from)
{
	std::list<Location> squares;
	std::list<Location> changed;
	
	int rez = -1;
	Location x,y;
	Square *f,*t;

	state.grid[from.row][from.col].isMarked = 0;
	changed.push_back(from);
	squares.push_back(from);

	while (squares.size())
	{
		x = squares.front();
		f = &state.grid[x.row][x.col];
		LOG("-> " << f->isMarked << "\n");
		squares.pop_front();
		
		LOG(x.row << " " << x.col << "\n");
		LOG(f->isMarked << "\n");
		
		if (f->isMarked > 15)
			break;

		//Direction 0
		y = x.move(0);
		t = &state.grid[y.row][y.col];

		if (t->antPlayer == 0)
		{
			if (jobs[t->myAntNumber] == -1)
			{
				jobs[t->myAntNumber] = 2;
				rez = f->isMarked + 1;
				break;
			}
		}

		if (!t->isWater && t->isMarked == -1)
		{
			t->isMarked = f->isMarked + 1;
			changed.push_back(y);
			squares.push_back(y);
		}

		//Direction 1
		y = x.move(1);
		t = &state.grid[y.row][y.col];

		if (t->antPlayer == 0)
		{
			if (jobs[t->myAntNumber] == -1)
			{
				jobs[t->myAntNumber] = 3;
				rez = f->isMarked + 1;
				break;
			}
		}

		if (!t->isWater && t->isMarked == -1)
		{
			t->isMarked = f->isMarked + 1;
			changed.push_back(y);
			squares.push_back(y);
		}

		//Direction 2
		y = x.move(2);
		t = &state.grid[y.row][y.col];

		if (t->antPlayer == 0)
		{
			if (jobs[t->myAntNumber] == -1)
			{
				jobs[t->myAntNumber] = 0;
				rez = f->isMarked + 1;
				break;
			}
		}

		if (!t->isWater && t->isMarked == -1)
		{
			t->isMarked = f->isMarked + 1;
			changed.push_back(y);
			squares.push_back(y);
		}

		//Direction 3
		y = x.move(3);
		t = &state.grid[y.row][y.col];

		if (t->antPlayer == 0)
		{
			if (jobs[t->myAntNumber] == -1)
			{
				jobs[t->myAntNumber] = 1;
				rez = f->isMarked + 1;
				break;
			}
		}

		if (!t->isWater && t->isMarked == -1)
		{
			t->isMarked = f->isMarked + 1;
			changed.push_back(y);
			squares.push_back(y);
		}
	}

	while (changed.size())
	{
		x = changed.front();
		f = &state.grid[x.row][x.col];
		f->isMarked = -1;
		changed.pop_front();
	}

	return rez;
}

int Bot::freeAntsNumber()
{
	int rez = 0;
	for (unsigned i = 0; i < jobs.size(); i++)
		if (jobs[i] == -1)
			rez++;
	return rez;
}
