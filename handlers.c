// TODO: number starting with 0
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

void writeReturnMessage(char *s1, char *s2, char *s3, char *cmdReturn)
{
    int length = strlen(s1) + strlen(s2) + strlen(s3) + 3;
    char newStr[length];
    bzero(newStr, strlen(newStr));

    strcpy(newStr, s1);
    strcat(newStr, " ");
    strcat(newStr, s2);
    strcat(newStr, " ");
    strcat(newStr, s3);
    sprintf(cmdReturn, "%s", newStr);
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
    if (strlen(cmd) <= 15)
    {
        char *strReader[10]; // max valid size for command after first word is removed ('9999 9999\n')
        char xStr[5];        // max valid size for one coordinate
        char yStr[5];        // max valid size for one coordinate
        char buf[2];         // char size
        bzero(xStr, strlen(xStr));
        bzero(yStr, strlen(yStr));

        for (int i = 0; i < strlen(cmd) - 1; i++)
        {
            sprintf(buf, "%c", cmd[i]);
            if (strcmp(buf, " ") == 0)
            {
                *strReader = &cmd[i + 1];
                strcpy(xStr, *strReader);
                bzero(buf, strlen(buf));
                break;
            }
            bzero(buf, strlen(buf));
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
                strncpy(yStr, *strReader, 4);
                break;
            }
            bzero(buf, strlen(buf));
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
void handleAdd(char *addCmd, struct Locations *loc, char *cmdReturn)
{
    int X, Y;
    char coordX[5], coordY[5];

    if (getCoordinates(addCmd, coordX, coordY))
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
        printf("msg in add: %s\n", cmdReturn);
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
        printf("list size: %ld\n", strlen(list));
        printf("list: %s\n", list);
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

    if (getCoordinates(rmCmd, coordX, coordY))
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

    if (getCoordinates(queryCmd, coordX, coordY))
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