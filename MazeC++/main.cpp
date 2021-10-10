//Authors: Livia Rose, Jolie Rabideau
//Date: 04/02/2020
using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "conio.h"
#include <unistd.h>

using namespace std;
using namespace conio;

void loadMaze( char maze[10][10], bool tempMaze[10][10]);
void printMaze(char maze[10][10], bool tempMaze[10][10]);
bool canEscape(char maze[10][10], bool tempMaze[10][10], int row, int col);
void askRowCol (int &row, int &col);
void moveCursor(char maze[10][10], bool tempMaze[10][10], int row, int col);



int main() {
    char maze[10][10];
    bool tempMaze[10][10];
    int row;
    int col;
    while (true){
        cout << clrscr();
        loadMaze(maze, tempMaze);
        cout << conio::clrscr();
        printMaze(maze, tempMaze);
        askRowCol(row, col);
        if (canEscape(maze,tempMaze, row, col)){
            cout << conio::gotoRowCol(16,1);
            cout << conio::resetAll() << flush;
            cout << "Can Escape" << endl;
        }else{
            cout << conio::gotoRowCol(16,1);
            cout << conio::resetAll() << flush;
            cout << "Can't Escape" << endl;
            
        }
        //cin.ignore(10,'\n');
        cout << "Press control c to quit or Press Space then Enter to Continue! ";
        while (cin.get() == '\n'){

            continue;
        }
    }
    return 0;
    
}

void loadMaze( char maze[10][10], bool tempMaze[10][10]){
       
    string fileName;
    cout << gotoRowCol(0,0);
    cout << "What is the file name? ";
    cin >> fileName;
    
    ifstream mazeStream(fileName.c_str());
    while(! mazeStream){
        mazeStream.clear();
        cout << "Error opening file: " << fileName << endl;
        cout << "Enter filename: ";
        cin >> fileName;
        mazeStream.open(fileName.c_str());
    }
    char junk;
    cout << "Load maze from: " << fileName << endl;
        for (int row=0; row<10; row++){
            for(int col=0; col<10; col++){
                mazeStream.get(maze[row][col]);
                tempMaze[row][col]==false;
            }
            mazeStream.get(junk);
        }
}

void printMaze (char maze[10][10], bool tempMaze[10][10]){
        //cout << conio::clrscr();
        cout << conio::gotoRowCol(1,1) << " 0123456789" << endl;
        for (int row=0; row<10; row++){
            cout << conio::gotoRowCol(row+2,1) << row;
            for(int col=0; col<10; col++){
                cout << conio::gotoRowCol(row+2,col+2)<< maze[row][col];
            }
            cout << endl;

        }
        cout << endl;
}

void askRowCol ( int &row, int &col){
    cout<< "Where do you want to start (row col): ";
    cin >> row;
    cin >> col;
    cout << conio::gotoRowCol(row+2,col+2);
}



bool canEscape (char maze[10][10], bool tempMaze[10][10], int row, int col){
    if (row<0 || row>=10 || col<0 || col>=10) return false;
    else if (maze[row][col]== 'E') return true;
    else if (maze[row][col]== '#') return false;
    //if you have been there before (turns red)
    else if (maze[row][col]== 'x') return false;
    //else if (maze[row][col]=='.'){
      //  moveCursor(maze, tempMaze, row, col);
    //}
    else{
        cout << conio::gotoRowCol(row+2,col+2) << bgColor(YELLOW)
             << "x\b" << flush;
        usleep(500000);
        maze[row][col]='x';
        if (canEscape(maze, tempMaze, row-1, col) ||
            canEscape(maze, tempMaze, row, col-1) ||
            canEscape(maze, tempMaze, row+1, col) ||
            canEscape(maze, tempMaze, row, col+1) 
           )
        { 
            cout << conio::gotoRowCol(row+2,col+2) << bgColor(GREEN)
                 << "$\b" << flush;
            return true;
        }
        else{
            cout << conio::gotoRowCol(row+2,col+2) << bgColor(RED)
                 << "X\b" << flush;
            return false;
        }
    }
}








