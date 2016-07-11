#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "cmd.h"

using namespace std;

char map[255][255];
Map map1;
Player player1;
int highscore,direction,key;
int X,Y;
bool gotCoin=false;
bool dontPauseThisTime=false;

void printMenu();

string encryptDecrypt(string toEncrypt)///Encrypts/Decrypts highscore file
{
    char key[5]={'S','N','A','K','E'};
    string output = toEncrypt;
    for (int i=0;i<toEncrypt.size();i++)
        output[i]=toEncrypt[i]^key[i%(sizeof(key)/sizeof(char))];
    return output;
}

void clearscreen()///Sets the console cursor position to (0,0)
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void addPlayer()///Ads player to map
{
    for(int i=1;i<=player1.lenght;i++)
        if(i==1) map1.addPlayerHeadToMap(player1.chain[i].playerX,player1.chain[i].playerY);
        else map1.addPlayerToMap(player1.chain[i].playerX,player1.chain[i].playerY);
}

void loadHighScore()///Reads highscore file
{
    string encryptedString;
    int encryptedLenght;
    ifstream scoreIn("high_score.snk");
    scoreIn>>encryptedString;
    scoreIn>>encryptedLenght;
    player1.name=encryptDecrypt(encryptedString);
    highscore=(encryptedLenght-313)/187;
    scoreIn.close();
}

bool itIsNotAvailable()///Checks if (X,Y) position is empty
{
    if(map1.checkAvailability(X,Y)) return false;
    return true;
}

void generateCoinCoords()///Ads coin to a random generated empty position
{
    do
    {
        srand(time(NULL));
        int rdm=rand()%mapY+1;
        if(rdm==1) rdm++;
        else if(rdm==mapX) rdm--;
        X=rdm%mapX;
        Y=rdm;
    }while(itIsNotAvailable());
    map1.addCoin(X,Y);
}

void initialLoad()///Loads highscore from file, generates map, adds snake to map and generates first coin
{
    loadHighScore();
    map1.generateMap();
    addPlayer();
    generateCoinCoords();
}

void movePlayer(int x,int y)///Moves snake "head"
{
    if(!gotCoin)
        map1.clearPlayerPosition(player1.chain[player1.lenght].playerX,player1.chain[player1.lenght].playerY);
    map1.addPlayerToMap(player1.chain[1].playerX,player1.chain[1].playerY);
    player1.modifyCoords();
    map1.addPlayerHeadToMap(x,y);
    player1.chain[1].playerX=x;
    player1.chain[1].playerY=y;
    gotCoin=false;
}

void moveToDirection()///Moves the snake to the current direction
{
    switch(direction)
    {
        case upArrowKey:
            if(map1.isNextMoveACoin(player1.chain[1].playerX-1,player1.chain[1].playerY))
            {
                player1.addChain();
                generateCoinCoords();
                gotCoin=true;
                dontPauseThisTime=true;
                cmd_change_color();
            }
            if(!map1.isMovePossible(player1.chain[1].playerX-1,player1.chain[1].playerY))
                player1.dies();
            else
                movePlayer(player1.chain[1].playerX-1,player1.chain[1].playerY);
            break;
        case downArrowKey:
            if(map1.isNextMoveACoin(player1.chain[1].playerX+1,player1.chain[1].playerY))
            {
                player1.addChain();
                generateCoinCoords();
                gotCoin=true;
                dontPauseThisTime=true;
                cmd_change_color();
            }
            if(!map1.isMovePossible(player1.chain[1].playerX+1,player1.chain[1].playerY))
                player1.dies();
            else
                movePlayer(player1.chain[1].playerX+1,player1.chain[1].playerY);
            break;
        case leftArrowKey:
            if(map1.isNextMoveACoin(player1.chain[1].playerX,player1.chain[1].playerY-1))
            {
                player1.addChain();
                generateCoinCoords();
                gotCoin=true;
                dontPauseThisTime=true;
                cmd_change_color();
            }
            if(!map1.isMovePossible(player1.chain[1].playerX,player1.chain[1].playerY-1))
                player1.dies();
            else
                movePlayer(player1.chain[1].playerX,player1.chain[1].playerY-1);
            break;
        case rightArrowKey:
            if(map1.isNextMoveACoin(player1.chain[1].playerX,player1.chain[1].playerY+1))
            {
                player1.addChain();
                generateCoinCoords();
                gotCoin=true;
                dontPauseThisTime=true;
                cmd_change_color();
            }
            if(!map1.isMovePossible(player1.chain[1].playerX,player1.chain[1].playerY+1))
                player1.dies();
            else
                movePlayer(player1.chain[1].playerX,player1.chain[1].playerY+1);
            break;
        case escapeKey:
            player1.quits();
            break;
    }
}

void printScore()///Prints the highscore and current score
{
    clearscreen();
    cout<<"HIGHSCORE="<<highscore<<" (by "<<player1.name<<")"<<'\n';
    cout<<"LENGHT="<<player1.lenght<<'\n';
}

void printEndMessage()///Prints a message when you die or quit
{
    if(!player1.hasPlayerQuit)
        cout<<"YOU LOST! PRESS ANY KEY TO CONTINUE...\n",Sleep(1000),getch();
    else
        cout<<"YOU QUIT. PRESS ANY KEY TO CONTINUE...\n",Sleep(1000),getch();
}

void initialPrint()///Initially prints the map and waits until the player pressed an arrow key or escape
{
    printScore();
    map1.drawMap();
    while(key!=rightArrowKey&&key!=upArrowKey&&key!=downArrowKey&&key!=escapeKey)
        key=getch();
    if(key==escapeKey)
        player1.quits();
    else
        direction=key;
}

void checkIfDirectionChanged()///Checks if player pressed an arrow key and changes the direction the snakes moves to
{
    if(kbhit())
    {
        key=getch();
        dontPauseThisTime=true;
        if(!(key==upArrowKey&&direction==downArrowKey)&&!(key==downArrowKey&&direction==upArrowKey)&&!(key==leftArrowKey&&direction==rightArrowKey)&&!(key==rightArrowKey&&direction==leftArrowKey))
            direction=key;
    }
}

void newHighscore()///Prints new highscore message and
{
    cout<<"CONGRATULATIONS! YOU SET A NEW HIGHSCORE!\n";
    cout<<"PLEASE INPUT YOUR NAME...\n";
    cin>>player1.name;
    ofstream scoreOut("high_score.snk");
    if(player1.name[0]==NULL)
        scoreOut<<"*NO NAME*"<<'\n';
    else
        scoreOut<<encryptDecrypt(player1.name)<<'\n';
    int encryptedLenght=(player1.lenght*187)+313;
    scoreOut<<encryptedLenght;
    scoreOut.close();
}

void pause(int pauseTime)///Checks if the game has to pause
{
    if(!dontPauseThisTime)
            Sleep(pauseTime);
        dontPauseThisTime=false;
}

void startGame()///Starts the game
{
    system("cls");
    initialPrint();
    while(player1.isPlayerAlive&&!player1.hasPlayerQuit)
    {
        printScore();
        map1.drawMap();
        checkIfDirectionChanged();
        moveToDirection();
        pause(player1.pauseTime);
    }
    if(player1.lenght>highscore)
        newHighscore();
    else
        printEndMessage();
    printMenu();
}

void helpMenu()///Prints the help menu
{
    system("cls");
    cout<<"     .---- |   | .---. |   / .----"<<'\n';
    cout<<"     |     |   | |   | |  /  |    "<<'\n';
    cout<<"     |     |\\  | |   | | /   |    "<<'\n';
    cout<<"     ----- | \\ | |---| |<    |----"<<'\n';
    cout<<"         | |  \\| |   | | \\   |    "<<'\n';
    cout<<"         | |   | |   | |  \\  |    "<<'\n';
    cout<<"     ----. |   | |   | |   \\ .----"<<'\n';
    cout<<"                        by Adrian Gozob"<<"\n\n\n";
    cout<<"This is a remake of the Snake video game,"<<'\n';
    cout<<"  coded in C++ using standard libraries."<<"\n\n";
    cout<<"             INSTRUCTIONS              "<<'\n';
    cout<<"---------------------------------------"<<'\n';
    cout<<"   Your goal is to eat hearths("<<(char)3<<") and"<<'\n';
    cout<<"   get longer without biting yourself  "<<'\n';
    cout<<"     or hitting the walls (| and -).   "<<"\n\n";
    cout<<"                CONTROLS               "<<'\n';
    cout<<"---------------------------------------"<<'\n';
    cout<<"    Use the arrow keys to change the   "<<'\n';
    cout<<"movement direction of the snake. Press "<<'\n';
    cout<<"\"Escape\" to quit the game or any other"<<'\n';
    cout<<"        key to pause the game.          "<<'\n';
    getch();
    printMenu();
}

void printMenu()
{
    system("color 0a");
    int option=1;
    do
    {
        key=0;
        system("cls");
        cout<<"     .---- |   | .---. |   / .----"<<'\n';
        cout<<"     |     |   | |   | |  /  |    "<<'\n';
        cout<<"     |     |\\  | |   | | /   |    "<<'\n';
        cout<<"     ----- | \\ | |---| |<    |----"<<'\n';
        cout<<"         | |  \\| |   | | \\   |    "<<'\n';
        cout<<"         | |   | |   | |  \\  |    "<<'\n';
        cout<<"     ----. |   | |   | |   \\ .----"<<'\n';
        cout<<"                        by Adrian Gozob"<<"\n\n\n";
        if(option==1)
        {
            cout<<"                ||PLAY||               "<<"\n\n";
            cout<<"               DIFFICULTY              "<<"\n\n";
            cout<<"                  HELP                 "<<"\n\n";
            cout<<"                  QUIT                 "<<"\n\n";
        }
        else if(option==2)
        {
            cout<<"                  PLAY                 "<<"\n\n";
            cout<<"             ||DIFFICULTY||            "<<"\n\n";
            cout<<"                  HELP                 "<<"\n\n";
            cout<<"                  QUIT                 "<<"\n\n";
        }
        else if(option==3)
        {
            cout<<"                  PLAY                 "<<"\n\n";
            cout<<"               DIFFICULTY              "<<"\n\n";
            cout<<"                ||HELP||               "<<"\n\n";
            cout<<"                  QUIT                 "<<"\n\n";
        }
        else if(option==4)
        {
            cout<<"                  PLAY                 "<<"\n\n";
            cout<<"               DIFFICULTY              "<<"\n\n";
            cout<<"                  HELP                 "<<"\n\n";
            cout<<"                ||QUIT||               "<<"\n\n";
        }
        while(key!=enterKey&&key!=upArrowKey&&key!=downArrowKey&&key!=escapeKey)
            key=getch();
        if(key==downArrowKey&&option<4)
            option++;
        else if(key==upArrowKey&&option>1)
            option--;
    }while(key!=enterKey&&key!=escapeKey);
    if(key==escapeKey) option=0;
    if(option==1)
    {
        player1.resetPlayer();
        initialLoad();
        startGame();
    }
    else if(option==2)
        printMenu();
    else if(option==3)
        helpMenu();
    else if(option==4)
        player1.quits();
}

int main()
{
    printMenu();
    return 0;
}

///TO DO LIST
///1.DIFFICULTY LEVEL
///2.FIX THE OPPOSITE DIRECTION BUG
