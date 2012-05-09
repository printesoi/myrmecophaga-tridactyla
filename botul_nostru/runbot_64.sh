#!/bin/bash

bot1='./bot1_64'
mybot='./MyBot'
bot3='./bot3_64'
xathis='java -cp xathis MyBot'
#map='maps/cell_maze/cell_maze_p02_01.map'
map='easy.map'

make
cp bin/MyBot ../
cd ..
./playgame.py --player_seed 0 --engine_seed 42 -v -l game_logs -m $map "$mybot" "$bot3"
cd botul_nostru

exit 0
