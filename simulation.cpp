#include <bits/stdc++.h> /*
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>*/
#include <unistd.h>
#include "pixel.h"
#include "simulation.h"

#define DEBUG if (0)
#define COUT cout << "\e[36m"
#define VAR(v) " [\e[32m" << #v << "\e[36m=\e[91m" << v << "\e[36m] "
#define ENDL "\e[39m" << endl

using namespace std;


///TESTING GITHUB

bool Simulation::checkIndexes(int x, int y)
{
    return ((x >= 0 && y >= 0 && x < rows && y < columns)? 1 : 0);
}

inline int Simulation::normV(int v)
{
    return ((v <= -1)? -1 : ((v >= 1)? 1 : 0));  ///so the velocity belongs to <-1, 1>
}

bool Simulation::shift(Pixel& curPix, int dvx, int dvy){DEBUG COUT << "PIXEL SHIFT" << VAR(curPix.x) << VAR(curPix.y) <<VAR(curPix.icon) << ENDL;
//curPix.resetMoved();
return false;
}/**/

bool Simulation::shift(Air& curPix, int dvx, int dvy)
{
    DEBUG COUT << "AIR SHIFT" << ENDL;
    return true;
}

bool Simulation::shift(Earth& curPix, int dvx, int dvy)
{
    DEBUG COUT << "EARTH SHIFT" << ENDL;
    return false;
}

bool Simulation::shift(Water& curPix, int dvx, int dvy) ///Thats anti-physics garbage
{
    int& x = curPix.x, y =curPix.y;
    if(curPix.moved)
    {
        return false;
    }
    if(dvx > 0 && curPix.vx < -1)
    {
        curPix.vx /= 2;
    }
    if(dvy > 0 && curPix.vy < -1)
    {
        curPix.vy /= 2;
    }
    curPix.moved = 1;
    DEBUG COUT << "WATERSHIFT" << ENDL;

    curPix.vx += dvx;
    curPix.vy += dvy;
    DEBUG COUT << "Shifting " << VAR(x) << VAR(y)  << VAR(curPix.vx) << VAR(curPix.vy) << ENDL;

    if(world[x+normV(curPix.vx)][y+normV(curPix.vy)]->moved == 0 &&  world[x+normV(curPix.vx)][y+normV(curPix.vy)]->dispatchShift(*this, curPix.vx+1, curPix.vy))
    {
        DEBUG COUT << "ACCUALLY SHIFTED" << VAR(x) << VAR(y) << VAR(curPix.vx) << VAR(curPix.vy) << ENDL;
        DEBUG print();
        int oldX = x, oldY = y; ///for not to swap on swapped indexes
        swap(world[oldX][oldY], world[oldX+normV(curPix.vx)][oldY+normV(curPix.vy)]);
        swap(world[oldX][oldY]->x, world[oldX+normV(curPix.vx)][oldY+normV(curPix.vy)]->x);
        swap(world[oldX][oldY]->y, world[oldX+normV(curPix.vx)][oldY+normV(curPix.vy)]->y);

        DEBUG print();
        DEBUG checkAllCoords();
        curPix.moved = 1;
        return true;
    }
    else
    {
        //if()

        int directionChangeX = 1;
        int directionChangeY = -1;
        if(rand() %2)
            swap(directionChangeX, directionChangeY);

        int prePushVx = curPix.vx;
        int prePushVy = curPix.vy;

        //curPix.vx -= normV(curPix.vx);
        //curPix.vy -= normV(curPix.vy);
        curPix.vx /= 3;
        curPix.vy /= 2;
        for(int i = 0; i < 4 && (abs(curPix.vx) > 0 || abs(curPix.vy) > 0); i++, swap(directionChangeX, directionChangeY)) ///i: 0, 1 redirecting at free; 2, 3 redirecting and pushing
        {
            if(i== 2)
            {
                //curPix.vx -= normV(curPix.vx);
                //curPix.vy -= normV(curPix.vy);
                curPix.vx /= 3;
                curPix.vy /= 3;

            }

            if(i < 2 && world[x+normV(curPix.vy*directionChangeX)][y+normV(curPix.vx*directionChangeY)]->free == 0)///at first try redirecting on a free box
            {
                continue;
            }
            if(world[x+normV(curPix.vy*directionChangeX)][y+normV(curPix.vx*directionChangeY)]->moved == 0 && world[x+normV(curPix.vy*directionChangeX)][y+normV(curPix.vx*directionChangeY)]->dispatchShift(*this, curPix.vy*directionChangeX+1, curPix.vx*directionChangeY))
            {
                DEBUG COUT << "ACCUALLY REDIRECTED AND SHIFTED" << VAR(x) << VAR(y) << VAR(curPix.vy*directionChangeX) << VAR(curPix.vx*directionChangeY) << ENDL;
                DEBUG print();


                if(checkIndexes(x+curPix.vy*directionChangeX, y+curPix.vx*directionChangeY) == 0 || checkIndexes(x, y) == 0)
                {
                    DEBUG COUT << VAR(x) << VAR(y) << VAR(x+curPix.vy*directionChangeX) << VAR(y+curPix.vx*directionChangeY) << ENDL;
                }
                int oldX = x, oldY = y;  ///for not to swap on swapped indexes
                swap(world[oldX][oldY], world[oldX+normV(curPix.vy*directionChangeX)][oldY+normV(curPix.vx*directionChangeY)]);
                swap(world[oldX][oldY]->x, world[oldX+normV(curPix.vy*directionChangeX)][oldY+normV(curPix.vx*directionChangeY)]->x);
                swap(world[oldX][oldY]->y, world[oldX+normV(curPix.vy*directionChangeX)][oldY+normV(curPix.vx*directionChangeY)]->y);
                curPix.vx = curPix.vy*directionChangeX;
                curPix.vy = curPix.vx*directionChangeY;
                DEBUG print();
                DEBUG checkAllCoords();
                curPix.moved = 1;
                return true;
            }

        }
        curPix.vx = prePushVx;
        curPix.vy = prePushVy;

        if(abs(curPix.vx) > 3) curPix.vx /= 2;
        if(abs(curPix.vy) > 1) curPix.vy /= 2;

        curPix.moved = 0;
    }
    return false;
}


void Simulation::clearMoved()
{
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < columns; ++j)
        {
            world[i][j]->resetMoved(0);
        }
    }
}

bool Simulation::checkAllCoords()
{
    bool ret = 0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            if(world[i][j]->x != i || world[i][j]->y != j)
            {
                DEBUG COUT << "i, j = " << i << ", " << j << " | x, y = " << world[i][j]->x << ", " << world[i][j]->y << ENDL;
                ret = 1;
            }
        }
    }
    return ret;
}

void Simulation::calculate()
{
    clearMoved();
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < columns; ++j)
        {
            if(i < rows-1 && world[i+1][j]->solid == 0) ///free fall
            {
                world[i][j]->dispatchShift(*this, 1, 0);
            }
            else if(i < rows-1 && j > 0 && world[i+1][j-1]->free && world[i][j-1]->free) ///slipping from the edge
            {
                world[i][j]->dispatchShift(*this, 0, -1);
            }
            else if(i < rows-1 && j < columns-1 && world[i+1][j+1]->free && world[i][j+1]->free) ///slipping from the edge
            {
                world[i][j]->dispatchShift(*this, 0, 1);
            }
            else ///resting on the ground
            {
                world[i][j]->dispatchShift(*this, 0, 0);
            }
            //print();
            DEBUG COUT << ENDL;
        }
    }
    DEBUG COUT << "CALC DONE" << ENDL << ENDL;
}


void Simulation::print()//█
{
    for(int i = 0; i < rows; ++i)
    {
        DEBUG cout << i;
        for(int j = 0; j < columns; ++j)
        {

            cout << world[i][j]->icon;
        }
        cout << endl;
    }
}

void Simulation::simulate(int timeLimit)
{
    system("clear");
    print();
    for(int t = 0; t < timeLimit; ++t)
    {
        calculate();
        usleep(150000);                  ///CYCLE TIME
        system("clear");
        print();
        checkAllCoords();
        //usleep(3000000);
    }
}

void Simulation::input()
{
    cin >> rows >> columns;

    world.resize(rows);
    for(vector<unique_ptr<Pixel>>& el : world)
        el.resize(columns);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < columns; ++j)
        {
            char ch = 0;
            cin >> ch;
            if(ch == 'W')
            {
                world[i][j] = make_unique<Water>(i, j);
            }
            else if(ch == 'E')
            {
                world[i][j] = make_unique<Earth>(i, j);
            }
            else if(ch == '.')
            {
                world[i][j] = make_unique<Air>(i, j);
            }
        }
    }
    DEBUG COUT << "INPUT DONE" << VAR(columns) << VAR(rows)<< ENDL;
}

Simulation::Simulation(int seed /*= 1*/)
{
    srand(seed);
}

