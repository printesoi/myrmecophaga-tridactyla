/*
 * =============================================================================
 *
 *       Filename:  Bot.h
 *        Authors:  Dodon Victor <dodonvictor at gmail dot com>
 *                  Cernov Vladimir <kaspersky_gg@gmail.com>
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

#include "Strategy.h"

struct Bot
{
    /** Current state of the game. */
    State state;

	/** Map for storing routes. */
	/** TODO REPLACE WITH HAST_TABLE. */
	
	std::map<int,int> hash;

    /** Plays a single game of Ants. */
    void playGame();

    /** Moves ants on the board. */
    void makeMoves();

    /** Indicates to the engine that it has made its moves. */
    void endTurn();

	/** Calculates the shortest path from location "from"
	 *  to location "to" and return it's length. */
	int bfs(Location from, Location to);
};

#endif

