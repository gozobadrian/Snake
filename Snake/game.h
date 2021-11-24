#include <iostream>
#include <fstream>
#include <Windows.h>

#define upArrowKey 72
#define downArrowKey 80
#define leftArrowKey 75
#define rightArrowKey 77
#define escapeKey 27
#define enterKey 13

using namespace std;

int mapX,mapY;

class Map
{
    public:
    char map[101][101];
    void generateMap() // Initialise map
    {
        for (int i = 1; i <= mapX; i++)
        {
            for (int j = 1; j <= mapY; j++)
            {
                if (i == 1 || i == mapX)
                {
                    map[i][j] = '-';
                }
                else if (j == 1 || j == mapY)
                {
                    map[i][j] = '|';
                }
                else
                {
                    map[i][j] = '_';
                }
            }
        }
    }

    void drawMap() // Prints the map
    {
        for (int i = 1; i <= mapX; i++)
        {
            for (int j = 1; j <= mapY; j++)
            {
                if (map[i][j] == '_')
                {
                    cout << " ";
                }
                else
                {
                    cout << map[i][j];
                }
            }
            cout << '\n';
        }
    }

    void addPlayerToMap(int x, int y) // Adds an 'O' to the map on (x,y) position, i.e. a part of the snake's "body"
    {
        map[x][y] = 'O';
    }

    void addPlayerHeadToMap(int x, int y) // Adds a '0' to the map on (x,y) position, i.e. the snake's "head"
    {
        map[x][y] = '0';
    }

    void clearPlayerPosition(int x, int y) // Clears position (x,y) ('_' is considered an empty space on the map)
    {
        map[x][y] = '_';
    }

    bool isMovePossible(int x, int y) // Checks if moving to (x,y) is possible (i.e. when the "head" moves to an empty space - '_' - or a coin - the heart symbol)
    {
        if (map[x][y] == '_' || map[x][y] == 3) return true;
        return false;
    }

    bool isNextMoveACoin(int x, int y) // Checks if you are about to get a coin if you move to position (x,y)
    {
        if (map[x][y] == 3) return true;
        return false;
    }

    bool checkAvailability(int x, int y) // Checks if the (x,y) position is empty ('_') - this is used to add coins to the map
    {
        if (map[x][y] == '_') return true;
        return false;
    }

    void addCoin(int x, int y) // Adds a coin to the map on position (x,y)
    {
        map[x][y] = 3;
    }

    Map() // Constructor initialising the map width and height
    {
        mapX = 20;
        mapY = 50;
    }
};

class Player
{
    public:
    struct coords
    {
        int playerX, playerY;
    };
    coords chain[300];
    bool isPlayerAlive;
    bool hasPlayerQuit;
    int lenght;
    int pauseTime;
    string name;

    void dies() // "Kills" player
    {
        isPlayerAlive = false;
    }

    void quits() // Player quits (if Esc key is pressed)
    {
        hasPlayerQuit = true;
    }

    void modifyCoords() // Moves every snake "chain" forward
    {
        for (int i = lenght; i >= 2; i--)
        {
            chain[i].playerX = chain[i - 1].playerX;
            chain[i].playerY = chain[i - 1].playerY;
        }
    }

    void addChain() // Adds one more chain to the snake
    {
        lenght++;
    }

    void resetPlayer() // Reinitialises the snake lenght, coordinates and other stats
    {
        lenght = 4;
        int startX = mapX / 2;
        int startY = mapY / 2 + lenght / 2;
        for (int i = 1; i <= lenght; i++)
        {
            chain[i].playerX = startX;
            chain[i].playerY = startY - i + 1;
        }
        isPlayerAlive = true;
        hasPlayerQuit = false;
        pauseTime = 50;
    }

    Player() // Constructor that initialises snake stats
    {
        lenght = 4;
        int startX = mapX / 2;
        int startY = mapY / 2 + lenght / 2;
        for (int i = 1; i <= lenght; i++)
        {
            chain[i].playerX = startX;
            chain[i].playerY = startY - i + 1;
        }
        isPlayerAlive = true;
        hasPlayerQuit = false;
        pauseTime = 50;
    }
};
