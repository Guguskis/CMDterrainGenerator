/*Terrain generator in a cmd by Matas*/
/*NOTE: there's a mystic tendency to decrease in height by going south and east*/
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
using namespace std;

const int gridSize = 20;    // a square grid size X by Y, 39 is maximum value
const int numOfPeaks = 15;   // number of peaks on which calculations are based, increasing this increases the overall height
const int maxPeakSize = 9;  // maximum size of peak, don't go over 9.... thousand!! But seriously, 9 or less
const int minPeakSize = 4;  // minimum peak size, don't make it higher than max
const int scalingMax = 2;   // maximum variation between cells, lower seems to make everything smoother
const int smoothFactor = 3; // increase to even out the terrain
const bool TrueforTilesFalseforNumbers = true; // set the display between numbers and tiles
struct str{int coord, color; bool taken, originalTaken;} coord[gridSize+2][gridSize+2];

void Initialization();
void Draw();
void TerrainGeneration();
void colorAssigment();
void setcolor(unsigned short color);
void colorTEST();
int main()
{
    Initialization();
  /*  Draw(); // function to draw original board with peak only values
    cout << "^^^^^^^^^^^^^^^^^^^^^\n";
    cout << "seeded random heights\n\n\n";
  */  TerrainGeneration(); // all of the logic
    Draw(); // drawing again with generated terrain
  /*  cout << "^^^^^^^^^^^^^^^^^^^^^\n";
    cout << "generated the terrain\n\n\n";
    colorTEST();
  */  return 0;
}
void Initialization()
{
    for(int y=0; y<=gridSize+1; y++) // y
    {
        for(int x=0; x<=gridSize+1; x++) // x
        {
            coord[x][y].coord=0;
            coord[x][y].taken=false;
            coord[x][y].originalTaken=false;
        }
    }
    int _numOfPeaks = numOfPeaks;
    srand(time(NULL)); // implementing random generated numbers with time seed
    // replace 'time(NULL)' with '1' for testing different variables

    while(_numOfPeaks>0 && numOfPeaks-(gridSize*gridSize)!=_numOfPeaks) // stopping the loop if number of peaks reaches maximum number of cells
    {
        int randX = rand()%(gridSize)+1;
        int randY = rand()%(gridSize)+1;
        if(!coord[randX][randY].taken)
        {
            coord[randX][randY].coord = rand()%(maxPeakSize-minPeakSize)+1+minPeakSize;
            coord[randX][randY].originalTaken = true;
            coord[randX][randY].taken = true;
            _numOfPeaks--;
        }
    }

}
void Draw()
{
    colorAssigment();
    for(int y=1; y<=gridSize; y++) // y
    {
        for(int x=1; x<=gridSize; x++) // x
        {
            setcolor(coord[x][y].color);
            cout << coord[x][y].coord << " ";
        }
        cout << endl;
    }
    cout << endl;
    setcolor(7);
}
void TerrainGeneration()
{
    for(int n=0; n<gridSize; n++) // do some iterations of the code to fill all the tiles
    {
        for(int y=1; y<=gridSize; y++)
        {
            for(int x=1; x<=gridSize; x++)
            {
                int gridAvg=0, numOfTaken=0; // initialize some variables to calculate the AVG of (x;y) and 1 square around

                for(int y2=y-1; y2<=y+1; y2++)
                {
                    for(int x2=x-1; x2<=x+1; x2++)
                    {
                        if(coord[x2][y2].taken) // take into account only the changed coordinates, so empty coordinates don't affect AVG
                        {
                            gridAvg+=coord[x2][y2].coord;
                            numOfTaken++;
                        }
                    }
                }
                if(numOfTaken>0 && !coord[x][y].taken)
                {
                    gridAvg/=numOfTaken;
                    int expectedHeight = gridAvg+(rand()%2-1*rand()%scalingMax); // making expected height so we can then alter it until it's in range from 0 to 9
                    while(expectedHeight<0 || expectedHeight>maxPeakSize)
                    {
                        int sign; // variable to implement increase AND DECREASE of height
                        sign=rand()%2;
                        if(sign==0) expectedHeight = gridAvg-rand()%scalingMax; // if negative
                        else expectedHeight = gridAvg+rand()%scalingMax; // if positive
                    }
                    coord[x][y].coord=expectedHeight;
                    coord[x][y].taken=true; // after all of calculations are done, we can finally mark this tile as calculated, i.e. taken
                }
            }
        }
    }
    for(int y=1; y<=gridSize; y++) // all of these loops are to soften original peak values so they don't vary wildly with nearby cells
    {
        for(int x=1; x<=gridSize; x++)
        {
            if(coord[x][y].originalTaken)
            {
                int gridAvg=0, numOfTaken=0;

                for(int y2=y-1; y2<=y+1; y2++)
                {
                    for(int x2=x-1; x2<=x+1; x2++)
                    {
                        if(coord[x2][y2].taken)
                        {
                            gridAvg+=coord[x2][y2].coord;
                            numOfTaken++;
                        }
                    }
                }
                coord[x][y].coord=gridAvg/numOfTaken;
            }
            //-------------------
            // This loop tries to smoothen and even out the terrain
            int gridAvg=0, numOfTaken=0;
            for(int y2=y-1; y2<=y+1; y2++)
            {
                for(int x2=x-1; x2<=x+1; x2++)
                {
                    if(coord[x2][y2].taken && x!=x2 && y!=y2)
                    {
                        gridAvg+=coord[x2][y2].coord;
                        numOfTaken++;
                    }
                }
            }
            gridAvg/=numOfTaken;
            if(coord[x][y].coord>gridAvg-smoothFactor && coord[x][y].coord<gridAvg+smoothFactor) coord[x][y].coord=gridAvg;
            //--------------------
        }
    }
}
void colorAssigment()
{
    if(TrueforTilesFalseforNumbers)
    {
        for(int y=1; y<=gridSize; y++)
        {
            for(int x=1; x<=gridSize; x++)
            {
                if(coord[x][y].coord==9) coord[x][y].color = 204;        // 68 dark red
                else if(coord[x][y].coord==8) coord[x][y].color = 238;  // 204 red
                else if(coord[x][y].coord==7) coord[x][y].color = 238;  // 238 yellow
                else if(coord[x][y].coord==6) coord[x][y].color = 170;  // 170 light green
                else if(coord[x][y].coord==5) coord[x][y].color = 170;   // 34 dark green
                else if(coord[x][y].coord==4) coord[x][y].color = 34;   // 34 dark green
                else if(coord[x][y].coord==3) coord[x][y].color = 34;  // 187 cyan
                else if(coord[x][y].coord==2) coord[x][y].color = 153;   // 51 dark cyan
                else if(coord[x][y].coord==1) coord[x][y].color = 153;  // 153 blue
                else if(coord[x][y].coord==0) coord[x][y].color = 17;   // 17 dark blue
            }
        }
    }
    else
    {
        for(int y=1; y<=gridSize; y++)
        {
            for(int x=1; x<=gridSize; x++)
            {
                if(coord[x][y].coord==9) coord[x][y].color = 4;         // 4 dark red
                else if(coord[x][y].coord==8) coord[x][y].color = 12;   // 12 red
                else if(coord[x][y].coord==7) coord[x][y].color = 14;   // 14 yellow
                else if(coord[x][y].coord==6) coord[x][y].color = 10;   // 10 light green
                else if(coord[x][y].coord==5) coord[x][y].color = 10;    // 2 dark green
                else if(coord[x][y].coord==4) coord[x][y].color = 2;    // 2 dark green
                else if(coord[x][y].coord==3) coord[x][y].color = 2;   // 11 cyan
                else if(coord[x][y].coord==2) coord[x][y].color = 9;    // 3 dark cyan
                else if(coord[x][y].coord==1) coord[x][y].color = 9;    // 9 blue
                else if(coord[x][y].coord==0) coord[x][y].color = 1;    // 1 dark blue
            }
        }
    }

}
void setcolor(unsigned short color)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon, color);
}
void colorTEST()
{
    setcolor(4);
    cout << "9";
    setcolor(12);
    cout << "8";
    setcolor(14);
    cout << "7";
    setcolor(10);
    cout << "6";
    setcolor(2);
    cout << "5";
    setcolor(2);
    cout << "4";
    setcolor(11);
    cout << "3";
    setcolor(3);
    cout << "2";
    setcolor(9);
    cout << "1";
    setcolor(1);
    cout << "0";
    cout << "\n";
    for(int i=0; i<=256; i++)
    {
        setcolor(i);
        cout << i << " ";
    }
/*
9 4  68
8 12 204
7 14 238
6 10 170
5 2  34
4 2  34
3 11 187
2 3  51
1 9  153
0 7  17
*/
}
