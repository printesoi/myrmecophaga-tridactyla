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

#include <queue>
#include <algorithm>

#include "State.h"

class Bot
{
    public:
        /** Current state of the game. */
        State state;

        /** Jobs that ants will do. */
        /** Represent the direction to move. */
        std::vector<int> jobs;

        /** Plays a single game of Ants. */
        void playGame();

        /** Prepare the data for a round. */
        void initRound();

        /** Gather food. */
        void gatherFood();

        /** Moves ants on the board. */
        void makeMoves();

        /** Explores the map. */
        void explore();

        /** Tries to hunt enemy hills. */
        void huntHills();

        /** Move free ants to the border. */
        void toBorder();

        /** Calculates areas. */
        void areas();

        /** Returns a direction to a border. */
        int findBorder(Square *from);

        /** Indicates to the engine that it has made its moves. */
        void endTurn();

        /** Returns the number of free ants. */
        int freeAntsNumber();
};

#endif

