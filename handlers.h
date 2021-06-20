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

void writeReturnMessage(char *s1, char *s2, char *s3, char *cmdReturn);

float distance(int x1, int y1, int x2, int y2);

int getCoordinates(char *cmd, char *X, char *Y);

void handleAdd(char *addCmd, struct Locations *loc, char *cmdReturn);
void handleList(char *addCmd, struct Locations *loc, char *cmdReturn);
void handleRemove(char *addCmd, struct Locations *loc, char *cmdReturn);
void handleQuery(char *addCmd, struct Locations *loc, char *cmdReturn);
void handleCommand(char *msg, struct Locations *loc, char *cmdReturn);