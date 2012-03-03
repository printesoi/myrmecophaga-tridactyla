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
