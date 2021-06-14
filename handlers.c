// TODO: remove fucking strtok from here

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// handleAdd handles the 'add X Y' command
const char *handleAdd(char *addCmd, struct Locations *loc)
{
    int X, Y;

    char *coordX = strtok(NULL, " ");
    X = parseInt(coordX);
    if (X < 0 || X > 9999)
        return "error";

    char *coordY = strtok(NULL, " ");
    Y = parseInt(coordY);
    if (Y < 0 || Y > 9999)
        return "error";

    char *end = strtok(NULL, " "); // additional invalid parts in add command
    if (end != NULL)
        return "error";

    if (loc->currOccupancy >= 50)
        return "limit exceeded";

    if (elementAlreadyRegistered(X, Y, loc) != -1)
        return writeReturnMessage(coordX, coordY, "already exists");

    loc->points[loc->currOccupancy].X = X;
    loc->points[loc->currOccupancy].Y = Y;
    loc->currOccupancy++;

    return writeReturnMessage(coordX, coordY, "added");
}

// handleList handles the 'list' command
const char *handleList(char *addCmd, struct Locations *loc)
{
    char *end = strtok(NULL, " "); // additional invalid parts in list command
    if (end != NULL)
        return "error";

    if (loc->currOccupancy == 0)
        return "none";

    char *list = malloc((3 * loc->currOccupancy) + 1);
    char pointX[5], pointY[5]; // array size defined by: `(int)((ceil(log10(9999))+1)*sizeof(char));`
    list[0] = '\0';

    for (int i = 0; i < loc->currOccupancy; i++)
    {
        sprintf(pointX, "%d", loc->points[i].X);
        sprintf(pointY, "%d", loc->points[i].Y);
        strcat(list, pointX);
        strcat(list, " ");
        strcat(list, pointY);
        if (i < loc->currOccupancy - 1)
            strcat(list, " ");
    }

    return list;
}

// handleRemove handles the 'rm X Y' command
const char *handleRemove(char *addCmd, struct Locations *loc)
{
    int X, Y;

    char *coordX = strtok(NULL, " ");
    X = parseInt(coordX);
    if (X < 0 || X > 9999)
        return "error";

    char *coordY = strtok(NULL, " ");
    Y = parseInt(coordY);
    if (Y < 0 || Y > 9999)
        return "error";

    char *end = strtok(NULL, " "); // additional invalid parts in add command
    if (end != NULL)
        return "error";

    int elemIndex = elementAlreadyRegistered(X, Y, loc);
    if (elemIndex == -1)
        return writeReturnMessage(coordX, coordY, "does not exist");

    for (int i = elemIndex; i < loc->currOccupancy - 1; i++)
    { // removes the element and "brings forward" all elements after it
        loc->points[i].X = loc->points[i + 1].X;
        loc->points[i].Y = loc->points[i + 1].Y;
    }
    // make the last element invalid (as it was removed)
    loc->points[loc->currOccupancy - 1].X = -1;
    loc->points[loc->currOccupancy - 1].Y = -1;

    // updated current occupancy
    loc->currOccupancy--;
    return writeReturnMessage(coordX, coordY, "removed");
}

// handleQuery handles the 'query X Y' command
const char *handleQuery(char *addCmd, struct Locations *loc)
{
    int X, Y;

    char *coordX = strtok(NULL, " ");
    X = parseInt(coordX);
    if (X < 0 || X > 9999)
        return "error";

    char *coordY = strtok(NULL, " ");
    Y = parseInt(coordY);
    if (Y < 0 || Y > 9999)
        return "error";

    char *end = strtok(NULL, " "); // additional invalid parts in add command
    if (end != NULL)
        return "error";

    if (loc->currOccupancy == 0)
        return "none";

    float minDistance = 14141.0; // bigger than max dist possible (from 0,0 to 9999,9999)
    float currDist;
    int minX, minY;
    for (int i = 0; i < loc->currOccupancy; i++)
    {
        currDist = distance(X, Y, loc->points[i].X, loc->points[i].Y);
        if (currDist < minDistance)
        {
            minDistance = currDist;
            minX = loc->points[i].X;
            minY = loc->points[i].Y;
        }
    }

    char *minDistCoord = malloc(4);
    char pointX[5], pointY[5]; // array size defined by: `(int)((ceil(log10(9999))+1)*sizeof(char));`
    minDistCoord[0] = '\0';
    sprintf(pointX, "%d", minX);
    sprintf(pointY, "%d", minY);
    strcat(minDistCoord, pointX);
    strcat(minDistCoord, " ");
    strcat(minDistCoord, pointY);

    return minDistCoord;
}

const char *handleCommand(char *msg, struct Locations *loc)
{
    const char *cmdReturn;

    if (strstr(msg, "add") != NULL)
    { // add X Y
        cmdReturn = handleAdd(msg, loc);
    }
    else if (strstr(msg, "rm") != NULL)
    { // rm X y
        cmdReturn = handleRemove(msg, loc);
    }
    else if (strstr(msg, "list") != NULL)
    { // list
        cmdReturn = handleList(msg, loc);
    }
    else if (strstr(msg, "query") != NULL)
    { // query X Y
        cmdReturn = handleQuery(msg, loc);
    }
    else
    {
        return "error";
    }

    return cmdReturn;
}