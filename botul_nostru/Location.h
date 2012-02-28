#include "global.h"

struct Location
{
    int row;
    int col;

	Location() : row(0), col(0) { }
    Location(int row, int col) : row(row), col(col) { }

    Location move(int dir);
};
