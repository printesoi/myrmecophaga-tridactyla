#!/bin/bash

cp bin/Myla ../
cd ..
./playgame.py -l game_logs -m maps/maze/maze_02p_01.map \"./Myla\" \"./bot1_32\" &>/dev/null
cd botul_nostru

exit 0
