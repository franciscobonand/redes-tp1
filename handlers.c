#include "handlers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

int parseInt(char *str)
{
    if (str[strlen(str) - 1] == '\n')
    {
        str[strcspn(str, "\n")] = 0; // removes '\n' from string
    }

    int val = atoi(str);

    char validator[strlen(str)];
    sprintf(validator, "%d", val);

    if (strcmp(validator, str) == 0)
        return val;

    return -1;
}

// elementAlreadyRegistered returns the array index of tuple of points in `loc`,
// if it exists. Returns -1 otherwise
int elementAlreadyRegistered(int X, int Y, struct Locations *loc)
{
    for (int i = 0; i < loc->currOccupancy; i++)
    {
        if (X == loc->points[i].X && Y == loc->points[i].Y)
            return i;
    }
    return -1;
}

const char *writeReturnMessage(char *s1, char *s2, char *s3)
{
    char *newStr = malloc(strlen(s1) + strlen(s2) + strlen(s3) + 3);
    newStr[0] = '\0';
    strcat(newStr, s1);
    strcat(newStr, " ");
    strcat(newStr, s2);
    strcat(newStr, " ");
    strcat(newStr, s3);
    return newStr;
}

// calculates the euclidean distance between (x1, y1) and (x2, y2)
float distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

// getCoordinates receives a command string and extracts the coordinates X an Y from it.
// If successful, returns 1. Returns 0 otherwise.
int getCoordinates(char *cmd, char *X, char *Y)
{
    if (strlen(cmd) < 15)
    {
        char *strReader[10]; // max valid size for command after 'add ' is removed ('9999 9999\n')
        char xStr[5];        // max valid size for one coordinate
        char yStr[5];        // max valid size for one coordinate
        char buf[2];         // char size

        for (int i = 0; i < strlen(cmd) - 1; i++)
        {
            sprintf(buf, "%c", cmd[i]);
            if (strcmp(buf, " ") == 0)
            {
                *strReader = &cmd[i + 1];
                strcpy(xStr, *strReader);
                memset(buf, 0, 2);
                break;
            }
            memset(buf, 0, 2);
            if (i == strlen(cmd) - 2)
            { // case when no white space between numbers is found
                return 0;
            }
        }

        for (int i = 0; i < strlen(xStr) - 1; i++)
        {
            sprintf(buf, "%c", xStr[i]);
            if (strcmp(buf, " ") == 0)
            {
                *strReader = &xStr[i + 1];
                xStr[i] = '\0';
                strcpy(yStr, *strReader);
                break;
            }
            memset(buf, 0, 2);
            if (i == strlen(cmd) - 2)
            { // case when no white space between numbers is found
                return 0;
            }
        }

        strcpy(X, xStr);
        strcpy(Y, yStr);
        return 1;
    }

    return 0;
}

// handleAdd handles the 'add X Y' command
const char *handleAdd(char *addCmd, struct Locations *loc)
{
    int X, Y;
    char coordX[5], coordY[5];

    if (getCoordinates(addCmd, coordX, coordY))
    {
        X = parseInt(coordX);
        if (X < 0 || X > 9999)
            return "error";

        Y = parseInt(coordY);
        if (Y < 0 || Y > 9999)
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
    return "error";
}

// handleList handles the 'list' command
const char *handleList(char *addCmd, struct Locations *loc)
{
    if (strlen(addCmd) < 5 ||
        (strlen(addCmd) == 5 && addCmd[strlen(addCmd) - 1] == '\n'))
    {
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

    return "error";
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