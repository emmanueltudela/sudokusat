# SudokuSat

I tried to implement a sudoku reduction to sat problem. It is very ineficient but I had fun.
I used SLIST for the first time (which is probably why it is slow). The reduction seems to
work, all my tests pass, you can try it by modifying the file loaded in main and using a 4*4
grid. This is not a solver, it will only tell you if you can solve the grid. Considering
the time it takes i wouldn't recommend using it in your solver.

## Concept
Read the concept of my implementation [here](concept.pdf)

## Dependencies
- gcc
- make
- cmake

## How to build
``` bash
mkdir build
cd build
cmake ..
make
```
