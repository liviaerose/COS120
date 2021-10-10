/**
  *@file Board.cpp
 * @brief Board class implementation (also called 'definition') file.
 * @author Stefan Brandle
 * @date March 2012
 */
//Livia Rose and Jolie Rabideau

#include <iostream>
#include <cstdlib>
#include <unistd.h>

// When writing a class implementation file, you must "#include" the class
// declaration file.
#include "Board.h"

// We also use the conio namespace contents, so must "#include" the conio declarations.
#include "conio.h"
#include "Programmer.h"
#include "Human.h"
/**
 * @brief The Board class constructor, responsible for intializing a Board object.
 * The Board constructor is responsible for initializing all the Board object variables.
 *
 * @param rows The number of rows to make the board.
 * @param cols The number of columns to make the board.
 * @param numberOfHumans The number of humans to place on the board.
 */
Board::Board(int rows, int cols, int numberOfHumans) {
    numHumans = numberOfHumans;
    numRows = rows;
    numCols = cols;
    currentTime = 0;
    uSleepTime = 250000;
    numInfected=0;
    numDead=0;
    numQuarantine=0;
    numNonbeliever=0;
}

/**
 * @brief The Board class destructor.
 * The Board destructor is responsible for any last-minute cleaning 
 * up that a Board object needs to do before being destroyed. In this case,
 * it needs to return all the memory borrowed for creating the Human objects.
 */
Board::~Board() {
    for(int pos=0; pos<numHumans; ++pos) {
        delete humans[pos];
    }
}

/**
 * @brief function that runs the simulation
 * Begins the simulation with the key information. Creates human objects.
 * Infects one human, then runs simulation until all are infected.
 */
void Board::run() {
    int row, col;

    cout<<"Green characters are healthy humans." <<endl;
    cout<<"Red characters are humans that are infected but do not know they are infected."<<endl;
    cout<<"Yellow characters are humans that are infected but are quarantined: they move around but keep 6 feet (3 characrer spaces) between them and anyother human." << endl;
    cout<<"Magenta characters are humans that are infected and know they are infected but do not believe in quarantines." <<endl;
    cout<<"Gray characters are humans that have died from infection."<<endl;
    cout<<"The stats on each category of will be displayed under the infection simulator board." <<endl;

    cout << "Press control c to quit or Press Enter to begin simulation ";
    char junk=cin.get();
        while (junk != '\n'){
            junk=cin.get();
        }

    // Creates 'Human' objects and sets the array pointers to point at them.
    for(int pos=0; pos<numHumans; pos++) {

         row = pos%numRows;       // row will be in range(0, numRows-1)
         col = random()%numCols;  // col will be in range(0, numCols-1)
         // Create and initialize another Human. 
         // Parameters are  row on board, col on board, initialy infected, 
         // a pointer to this board object ('this'), and the current role.
         humans[pos] = new Human(row, col, false, this, "Human"); 
    }

    // Infect a random human in the range 0 to numHumans-1
    int victim=random()%numHumans;
    humans[victim]->setInfected();  
    humans[victim]->getLocation(row,col);
    delete humans[victim];
    humans[victim] = new Programmer(row, col, true, this, "Programmer");

    for(currentTime=0; currentTime<240 && allInfected() == false; ++currentTime) {
        // Clear screen before every new time unit
        cout << conio::clrscr() << flush;

        // Tell each human to try moving
        for(int pos=0; pos<numHumans; ++pos) {
            humans[pos]->move();
        }

        // Deal with infection propagation.
        processInfection();
        

        // Tell each human to draw itself on board with updated infection status
        for(int pos=0; pos<numHumans; ++pos) {
            humans[pos]->draw();
        }
        
        // Checks to see if the human is infected and if so it is randomly placed in quarantine or nonbeliever in quarantine.
        for (int pos=0; pos<numHumans; pos++){
            bool known=false;
            if (humans[pos]->isInfected()){
                if (random()%2==0){
                    known=true;
                }
            }

            if ((humans[pos]->getRole()).compare("Programmer")==0 && known){
                Programmer* p = (Programmer*) humans[pos];
                if (p->getQuarantine()==false && p->getNonbeliever()==false){
                    if(random()%2==0){
                        p->setQuarantine();
                    }
                
                    else{
                        p->setNonbeliever();
                    }
                }
            }
       
            Programmer* point = (Programmer*) humans[pos];
            if(humans[pos]->isInfected()){
                if(random()%1000==0){
                    if ((humans[pos]->getRole()).compare("Programmer")==0){
                        point->setDead();
                    }
                }
            }
        }


        // Print statistics.
        cout << conio::gotoRowCol(numRows+3, 1) 
             << "Time=" << currentTime 
             << " Total humans=" << numHumans
             << " Total infected=" << numInfected << endl
             << " Total quarantined (6 feet apart at all times)="<< numQuarantine << endl
             << " Total infected but chose not to be quarantined=" << numNonbeliever << endl
             << " Total dead=" << numDead << flush;


        // Sleep specified microseconds
        usleep(uSleepTime);
    }

    // Position the cursor so prompt shows up on its own line
    cout << endl;
}

/**
 * @brief Determines whether or not all humans are infected.
 * @return If even one human is uninfected, returns false. Otherwise, returns true.
 */
bool Board::allInfected() {
    for(int i=0; i<numHumans; ++i) {
        if(humans[i]->isInfected() == false) return false;
    }

    return true;
}

/**
 * @brief The function that handles one infection cycle to determine what new infections
 *        are present.
 * For each pair of adjacent humans in the simulation, processInfection() makes sure that if 
 * one is infected, the other becomes infected as well.
 */
void Board::processInfection() {
    for( int i=0; i<numHumans; ++i ) {
        for( int j=i+1; j<numHumans; ++j ) {
            int row, col;
            if( isNextTo(humans[i], humans[j]) ){
                if( humans[i]->isInfected() && humans[j]->isInfected()==false ) {
                   humans[j]->setInfected();
                   humans[j]->getLocation(row, col);
                   delete humans[j];
                   humans[j] = new Programmer(row, col, true, this, "Programmer");
                } else if ( humans[j]->isInfected() && humans[i]->isInfected()==false ) {
                    humans[i]->setInfected();
                    humans[i]->getLocation(row, col);
                    delete humans[i];
                    humans[i] = new Programmer(row, col, true, this, "Programmer");
                }
                
                else if( (humans[i]->getRole()).compare("Programmer")==0 && (humans[j]->getRole()).compare("Human")==0 ) {
                   humans[j]->setInfected();
                   humans[j]->getLocation(row, col);
                   delete humans[j];
                   humans[j] = new Programmer(row, col, true, this, "Programmer");
                } else if (( humans[j]->getRole()).compare("Programmer")==0 && (humans[i]->getRole()).compare("Human")==0 ) {
                    humans[i]->setInfected();
                    humans[i]->getLocation(row, col);
                    delete humans[i];
                    humans[i] = new Programmer(row, col, true, this, "Programmer");
                }
                
                
                
            }
        }
    }

    // Reset the board 'numInfected' count and recount how many are infected.
     numInfected = 0;
     numQuarantine=0;
     numNonbeliever=0;
     numDead=0;

    for( int i=0; i<numHumans; ++i ) {
        if(( humans[i]->getRole()).compare("Programmer")==0 ) numInfected++;

        Programmer* point=(Programmer*) humans[i];
        if ((humans[i]->getRole()).compare("Programmer")==0){
            if(point->getQuarantine()) numQuarantine++;
            if(point->getNonbeliever()) numNonbeliever++;
        }
        if ((humans[i]->getRole()).compare("Programmer")==0){
            if(point->getDead()) numDead++;
        }
    }
}

/**
 * @brief The function that determines whether a particular move can happen.
 *        If the move would go off the board, or land on the same position as another
 *        human, the function returns false (do not move). Otherwise, it returns true (ok to proceed).
 * @param[in] row the row the human wishes to move to.
 * @param[in] col the col the human wishes to move to.
 * @return Whether the human calling this function may move to the specified row and column.
 */
bool Board::tryMove(int row, int col) {
    int tryRow=-1, tryCol=-1;
    int quarantineRow=-3, quarantineCol=-3;

    //if human is dead, the human can't move
    for (int i=0; i<numHumans; i++){
        Programmer* ppoint=(Programmer*) humans[i];
        if ((humans[i]->getRole()).compare("Programmer")==0){
            if(ppoint->getDead()){
                return false;
            }
        }
    }

    //if human is in quarantine, the human cannot move within three virtual spaces of any other human.
    for (int i=0; i<numHumans; i++){
        Programmer* progpoint=(Programmer*) humans[i];
            if ((humans[i]->getRole()).compare("Programmer")==0){
                if (progpoint->getQuarantine()){
                    progpoint->getLocation(quarantineRow, quarantineCol);
                   // if (abs(row-quarantineRow)<=3 && abs(col-quarantineCol)<=3) return false;
                }
            }
    }

    // If off board, the move is not permitted
    if( row<0 || row>=numRows || col<0 || col>=numCols ) return false;

    // Else if another human is on the same location, the move is not permitted
    for(int i=0; i<numHumans; ++i) {
        humans[i]->getLocation(tryRow, tryCol);
        if( row==tryRow && col==tryCol ) return false;
       
    }

    // No problems, so the move is permitted
    return true;
}

/**
 * @brief The function that determines whether two humans are on adjacent squares.
 * @param h1 pointer to first human object.
 * @param h2 pointer to second human object.
 * @return Whether or not h1 and h2 are on adjacent squares.
 */
bool Board::isNextTo(Human *h1, Human* h2) {
    // Get human location information
    int h1Row, h1Col;
    h1->getLocation(h1Row, h1Col);
    int h2Row, h2Col;
    h2->getLocation(h2Row, h2Col);

    // Return whether h1 and h2 are on adjacent squares in any direction 
    // (horizontally, vertically, diagonally).
    return abs(h1Row-h2Row)<=1 && abs(h1Col-h2Col)<=1;
}


