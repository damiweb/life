#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

/* global variables for multithreading */

int keypress = 0; /* if key press then handle key code and until the key wont be used, ThreadFunction could not overwrite the value of this variable */

/* implementation of getch function known from windows environment */
char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    //printf("%c\n",buf);
    return buf;
}

/* implementation of multithreading */

void *ThreadFunction(void *arg) {
        do
        {
        	int temp = getch();
        	if(temp == 0) keypress = temp;
        	temp = 0;
        }while(keypress != 27);
        return NULL;
}


main()
{	
	/*
	FILE *fpoint = fopen("file.txt", "a+");
	char buffer[512];
	fgets(buffer, 511, stdin);
	fputs(buffer, stdout);
	*/

system("clear");

/* struktura x, y, ptr */

typedef struct array{
	int** ptr;
	int x;
	int y;
}array;


/* zapytaj o zmienne */

printf("Podaj liczbę wierszy: \n");
int y;
scanf("%i",&y);
printf("Podaj liczbę kolumn: \n");
int x;
scanf("%i",&x);

/* utworzenie tablicy martwej */

int** table = (int**) malloc(x * sizeof(int**));
int i = 0;
for(i = 0; i < x; i++)
{
	int* currentcol= (int*) malloc(y * sizeof(int*));
	table[i] = currentcol;
	int row;
	for(row = 0; row < y; row++)
	{
		table[i][row] = 0;
	}
}
array gametable = {table, x, y};

int r, c;
for(r = 0; r < y; r++){
	for(c = 0; c < x; c++){
		int cont = gametable.ptr[c][r];
		if(cont != 0){
			printf("\u2588");
		} else printf("\u2591");
		/*printf("%i", cont);*/
	}
	printf("\n");
}

/* add alive cells */
int count;


system("clear");
printf("Co chcesz zrobić?\n-Wybrać losowo komórki (1),\n-Wybrać samodzielnie komórki (2),\n");
int opt1;
scanf("%i", &opt1);

// switch variables
int xpos = 0, ypos = 0, sw = 0, again = 1, currentcell;


switch(opt1)
{
	case 1:
		do{
		printf("Podaj liczbę żywych komórek: \n");
		scanf("%i",&count);
		}while(count > ((gametable.x) * (gametable.x)));

		int it = 0;
		for(it = 0; it < count; it++)
		{
			int rx;
			int ry;
			do
			{
			rx = rand() % gametable.x;
			ry = rand() % gametable.y;
			}while(gametable.ptr[rx][ry] > 0);
			gametable.ptr[rx][ry] = 1;
			printf("%i %i \n", rx, ry);

		}
		printf("\n \n \n");
		break;
	case 2:
		do{
			system("clear");
			printf("USE W, A, S, D keys to move, G key to switch a cell state, and P to PROCEED. \n\n");
			printf("coords: %i %i \n\n", xpos, ypos);
			for(r = 0; r < y; r++){
				for(c = 0; c < x; c++){
					currentcell = gametable.ptr[c][r];
					if(currentcell != 0 && c == xpos && r == ypos){
						if(sw != 0){
							gametable.ptr[xpos][ypos] = 0;
							sw = 0;
							printf("\u2591"); // 25%
						} else printf("\u2593"); // 75%
					} else if(currentcell == 0 && c == xpos && r == ypos) {
						if(sw != 0){
							gametable.ptr[xpos][ypos] = 1;
							sw = 0;
							printf("\u2593"); // 75%
						} else printf("\u2591"); // 25%
					} else if(currentcell != 0){
						printf("\u2588"); // 100%
					} else printf(" ");

					/*printf("%i", cont);*/
				}
				printf("\n");
			}
			int action = getch();
			switch(action)
			{
				case 119: // up
					ypos--;
					if(ypos < 0) ypos = y - 1;
					break;
				case 115: // down
					ypos++;
					if(ypos > (y - 1)) ypos = 0;
					break;
				case 100: // right
					xpos++;
					if(xpos > (x - 1)) xpos = 0;
					break;
				case 97: // left
					xpos--;
					if(xpos < 0) xpos = x - 1;
					break;
				case 103: // switch
					sw = 1;
					break;
				case 112: // proceed
					again = 0;
					break;
				default:
					break;
			}

		}while(again);
		break;
	default:
		return 0;

}

system("clear");

/* prompt the game table */

printf("\nIf the game will achieve infinity loop or there will be no difference betwen iterations then press Ctrl + C\nto close an aplication.\n\n");

for(r = 0; r < y; r++){
	for(c = 0; c < x; c++){
		int cont = gametable.ptr[c][r];
		if(cont != 0){
			printf("\u2588");
		} else printf("\u2591");
		/*printf("%i", cont);*/
	}
	printf("\n");
}

/* ALGORYTM */

usleep(900000);


system("clear");

/* buffering table */

int** table2 = (int**) malloc(x * sizeof(int**));
i = 0;
for(i = 0; i < x; i++)
{
	int* currentcol= (int*) malloc(y * sizeof(int*));
	table2[i] = currentcol;
	int row;
	for(row = 0; row < y; row++)
	{
		table2[i][row] = 0;
	}
}
array gametable2 = {table2, x, y};

/* Creating new thread for which have to handle a key */

pthread_t mythread;

if ( pthread_create( &mythread, NULL, ThreadFunction, NULL) ) {
        printf("błąd przy tworzeniu wątku\n"); abort();
}

/* Starting game loop */

int life = 0;
do
{
	life = 0;
	printf("\nIf the game will achieve infinity loop or there will be no difference betwen iterations then press Ctrl + C to close an aplication.\n\n");
	for(r = 0; r < y; r++){
		for(c = 0; c < x; c++){
			int cont = gametable.ptr[c][r];

			/* ALGORYTHM OF GAME */
			if(cont == 0)
			{	/* dead */
				int counter = 0;
				if(c > 0){if(gametable.ptr[c-1][r] > 0) counter++;} // left
				if(c < (gametable.x - 1)){if(gametable.ptr[c+1][r] > 0) counter++;} // right
				if(r > 0){if(gametable.ptr[c][r-1] > 0) counter++;} // top
				if(r < (gametable.y - 1)){if(gametable.ptr[c][r+1] > 0) counter++;} //bottom
				if(c > 0 && r > 0){if(gametable.ptr[c-1][r-1] > 0) counter++;} // top-left
				if(c < (gametable.x - 1) && r > 0){if(gametable.ptr[c+1][r-1] > 0) counter++;} // top-right
				if(c > 0 && r < (gametable.y - 1)){if(gametable.ptr[c-1][r+1] > 0) counter++;} // bottom-left
				if(c < (gametable.x - 1) && r < (gametable.y - 1)){if(gametable.ptr[c+1][r+1] > 0) counter++;} // bottom-right
				if(counter == 3) gametable2.ptr[c][r] = 1;
				counter = 0;


			} else
			{ 	/* alive */
				int counter = 0;
				if(c > 0){if(gametable.ptr[c-1][r] > 0) counter++;} // left
				if(c < (gametable.x - 1)){if(gametable.ptr[c+1][r] > 0) counter++;} // right
				if(r > 0){if(gametable.ptr[c][r-1] > 0) counter++;} // top
				if(r < (gametable.y - 1)){if(gametable.ptr[c][r+1] > 0) counter++;} //bottom
				if(c > 0 && r > 0){if(gametable.ptr[c-1][r-1] > 0) counter++;} // top-left
				if(c < (gametable.x - 1) && r > 0){if(gametable.ptr[c+1][r-1] > 0) counter++;} // top-right
				if(c > 0 && r < (gametable.y - 1)){if(gametable.ptr[c-1][r+1] > 0) counter++;} // bottom-left
				if(c < (gametable.x - 1) && r < (gametable.y - 1)){if(gametable.ptr[c+1][r+1] > 0) counter++;} // bottom-right
				if(counter == 2 || counter == 3) gametable2.ptr[c][r] = 1;
				else gametable2.ptr[c][r] = 0;
				counter = 0;

			}
			/* END OF ALGORYTHM */
			int cont2 = gametable2.ptr[c][r];
			if(cont2 != 0){
				printf("\u2588");
				life++;
			} else printf("\u2591");

		}
		printf("\n");
	}

	/* rewrite a table */

	for(i = 0; i < x; i++)
	{
		int row;
		for(row = 0; row < y; row++)
		{
			table[i][row] = table2[i][row];
		}
	}

	usleep(900000);

	if(keypress != 0){
		int option = 0;
		do{
			system("clear");
			printf("Space pressed, ALGORYTM PAUSED. What do you want to do?\n- CONTINUE (press C key).");
			option = getch();
		} while(option != 99);
	}
	if(keypress == 27)
	{
		life = 0;
	}

	system("clear");

} while(life);

if ( pthread_join ( mythread, NULL ) ) {
	printf("błąd w kończeniu wątku\n");
	abort();
}

usleep(900000);
usleep(900000);
usleep(900000);

printf("*********** GAME OVER ***********");

usleep(900000);
usleep(900000);
usleep(900000);

system("clear");

}

/* SOURCES:

http://unicode.org/charts/PDF/U2580.pdf
http://pl.wikipedia.org/wiki/Gra_w_%C5%BCycie#Opis_regu.C5.82_gry
http://en.wikipedia.org/wiki/Escape_sequences_in_C
http://www.tutorialspoint.com/c_standard_library/c_function_rand.htm
http://en.cppreference.com/w/c/io


*/