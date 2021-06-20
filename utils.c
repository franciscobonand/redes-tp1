#include "utils.h"

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