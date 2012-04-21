#!/bin/bash

bot='./bot1_64'
mybot='./MyBot'

make &>/dev/null
cp bin/MyBot ../
cd ..
./playgame.py --player_seed 0 --engine_seed 42 -v -l game_logs -m easy.map $mybot $bot
cd botul_nostru

exit 0
