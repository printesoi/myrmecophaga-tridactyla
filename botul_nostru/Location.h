#ifndef LOCATION_H_
#define LOCATION_H_

#include "global.h"

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
    
	bool operator < (Location);
	bool operator > (Location);
	bool operator == (Location);

    Location move(int dir);

    Location move(int x,int y);
};

#endif
