/* The Game of Life */
/* ANSI C implementation of the game of life */    
/* Created by Daniel Miksa */
/* Copyright (c) 2014 Daniel Miksa. All rights reserved. */

#include "s2p1.h"

int keypress = 0;

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
  /*printf("%c\n",buf);*/
  return buf;
}

/* implementation of multithreading */

void *ThreadFunction(void *arg) {
        do
        {
          fflush(stdin);
          int temp = getch();
          if(keypress == 0) keypress = temp;
          temp = 0;
        }while(keypress != 27);
        return NULL;
}

/* Get string function */
/* Function is reading from stdin up to 63 characters. After that it is 
puting the string to array, next it's counting the string length and return pointer
to the array, which is exactly that length what a string we type.  */

char* get_string(){
  char inbuffer[64];
  fgets(inbuffer, 63, stdin);
  /*fputs(inbuffer, stdout);*/
  int len = strlen(inbuffer);
  /*printf("Dlugosc nazwy pliku: %d\n", len);*/
  char* fname = (char*) malloc(len * sizeof(char)); 
  strncpy(fname, inbuffer, (len-1));
  fname[len-1] = '\0';
  /*fputs(fname, stdout);*/
  return fname;
}

/* File open function - opens a file and fill the array with numbers contained in a file.
Parameters:
  name - name of file have to be loaded,
  matrix - pointer to a 2D matrix structure;
  coords - pointer to an array with x, y coordinates,
return - error (1), success (0)
*/

int file_open(char* name, array matrix, int* coords)
{
  FILE* fpoint;
  fpoint = fopen(name,"r");   /* otwiera plik ilosc.txt, który ma 100 liczb w kolumnie*/
  if (fpoint==NULL) {perror ("W tym katalogu brakuje pliku ilosc.txt!\n\n\n"); return 1;} /*sprawdza czy ten plik istnieje*/
  else
  {
    int i;
    for (i = 0; i < ((matrix.x)*(matrix.y)*2); ++i)
    {
    if (feof(fpoint)) break;      /* jeśli koniec pliku, to kończymy pobieranie */
      fscanf(fpoint, "%d", &coords[i]);   /* wczytuje liczby z pliku do tablicy*/
      printf("Oto %d. liczba: %d\n", i+1, coords[i]);
    }
    fclose(fpoint);
    return 0; /* success */
  }
}

/* Function fill the game matrix base on array read from file */

int fill_matrix(array matrix, int* coords)
{
  int selx = -1;
  int sely = -1;
  int i;
  int counter = 0;
  for(i = 0; i<((matrix.x)*(matrix.y)*2); i++)
  {
    if(coords[i] == -1) break;
    /*printf("%d\n", i);*/
    if((i+1)%2) {/*printf("zaznaczono x\n");*/ selx = i;}
    if(i%2) {/*printf("zaznaczono y\n");*/ sely = i;}
    /*printf("%d %d\n\n", selx, sely);*/
    if((selx >= 0) && (sely >= 0))
    {
      /*printf("pierwszy warunek składany %d %d \n", al_cells[selx], al_cells[sely]);*/
      int tx = coords[selx];
      int ty = coords[sely];

      if(tx < matrix.x && tx >= 0 && ty < matrix.y && ty >= 0){
        /*printf("drugi warunek składany %d %d\n", tx, ty);*/
        matrix.ptr[tx][ty] = 1;
        counter++;
        }
        selx = -1;
        sely = -1;
    }
  }
  return counter;
}

array create(void)
{
  printf("Podaj liczbę wierszy: \n");
  int y;
  scanf("%i",&y);
  printf("Podaj liczbę kolumn: \n");
  int x;
  scanf("%i",&x);
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
  return gametable;
}

void display(array gametable)
{
  int x = gametable.x;
  int y = gametable.y;
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
}

void fill(array gametable)
{
  int x = gametable.x;
  int y = gametable.y;
  int count;

  system("clear");
  printf("Co chcesz zrobić?\n-Wybrać losowo komórki (1),\n-Wybrać samodzielnie komórki (2),\n-Wczytac komorki z pliku (3).\n");
  int opt1;
  scanf("%i", &opt1);
  fflush(stdin);
  /*switch variables*/
  int xpos = 0, ypos = 0, sw = 0, again = 1, currentcell;
  int r, c; /*variables for loops*/

  /* File necessary variables */
  FILE* alive_cells;
  int valid;
  char* fname;
  int* al_cells;
  int i;
  int open_result;
  char* waste_newlinechar[2];

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
        	    printf("\u2591"); /*25%*/
        	  } else printf("\u2593"); /*75%*/
        	  } else if(currentcell == 0 && c == xpos && r == ypos) {
        	  if(sw != 0){
        	    gametable.ptr[xpos][ypos] = 1;
        	    sw = 0;
        	    printf("\u2593"); /*75%*/
        	  } else printf("\u2591"); /*25%*/
        	  } else if(currentcell != 0){
        	  printf("\u2588"); /*100%*/
        	  } else printf(" ");

        	  /*printf("%i", cont);*/
  	       }
          printf("\n");
        }
        int action = getch();
        switch(action)
        {
        	case 119: /*up*/
        	  ypos--;
        	  if(ypos < 0) ypos = y - 1;
        	  break;
        	case 115: /*down*/
        	  ypos++;
        	  if(ypos > (y - 1)) ypos = 0;
        	  break;
        	case 100: /*right*/
        	  xpos++;
        	  if(xpos > (x - 1)) xpos = 0;
        	  break;
        	case 97: /*left*/
        	  xpos--;
        	  if(xpos < 0) xpos = x - 1;
        	  break;
        	case 103: /*switch*/
        	  sw = 1;
        	  break;
        	case 112: /*proceed*/
        	  again = 0;
        	  break;
        	default:
        	  break;
        }
      }while(again);
      break;
    case 3:
      valid=1;
      fgets(waste_newlinechar, 2, stdin);
      do{
        printf("Wpisz nazwę pliku do otwarcia:\n");
        fname = get_string();
        alive_cells = fopen(fname,"r");
        printf("\n");
        if (alive_cells == NULL) printf("Niepoprawna nazwa pliku, sprobuj ponownie:\n");
        else valid = 0;
      }while(valid);
      /* array allocation */
      al_cells = (int*) malloc((x*y*2) * sizeof(int)); 
      for (i = 0; i < (x*y*2); ++i)
      {
        al_cells[i] = -1;
      }
      /* opening a file with prompted name */
      open_result = file_open(fname, gametable, al_cells);
      if(open_result == 0){
        int inserted = fill_matrix(gametable, al_cells);
        printf("Wstawiono %d komorek.\n\n", inserted);  
      }
      break; /* END of CASE 3 */
    default:
      break;
  }
}

int game(array gametable)
{
  int x = gametable.x;
  int y = gametable.y;  
  printf("\nIf the game will achieve infinity loop or there will be no difference betwen iterations then press Ctrl + C\nto close an aplication.\nPress SPACE to pause and choose the action. \n\n\n");
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

  /* ALGORYTM */

  usleep(900000);
  system("clear");

  /* buffering table */

  int** table2 = (int**) malloc(x * sizeof(int**));
  int i = 0;
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



  int life = 0;
  do
  {
    life = 0;
    printf("\nIf the game will achieve infinity loop or there will be no difference betwen iterations then press Ctrl + C to close an aplication.\nPress SPACE to pause and choose the action.\n\n");
    for(r = 0; r < y; r++){
      for(c = 0; c < x; c++){
	     int cont = gametable.ptr[c][r];

	     /* ALGORYTHM OF GAME */
	     if(cont == 0)
	     { /* dead */
	       int counter = 0;
	       if(c > 0){if(gametable.ptr[c-1][r] > 0) counter++;} /*left*/
	       if(c < (gametable.x - 1)){if(gametable.ptr[c+1][r] > 0) counter++;} /*right*/
	       if(r > 0){if(gametable.ptr[c][r-1] > 0) counter++;} /*top*/
	       if(r < (gametable.y - 1)){if(gametable.ptr[c][r+1] > 0) counter++;} /*bottom*/
	       if(c > 0 && r > 0){if(gametable.ptr[c-1][r-1] > 0) counter++;} /* top-left*/
	       if(c < (gametable.x - 1) && r > 0){if(gametable.ptr[c+1][r-1] > 0) counter++;} /* top-right*/
	       if(c > 0 && r < (gametable.y - 1)){if(gametable.ptr[c-1][r+1] > 0) counter++;} /* bottom-left*/
	       if(c < (gametable.x - 1) && r < (gametable.y - 1)){if(gametable.ptr[c+1][r+1] > 0) counter++;} /* bottom-right*/
	       if(counter == 3) gametable2.ptr[c][r] = 1;
	       counter = 0;
     
     
	     } else
     	 { /* alive */
	       int counter = 0;
	       if(c > 0){if(gametable.ptr[c-1][r] > 0) counter++;} /* left*/
	       if(c < (gametable.x - 1)){if(gametable.ptr[c+1][r] > 0) counter++;} /* right*/
	       if(r > 0){if(gametable.ptr[c][r-1] > 0) counter++;} /* top*/
	       if(r < (gametable.y - 1)){if(gametable.ptr[c][r+1] > 0) counter++;} /*bottom*/
	       if(c > 0 && r > 0){if(gametable.ptr[c-1][r-1] > 0) counter++;} /* top-left*/
	       if(c < (gametable.x - 1) && r > 0){if(gametable.ptr[c+1][r-1] > 0) counter++;} /* top-right*/
	       if(c > 0 && r < (gametable.y - 1)){if(gametable.ptr[c-1][r+1] > 0) counter++;} /* bottom-left*/
	       if(c < (gametable.x - 1) && r < (gametable.y - 1)){if(gametable.ptr[c+1][r+1] > 0) counter++;} /* bottom-right*/
	       if(counter == 2 || counter == 3) gametable2.ptr[c][r] = 1;
	       else gametable2.ptr[c][r] = 0;
	       counter = 0;
     
	     }
	     /* END OF ALGORYTHM */
	     int cont2 = gametable2.ptr[c][r];
       if(cont2 != 0){
         printf("\u2588");
       	 life = 1;
       } else printf("\u2591");
       
      }
      printf("\n");
    }

    /* rewrite a table */
    int i;
    for(i = 0; i < x; i++)
    {
      int row;
      for(row = 0; row < y; row++)
      {
        gametable.ptr[i][row] = table2[i][row];
      }
    }

    usleep(900000);
    printf("\n");
    /* interrupt process with specified keys */
    if(keypress == 32){
      int condition = 1;
      do{
        keypress = 0;
        system("clear");
        printf("What do you want to do?\n-Terminate the game and save file (press S)\n-Just CONTINUE (press SPACE),\n-Break without saving (press ESC).\n");
        sleep(1);
        if(keypress == 32){condition = 0; keypress = 0;}
        if(keypress == 27){condition = 0; life = 0; return 1;} /* Force stop program */
        if(keypress == 115){condition = 0; life = 0; keypress = 0;}
      } while(condition);
    }

    system("clear");

  } while(life);
}

/* this function save a file with a frame we paused */

int save(array gametable)
{
  printf("Saving...\n");
  FILE* dataout;
  dataout = fopen("output.txt","w+");
    int i;
    for(i = 0; i < gametable.x; i++)
    {
      int row;
      for(row = 0; row < gametable.y; row++)
      {
        if(gametable.ptr[i][row] == 1) fprintf(dataout, "%d %d\n", i, row);
      }
    }
    fclose(dataout);
}