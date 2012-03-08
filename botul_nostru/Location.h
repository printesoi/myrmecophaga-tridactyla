#include "global.h"

class Location
{

public:
	int row;
	int col;
	int f;
	int g;
	int dir;
    
	Location() : row(0), col(0) { }
    Location(int row, int col) : row(row), col(col) { }
    
	bool operator == (Location);

    Location move(int dir);

    Location move(int x,int y);
};
