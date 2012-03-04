/*
 * =============================================================================
 *
 *       Filename:  Location.cpp
 *        Authors:  Dodon Victor <dodonvictor at gmail dot com>
 *                  Cernov Vladimir <gg.kaspersky@gmail.com>
 *                  Maldur Vitalie <maldur.vitalik@yahoo.com>
 *                  Straticiuc Vicu <straticiuc_vicu@yahoo.com>
 *        Created:  02/25/2012
 *    Description:  
 *
 * =============================================================================
 */

#include "Location.h"
/** Operator for sort utility */
bool Location::operator == (Location param)
{
    return col == param.col && row == param.row;
}

/** Structure for representing a location on the map. */
Location Location::move(int dir) 
{
    Location returnValue(row + ROW_DIRECTION[dir], col + COLUMN_DIRECTION[dir]);
    
    if (returnValue.row < 0) 
    {
        returnValue.row += gparam::mapRows;
    } 
    else if (returnValue.row == gparam::mapRows)
    {
        returnValue.row = 0;
    }

    if (returnValue.col < 0)
    {
        returnValue.col += gparam::mapColumns;
    } 
    else if (returnValue.col == gparam::mapColumns)
    {
        returnValue.col = 0;
    }

    return returnValue;
}

Location Location::move(int x,int y)
{
	Location rez(row + x, col + y);
	if (rez.row < 0)
		rez.row += gparam::mapRows;
	else
		if (rez.row == gparam::mapRows)
			rez.row = 0;
    if (rez.col < 0)
        rez.col += gparam::mapColumns;
    else
		if (rez.col == gparam::mapColumns)
			rez.col = 0;
    return rez;
}
