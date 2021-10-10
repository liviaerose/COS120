/**
 * @file main.cpp
 * @brief Contains the main function that starts the infection simulation.
 *
 * @author Stefan Brandle
 * @date March 2012
 * 
 * @author Livia Rose
 * @author Jolie Rabideau
 * @date May 2020
 **/

#include <iostream>
#include <cstdlib>

#include "Board.h" //incuded because we create a board objecy in main()

using namespace std;

/**
 * @fn main()
 * @brief main function that starts the simulation running
 * The main function does the following:
 * (1) seeds the random number generator
 * (2) creates a board object that is 20 rows, 80 columns, 70 humans
 * (3) starts the simulation running by calling the board's run function.
 **/
int main() {
    // Only seed the random number generator once!

    Board board(20, 80, 70);
    board.run();

    return 0;
}
