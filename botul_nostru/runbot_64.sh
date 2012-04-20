#!/bin/bash

make &>/dev/null
cp bin/MyBot ../
cd ..
./playgame.py --player_seed 0 --engine_seed 42 -v -l game_logs -m easy.map \"./MyBot\" \"./bot2_64\"
cd botul_nostru

exit 0
