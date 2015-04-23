/* The Game of Life */
/* ANSI C implementation of the game of life */    
/* Created by Daniel Miksa */
/* Copyright (c) 2014 Daniel Miksa. All rights reserved. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <pthread.h>

/* struktura x, y, ptr */

typedef struct array{
int** ptr;
int x;
int y;
}array;

char getch();
void *ThreadFunction(void *arg);
char* get_string();
int file_open(char* name, array matrix, int* coords);
int fill_matrix(array matrix, int* coords);
array create(void);
void display(array gametable);
void fill(array gametable);
int save(array gametable);