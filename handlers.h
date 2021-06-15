#pragma once

#include <stdio.h>
#include <stdlib.h>

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

int parseInt(char *str);

int elementAlreadyRegistered(int X, int Y, struct Locations *loc);

const char *writeReturnMessage(char *s1, char *s2, char *s3);

float distance(int x1, int y1, int x2, int y2);

int getCoordinates(char *cmd, char *X, char *Y);

const char *handleAdd(char *addCmd, struct Locations *loc);

const char *handleList(char *addCmd, struct Locations *loc);

const char *handleRemove(char *addCmd, struct Locations *loc);

const char *handleQuery(char *addCmd, struct Locations *loc);

const char *handleCommand(char *msg, struct Locations *loc);