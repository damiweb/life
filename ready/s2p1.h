/* The Game of Life */
/* ANSI C implementation of the game of life */    
/* Created by Daniel Miksa */
/* Copyright (c) 2014 Daniel Miksa. All rights reserved. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/* struktura x, y, ptr */

typedef struct array{
int** ptr;
int x;
int y;
}array;

char getch();
array create(void);
void display(array gametable);
void fill(array gametable);