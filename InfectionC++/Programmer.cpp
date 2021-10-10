/**
 * @file Programmer.cpp
 * @brief The Programmer class implementation
 *
 * @author Livia Rose
 * @author Jolie Rabideau
 * @date May 2020
**/

#include <cstdlib>
#include <iostream>

#include "Programmer.h" 
#include "Human.h"
#include "conio.h"

Programmer::Programmer(int initRow, int initCol, bool initInfected, Board* thisBoard, string role)
: Human(initRow, initCol, initInfected, thisBoard, role)
{
   infected = initInfected;
   isQuarantine=false; 
   isNonbeliever=false;
   isDead=false;
}

Programmer::~Programmer(){
    // nothing to do
}

/**
  * @brief This is overwriting the Human class move function. Have the human try to move.
  * To know whether it is ok to move to some position (r,c), ask the board
  * whether the position is ok. E.G., "if (board->tryMove(r,c) )..."
  * If the move is ok, then update the human's row and column to reflect the move.
 **/
void Programmer::move() {
    // Generate a +/- 2 row and column delta.
    int rowDelta=random()%5-2, colDelta=random()%5-2;
    // Ask the board whether you
    if(board->tryMove(initRow+rowDelta, initCol+colDelta)) {
        initRow+=rowDelta;
        initCol+=colDelta;
    }

}

/**
 * @brief This is overwriting the Human class draw function. Moving to a specific row and col
 * and then changing the quarantined people yellow, the nonbelievers purple, the dead people grey,
 * and everybody else red..
 */
void Programmer::draw(){
    cout << conio::gotoRowCol(initRow+1, initCol+1);
    if(isQuarantine){
        cout<<conio::bgColor(conio::LIGHT_YELLOW);
    }if(isNonbeliever){
        cout<<conio::bgColor(conio::LIGHT_MAGENTA);
    }if(isDead){
        cout<<conio::bgColor(conio::LIGHT_GRAY);
    }else {
        cout << conio::bgColor(conio::LIGHT_RED);
    }

    cout << 'P' << conio::resetAll() << flush;
}

/**
 * @brief Checks to see if programmer is in quarantine
 * @return sends back the value of isQuarantine
 */
bool Programmer::getQuarantine() {
	return isQuarantine;
}

/**
 * @brief Puts the programmer into quarantine: they stay 3 virtual spaces away from any other human or programmer. 
*/
void Programmer::setQuarantine() {
	isQuarantine=true;
}

/**
 * @brief Checks to see if the programmer doesn't believe in quarantine
 * @return sends back the value of isNonbeliever
 */
bool Programmer::getNonbeliever(){
	return isNonbeliever;
}

/**
 * @brief Sets the programmer as an individual who knows they are infected but
 * they don't believe the quarantine works: they continue walking around like normal
 */
void Programmer::setNonbeliever(){
	isNonbeliever=true;
}

/**
 * @brief Checks to see if the programmer is dead
 * @return sends back the value of isDead
 */
bool Programmer::getDead(){
	return isDead;
}

/** 
 * @brief Sets the programmer as dead.
 */
void Programmer::setDead(){
	isDead=true;
}

