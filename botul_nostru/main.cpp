/*
 * =============================================================================
 *
 *       Filename:  main.cpp
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

int main(int argc, char *argv[])
{
    Bot* bot = new Bot();
    bot->playGame();
    delete bot;

    LOG("Bot gracefully shutting down...");

    return 0;
}
