#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int getMsgSize(char *reader, char *receiver);