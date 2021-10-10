//----------------------------------------------------
// The "#ifndef ..." and "#define ..." lines are used to prevent the compiler from accidentally
// processing the contents of Board.h more than once, thus causing "Board redefined" errors.
// At the end of the file is a "#endif" which marks the end of the "#ifndef" section.
//----------------------------------------------------
#ifndef BOARD_H
#define BOARD_H

/**
 * @file Board.h
 * @brief the Board class declaration file
 *
 * @author Stefan Brandle
 * @date March 2012
 * @author Livia Rose
 * @author Jolie Rabideau
 * @date May 2020
 */

//----------------------------------------------------
// "forward" declaration of Board. Tell the compiler that this will be
// defined properly further "forward" in the program. This is needed because
// both classes reference each other. Otherwise, when include "Human.h" and 
// the compiler finds a reference to Board, it will complain.
//----------------------------------------------------
class Board;

// The Board class uses the Human class, so must include the Human class declaration.
#include "Human.h"

using namespace std;

/**
 * @class Board
 * @brief The Board class declaration.
 */
class Board {
  public:
    Board(int numRows, int numCols, int numHumans); // Board class constructor
    ~Board();                 // Board class destructor
    void run();               // Main function that runs the simulation
    bool tryMove(int row, int col); // Function that lets human know whether move is ok
    static const int MAX_HUMAN_COUNT = 100; // Maximum humans simulation can handle

  protected:
    //----------------------------------------------------
    // Private functions and data. These cannot be referenced other than by functions that are
    // part of the Board class.
    //----------------------------------------------------
    void processInfection();  // Go through and process infection status
    bool allInfected();       // Tells whether all humans are infected
    bool isNextTo(Human* h1, Human* h2); // Tells whether one human is next to another

    Human* humans[MAX_HUMAN_COUNT];
    int numHumans;            // Num humans
    int numInfected;          // Num humans infected
    int currentTime;          // Current time in simulation
    int numRows;              // Number of rows in board
    int numCols;              // Number of cols in board
    int numDead;
    int numQuarantine;
    int numNonbeliever;
    int uSleepTime;           // Num microseconds to sleep between updates
};

#endif //#ifndef BOARD_H
