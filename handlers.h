#pragma once

#include "utils.c"
#include <stdio.h>
#include <stdlib.h>

#define BUFSZ 500

const char *handleAdd(char *addCmd, struct Locations *loc);

const char *handleList(char *addCmd, struct Locations *loc);

const char *handleRemove(char *addCmd, struct Locations *loc);

const char *handleQuery(char *addCmd, struct Locations *loc);

const char *handleCommand(char *msg, struct Locations *loc);