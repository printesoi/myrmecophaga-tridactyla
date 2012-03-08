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

bool Location::operator == (Location param)
{
    return col == param.col && row == param.row;
}

Location Location::move(int dir) 
{
    Location rez(row + ROW_DIRECTION[dir], col + COLUMN_DIRECTION[dir]);
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
