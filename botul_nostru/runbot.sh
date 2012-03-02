#!/bin/bash

make &>/dev/null
cp bin/Myla ../
cd ..
./playgame.py --player_seed 0 --engine_seed 42 -l game_logs -m maps/maze/maze_02p_01.map \"./Myla\" \"./Myla\" &>/dev/null
cd botul_nostru

exit 0
