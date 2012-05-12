#!/bin/bash

bot1='./bot1_64'
mybot='./MyBot'
bot3='./bot3_64'
xathis='java -cp xathis MyBot'
mihai='java -jar MyBot.jar'
map='maps/cell_maze/cell_maze_p04_19.map'
#map='easy.map'

make
cp bin/MyBot ../
cd ..
./playgame.py --player_seed 0 --engine_seed 42 -v -l game_logs -m $map "$xathis" "$bot3" "$bot3" "$mihai"
cd botul_nostru

exit 0
