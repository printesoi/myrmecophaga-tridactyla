#!/bin/bash

bot='./MyBot'
bot2='./bot2_64'

cp botul_nostru/bin/MyBot .

./playgame.py -v -t 1000 --engine_seed 42 --player_seed 0 -l game_logs -m maps/cell_maze/cell_maze_p02_19.map "$bot2" "$bot"
./playgame.py -v -t 1000 --engine_seed 42 --player_seed 0 -l game_logs -m maps/cell_maze/cell_maze_p02_20.map "$bot2" "$bot"
./playgame.py -v -t 1000 --engine_seed 42 --player_seed 0 -l game_logs -m maps/maze/maze_p02_01.map "$bot2" "$bot"
./playgame.py -v -t 1000 --engine_seed 42 --player_seed 0 -l game_logs -m maps/random_walk/random_walk_p02_11.map "$bot2" "$bot"

exit 0
