#!/bin/bash

bot1='./bot1_64'
mybot='./MyBot'
bot3='./bot3_64'

make
cp bin/MyBot ../
cd ..
./playgame.py --player_seed 0 --engine_seed 42 -v -l game_logs -m easy.map $mybot $bot3
cd botul_nostru

exit 0
