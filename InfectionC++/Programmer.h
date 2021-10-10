/**
 * @file Programmer.h
 * @brief Declaration of the Programmer Class
 *
 * @author Livia Rose
 * @author Joie Rabideau
 * @date 4/27/20
 *
**/

#ifndef PROGRAMMER_H
#define PROGRAMMER_H

#include "Human.h"

using namespace std;

class Programmer: public Human{
    public: 
        Programmer(int initRow, int initCol, bool initInfected,  Board* thisBoard, string role);
        ~Programmer();
        void move();
        void draw();
        bool getQuarantine();
        void setQuarantine();
        bool getNonbeliever();
        void setNonbeliever();
        bool getDead();
        void setDead();
    private:
        bool isQuarantine;
        bool isNonbeliever;
        bool isDead;
        bool initInfected;
        int initRow;
        int initCol;

};


#endif // PROGRAMMER_H

