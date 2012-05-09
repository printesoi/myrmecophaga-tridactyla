#!/bin/bash

bot='./MyBot'
xathis='java -cp xathis MyBot'
contrabot='./bot3_64'

cp botul_nostru/bin/MyBot .

./playgame.py -v -t 1000 --engine_seed 42 --player_seed 0 -l game_logs -m maps/cell_maze/cell_maze_p02_10.map "$contrabot" "$bot"
#./playgame.py -v -t 1000 --engine_seed 42 --player_seed 0 -l game_logs -m maps/cell_maze/cell_maze_p02_15.map "$contrabot" "$bot"
#./playgame.py -v -t 1000 --engine_seed 42 --player_seed 0 -l game_logs -m maps/maze/maze_p02_14.map "$contrabot" "$bot"
#./playgame.py -v -t 1000 --engine_seed 42 --player_seed 0 -l game_logs -m maps/random_walk/random_walk_p02_10.map "$contrabot" "$bot"

exit 0
