// TODO: number starting with 0
#include "handlers.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// handleAdd handles the 'add X Y' command
void handleAdd(char *addCmd, struct Locations *loc, char *cmdReturn)
{
    int X, Y;
    char coordX[5], coordY[5];

    if (getCoordinates(addCmd, coordX, coordY, "add"))
    {
        X = parseInt(coordX);
        if (X < 0 || X > 9999)
        {
            sprintf(cmdReturn, "%s", "error");
            return;
        }

        Y = parseInt(coordY);
        if (Y < 0 || Y > 9999)
        {
            sprintf(cmdReturn, "%s", "error");
            return;
        }

        if (loc->currOccupancy >= 50)
        {
            sprintf(cmdReturn, "%s", "limit exceeded");
            return;
        }

        if (elementAlreadyRegistered(X, Y, loc) != -1)
        {
            writeReturnMessage(coordX, coordY, "already exists", cmdReturn);
            return;
        }

        loc->points[loc->currOccupancy].X = X;
        loc->points[loc->currOccupancy].Y = Y;
        loc->currOccupancy++;

        writeReturnMessage(coordX, coordY, "added", cmdReturn);
        return;
    }
    sprintf(cmdReturn, "%s", "error");
    return;
}

// handleList handles the 'list' command
void handleList(char *listCmd, struct Locations *loc, char *cmdReturn)
{
    if (strlen(listCmd) < 5 ||
        (strlen(listCmd) == 5 && listCmd[strlen(listCmd) - 1] == '\n'))
    {
        if (loc->currOccupancy == 0)
        {
            sprintf(cmdReturn, "%s", "none");
            return;
        }

        char list[(9 * loc->currOccupancy) + (loc->currOccupancy - 1) + 1];
        char pointX[5], pointY[5]; // array size defined by: `(int)((ceil(log10(9999))+1)*sizeof(char));`
        bzero(list, strlen(list));

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

        sprintf(cmdReturn, "%s", list);
        return;
    }

    sprintf(cmdReturn, "%s", "error");
}

// handleRemove handles the 'rm X Y' command
void handleRemove(char *rmCmd, struct Locations *loc, char *cmdReturn)
{
    int X, Y;
    char coordX[5], coordY[5];

    if (getCoordinates(rmCmd, coordX, coordY, "rm"))
    {
        X = parseInt(coordX);
        if (X < 0 || X > 9999)
        {
            sprintf(cmdReturn, "%s", "error");
            return;
        }

        Y = parseInt(coordY);
        if (Y < 0 || Y > 9999)
        {
            sprintf(cmdReturn, "%s", "error");
            return;
        }

        int elemIndex = elementAlreadyRegistered(X, Y, loc);
        if (elemIndex == -1)
        {
            writeReturnMessage(coordX, coordY, "does not exist", cmdReturn);
            return;
        }

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

        writeReturnMessage(coordX, coordY, "removed", cmdReturn);
        return;
    }

    sprintf(cmdReturn, "%s", "error");
}

// handleQuery handles the 'query X Y' command
void handleQuery(char *queryCmd, struct Locations *loc, char *cmdReturn)
{
    int X, Y;
    char coordX[5], coordY[5];

    if (getCoordinates(queryCmd, coordX, coordY, "query"))
    {
        X = parseInt(coordX);
        if (X < 0 || X > 9999)
        {
            sprintf(cmdReturn, "%s", "error");
            return;
        }

        Y = parseInt(coordY);
        if (Y < 0 || Y > 9999)
        {
            sprintf(cmdReturn, "%s", "error");
            return;
        }

        if (loc->currOccupancy == 0)
        {
            sprintf(cmdReturn, "%s", "none");
            return;
        }

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

        sprintf(cmdReturn, "%s", minDistCoord);
        return;
    }

    sprintf(cmdReturn, "%s", "error");
}

void handleCommand(char *msg, struct Locations *loc, char *cmdReturn)
{
    if (strstr(msg, "add") != NULL)
    { // add X Y
        handleAdd(msg, loc, cmdReturn);
    }
    else if (strstr(msg, "rm") != NULL)
    { // rm X y
        handleRemove(msg, loc, cmdReturn);
    }
    else if (strstr(msg, "list") != NULL)
    { // list
        handleList(msg, loc, cmdReturn);
    }
    else if (strstr(msg, "query") != NULL)
    { // query X Y
        handleQuery(msg, loc, cmdReturn);
    }
    else
    {
        sprintf(cmdReturn, "%s", "error");
    }
}