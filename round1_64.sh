#!/bin/bash

bot1='./bot1_64'
mybot='./MyBot'

./playgame.py -v -t 100 --engine_seed 42 --player_seed 0 -l game_logs -m maps/cell_maze/cell_maze_p02_19.map "$mybot" "$bot1"
./playgame.py -v -t 100 --engine_seed 42 --player_seed 0 -l game_logs -m maps/cell_maze/cell_maze_p02_20.map "$mybot" "$bot1"
./playgame.py -v -t 100 --engine_seed 42 --player_seed 0 -l game_logs -m maps/random_walk/random_walk_p02_21.map "$mybot" "$bot1"
./playgame.py -v -t 100 --engine_seed 42 --player_seed 0 -l game_logs -m maps/random_walk/random_walk_p02_24.map "$mybot" "$bot1"

exit 0
