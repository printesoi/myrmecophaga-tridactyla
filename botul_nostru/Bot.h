/*
 * =============================================================================
 *
 *       Filename:  Bot.h
 *        Authors:  Dodon Victor <dodonvictor at gmail dot com>
 *                  Cernov Vladimir <gg.kaspersky@gmail.com>
 *                  Maldur Vitalie <maldur.vitalik@yahoo.com>
 *                  Straticiuc Vicu <straticiuc_vicu@yahoo.com>
 *        Created:  02/25/2012
 *    Description:
 *
 * =============================================================================
 */

/** Structure that represents an AI in a game. */
#ifndef BOT_H_
#define BOT_H_

#include <unordered_map>

#include "Strategy.h"

struct Bot
{
    /** Current state of the game. */
    State state;

	/** Map for storing routes.
	 * Victor: Replaced with hash table */
	std::unordered_map<int,int> hash;

	/** Jobs that ants will do.
	 *  Represent the direction to move. */
	std::vector<int> jobs;

	/** Plays a single game of Ants. */
    void playGame();

	/** Gather food. */
	void gatherFood();

    /** Moves ants on the board. */
    void makeMoves();

    /** Indicates to the engine that it has made its moves. */
    void endTurn();

	/** Calculates the distance to the nearest ant from location "from". */
	int bfs(Location from);

	/** Returns the number of free ants. */
	int freeAntsNumber();
};

#endif

