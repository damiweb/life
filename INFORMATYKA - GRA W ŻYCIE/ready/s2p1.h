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