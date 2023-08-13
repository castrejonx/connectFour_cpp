#include <cstdio>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cctype>
#include <windows.h>

// WinApi header
//C stuff
/*
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
*/

using namespace std;

//Initializes entire grid to 'empty' slots
void resetGame(char array[6][7])
{
    for(int i=0; i<=5; i++)
    {
        for(int j=0; j<=6; j++)
        {
            array[i][j]=' ';
        }
    }
}

//Prints the current state of the game grid
void printGame(char array[6][7])
{
 //   _setmode(_fileno(stdout),_O_U16TEXT);

    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    HANDLE hstdin  = GetStdHandle( STD_INPUT_HANDLE  );
	HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );

	// Captures current state in order to go back to it later
    CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo( hstdout, &csbi );

    const char topLeftBorder = '\xC9'; // 201
    const char topBottomBorder = '\xCD'; // 205
    const char topMiddleBorder = '\xCB'; // 203
    const char topRightBorder = '\xBB'; // 187
    const char rightMiddleBorder = '\xB9'; // 185
    const char bottomRightBorder = '\xBC'; // 188
    const char bottomMiddleBorder = '\xCA'; // 202
    const char bottomLeftBorder = '\xC8'; // 200
    const char leftMiddleBorder = '\xCC'; // 204
	const char leftRightBorder = '\xBA'; // 186
	const char middleMiddleBorder = '\xCE'; // 206

    cout<<" 1 2 3 4 5 6 7\n";
    /*cout<<topLeftBorder<<topBottomBorder<<topMiddleBorder<<topBottomBorder<<topMiddleBorder<<topBottomBorder<<
        topMiddleBorder<<topBottomBorder<<topMiddleBorder<<topBottomBorder<<topMiddleBorder<<topBottomBorder<<
        topMiddleBorder<<topBottomBorder<<topRightBorder<<"\n";*/

    for(int i=0; i<=5; i++)
    {
        SetConsoleTextAttribute(hConsole, 7); //Sets text color to white
        cout<<leftMiddleBorder<<topBottomBorder<<middleMiddleBorder<<topBottomBorder<<middleMiddleBorder<<topBottomBorder<<
        middleMiddleBorder<<topBottomBorder<<middleMiddleBorder<<topBottomBorder<<middleMiddleBorder<<topBottomBorder<<
        middleMiddleBorder<<topBottomBorder<<rightMiddleBorder<<"\n";
        for(int j=0; j<=6; j++)
        {
            cout<<leftRightBorder;
            if(array[i][j]=='x\01')
            {
                SetConsoleTextAttribute(hConsole, 4); //Sets text color to red
                cout<<array[i][j];
                SetConsoleTextAttribute(hConsole, 7); //Sets text color to white
            }
            else if(array[i][j]=='x\02')
            {
                SetConsoleTextAttribute(hConsole, 1); //Sets text color to blue
                cout<<array[i][j];
                SetConsoleTextAttribute(hConsole, 7); //Sets text color to white
            }
            else
                cout<<array[i][j];
        }
        cout<<leftRightBorder<<"\n";
    }
    cout<<bottomLeftBorder<<topBottomBorder<<bottomMiddleBorder<<topBottomBorder<<bottomMiddleBorder<<topBottomBorder<<
        bottomMiddleBorder<<topBottomBorder<<bottomMiddleBorder<<topBottomBorder<<bottomMiddleBorder<<topBottomBorder<<
        bottomMiddleBorder<<topBottomBorder<<bottomRightBorder<<"\n";
        SetConsoleTextAttribute( hstdout, csbi.wAttributes ); //Sets text color back to green

}

//Inserts player game piece into column and places it at 'lowest', 'empty' slot available
void insertGamepiece(char array[6][7], int column, char gamePiece)
{
    int row=0;
        for(int i=0; i<=5; i++)
        {
            if(array[i][column-1]==' ')
                row++;
        }
    array[row-1][column-1]=gamePiece;
    printGame(array);
}

//Check for four game pieces in a row
bool rowOfFour(char array[6][7], char alpha)
{
    //Check rows for horizontal four in a row
    for(int i=5; i>=0; i--)
    {
        for(int j=0; j<=3; j++)
        {
            if(array[i][j]==alpha && array[i][j+1]==alpha && array[i][j+2]==alpha && array[i][j+3]==alpha)
                return true;
        }
    }
    //Check columns for vertical four in a row
    for(int j=0; j<=6; j++)
    {
        for(int i=5; i>=3; i--)
        {
            if(array[i][j]==alpha && array[i-1][j]==alpha && array[i-2][j]==alpha && array[i-3][j]==alpha)
                return true;
        }
    }
    //Check diagonal four in a row, up and right, from left to right
    for(int i=5; i>=3; i--)
    {
        for(int j=0; j<=3; j++)
        {
            if(array[i][j]==alpha && array[i-1][j+1]==alpha && array[i-2][j+2]==alpha && array[i-3][j+3]==alpha)
                return true;
        }
    }
    //Check diagonal four in a row, up and left, from left to right
    for(int i=5; i>=3; i--)
    {
        for(int j=3; j<=6; j++)
        {
            if(array[i][j]==alpha && array[i-1][j-1]==alpha && array[i-2][j-2]==alpha && array[i-3][j-3]==alpha)
                return true;
        }
    }
    return false;
}

//Checks whether all the slots are full and no moves are left
bool fullGame(char array[6][7])
{
    if(array[0][0]!=' ' && array[0][1]!=' ' && array[0][2]!=' ' && array[0][3]!=' ' &&
       array[0][4]!=' ' && array[0][5]!=' ' && array[0][6]!=' ')
          return true;
    else
        return false;
}

//Checks whether all the slots are full in a column
bool fullColumn(char array[6][7], int columnNumber)
{
    //cout<<"connectFour[0]["<<columnNumber<<"]="<<array[0][columnNumber]<<endl;
    if(array[0][columnNumber]!=' ')
        return true;

    else
        return false;
}

int main()
{
    char connectFour[6][7]; //Create Connect Four 6x7 grid
    char columnChoice; //Player choice of column to play
    int icolumnChoice; //Converted holder for player column choice from a char to an int
    char columnPlay; //Not sure whether to use this yet
    const char player1Gamepiece='x\01';
    const char player2Gamepiece='x\02';
    bool turn=true; //Player 1 always goes first

    resetGame(connectFour); //Grid initialized to 'empty' slots
    printGame(connectFour); //Print the initialized board

    while(rowOfFour(connectFour,player1Gamepiece)==false && rowOfFour(connectFour,player2Gamepiece)==false && fullGame(connectFour)==false)
    {
        if(turn==true && rowOfFour(connectFour,player2Gamepiece)==false)
        {
            cout<<"Player 1: Choose your column\n";
            cin>>columnChoice;
            icolumnChoice=columnChoice-48;

            while(icolumnChoice!=1 && icolumnChoice!=2 && icolumnChoice!=3 && icolumnChoice!=4 &&
                    icolumnChoice!=5 && icolumnChoice!=6 && icolumnChoice!=7)
            {
                cout<<"You must choose a column number between 1 and 7. \nPlease try again\n";
                cin>>columnChoice;
                icolumnChoice=columnChoice-48;
            }

            while(fullColumn(connectFour,icolumnChoice-1)==true)
            {
                cout<<"That column is full. Try another\n";
                cin>>columnChoice;
                icolumnChoice=columnChoice-48;
            }
        }
        insertGamepiece(connectFour, icolumnChoice, player1Gamepiece);
        turn=false;

        if(turn==false && rowOfFour(connectFour,player1Gamepiece)==false)
        {
            cout<<"Player 2: Choose your column\n";
            cin>>columnChoice;
            icolumnChoice=columnChoice-48;

            while(icolumnChoice!=1 && icolumnChoice!=2 && icolumnChoice!=3 && icolumnChoice!=4 &&
                    icolumnChoice!=5 && icolumnChoice!=6 && icolumnChoice!=7) //******need to fix this for error management somehow****
            {
                cout<<"You must choose a column number between 1 and 7. \nPlease try again\n";
                cin>>columnChoice;
                icolumnChoice=columnChoice-48;
                cout<<"icolumnChoice="<<icolumnChoice<<" columnChoice"<<columnChoice<<endl;
            }

            while(fullColumn(connectFour,icolumnChoice-1)==true)
            {
                cout<<"That column is full. Try another\n";
                cin>>columnChoice;
                icolumnChoice=columnChoice-48;
            }
        }
        insertGamepiece(connectFour, icolumnChoice, player2Gamepiece);
        turn=true;
    }

    if(rowOfFour(connectFour,player1Gamepiece)==true || rowOfFour(connectFour,player2Gamepiece)==true || fullGame(connectFour)==true)
    {
        if(rowOfFour(connectFour,player1Gamepiece)==true)
            cout<<"Player 1 wins the game!"<<endl;
        else if(rowOfFour(connectFour,player2Gamepiece)==true)
            cout<<"Player 2 wins the game!"<<endl;
        else
            cout<<"No one wins the game!"<<endl;
    }
    return 0;
}