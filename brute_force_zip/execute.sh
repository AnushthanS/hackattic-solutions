#!/bin/zsh

cd build
cmake .. -G Ninja -DCMAKE_CXX_COMPILER=clang++
ninja 

./get_problem
./brute_force_zip