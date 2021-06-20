#pragma once

#include "structure.h"

#include <stdio.h>
#include <stdlib.h>

void handleAdd(char *addCmd, struct Locations *loc, char *cmdReturn);

void handleList(char *addCmd, struct Locations *loc, char *cmdReturn);

void handleRemove(char *addCmd, struct Locations *loc, char *cmdReturn);

void handleQuery(char *addCmd, struct Locations *loc, char *cmdReturn);

void handleCommand(char *msg, struct Locations *loc, char *cmdReturn);