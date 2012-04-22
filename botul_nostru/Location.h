#ifndef LOCATION_H_
#define LOCATION_H_

#include "global.h"
#include "Square.h"

class Location
{
    public:
        int row;
        int col;

        Location()
        {
            row = col = 0;
        }

        Location(int row, int col) : row(row), col(col)
    {
    }

        Location(Square s)
        {
            row = s.x;
            col = s.y;
        }

        bool operator == (Location);

        Location move(int dir);

        Location move(int x,int y);
};

#endif
