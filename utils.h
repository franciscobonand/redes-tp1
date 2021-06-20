#pragma once

#include "structure.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int parseInt(char *str);

int elementAlreadyRegistered(int X, int Y, struct Locations *loc);

void writeReturnMessage(char *s1, char *s2, char *s3, char *cmdReturn);

float distance(int x1, int y1, int x2, int y2);

int getCoordinates(char *cmd, char *X, char *Y);