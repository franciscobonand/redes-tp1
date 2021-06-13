#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSZ 500

struct Coordinates
{
    int X;
    int Y;
};

struct Locations
{
    int currOcupation;
    struct Coordinates points[50];
};

int parseInt(char *str, int *val)
{
    char *temp;
    *val = strtol(str, &temp, 0);

    if (temp == str || *temp != '\0')
        return 0;

    return 1;
}

int elementAlreadyRegistered(int X, int Y, struct Locations *loc)
{
    for (int i = 0; i < loc->currOcupation; i++)
    {
        if (X == loc->points[i].X && Y == loc->points[i].Y)
            return 1;
    }
    return 0;
}

const char *writeReturnMessage(char *s1, char *s2, char *s3)
{
    char *ns = malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1);
    ns[0] = '\0';
    strcat(ns, s1);
    strcat(ns, " ");
    strcat(ns, s2);
    strcat(ns, s3);
    return ns;
}

// handleAdd handles the 'add' command. If it was successful, returns 1.
// if the new element to be inserted already exists, returns 2.
// if an error occured, returns 0.
const char *handleAdd(char *addCmd, struct Locations *loc)
{
    int X, Y;

    char *coordX = strtok(NULL, " ");
    if (!parseInt(coordX, &X))
        return "error";

    char *coordY = strtok(NULL, " ");
    if (!parseInt(coordY, &Y))
        return "error";

    char *end = strtok(NULL, " ");
    if (end != NULL)
        return "error";

    if (elementAlreadyRegistered(X, Y, loc))
        return writeReturnMessage(coordX, coordY, " already exists");

    loc->points[loc->currOcupation].X = X;
    loc->points[loc->currOcupation].Y = Y;
    loc->currOcupation++;

    return writeReturnMessage(coordX, coordY, " added");
}

const char *handleCommand(char *msg, struct Locations *loc)
{
    const char *cmdReturn;
    char *cmd = strtok(msg, " ");

    if (strcmp(cmd, "add") == 0)
    { // add X Y
        cmdReturn = handleAdd(cmd, loc);
    }
    else if (strcmp(cmd, "rm") == 0)
    { // rm X y
    }
    else if (strcmp(cmd, "list") == 0)
    { // list
    }
    else if (strcmp(cmd, "query") == 0)
    { // query X Y
    }
    else
    {
    }

    return cmdReturn;
}