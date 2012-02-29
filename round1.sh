#!/bin/bash

./playgame.py -t 100 --engine_seed 42 --player_seed 0 -l game_logs -m maps/cell_maze/cell_maze_p02_19.map "./Myla" "./bot1_32"
./playgame.py -t 100 --engine_seed 42 --player_seed 0 -l game_logs -m maps/cell_maze/cell_maze_p02_20.map "./Myla" "./bot1_32"
./playgame.py -t 100 --engine_seed 42 --player_seed 0 -l game_logs -m maps/random_walk/random_walk_p02_21.map "./Myla" "./bot1_32"
./playgame.py -t 100 --engine_seed 42 --player_seed 0 -l game_logs -m maps/random_walk/random_walk_p02_24.map "./Myla" "./bot1_32"

exit 0
