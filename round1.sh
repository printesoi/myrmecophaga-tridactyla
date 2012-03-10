#!/bin/bash

bot='./MyBot'
bot1='./bot1_32'

cp botul_nostru/bin/MyBot .

#./playgame.py -t 100 --engine_seed 42 --player_seed 0 -l game_logs -m maps/cell_maze/cell_maze_p02_19.map "$bot1" "$bot"
./playgame.py -v -t 1000 --engine_seed 42 --player_seed 0 -l game_logs -m maps/cell_maze/cell_maze_p02_20.map "$bot1" "$bot"
#./playgame.py -t 100 --engine_seed 42 --player_seed 0 -l game_logs -m maps/random_walk/random_walk_p02_21.map "$bot1" "$bot"
#./playgame.py -t 100 --engine_seed 42 --player_seed 0 -l game_logs -m maps/random_walk/random_walk_p02_24.map "$bot1" "$bot"

exit 0
