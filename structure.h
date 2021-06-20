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