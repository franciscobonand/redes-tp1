#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define BUFSZ 500

struct Coordinates
{
    int X;
    int Y;
};

struct Locations
{
    int currOccupancy;
    struct Coordinates points[50];
};

int parseInt(char *str)
{
    if (str[strlen(str) - 1] == '\n')
    {
        str[strcspn(str, "\n")] = 0;
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