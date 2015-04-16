/* getch test */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>


int main()
{
	FILE* fp;

	fp = fopen("data.txt", "w+");
	fprintf(fp, "20 12 14 54 81 4 15 46 21 58 ");
	fputs("32 42 15 21 26 74 12 34", fp);
	fprintf()
	fclose(fp);
	int x = 10, y = 10;
	int* al_cells = (int*) malloc((x*y) * sizeof(int)); 
	int i;
	FILE * alive_cells;
	alive_cells = fopen("data.txt","r");   /* otwiera plik ilosc.txt, który ma 100 liczb w kolumnie*/
	if (alive_cells==NULL) perror ("W tym katalogu brakuje pliku ilosc.txt!\n\n\n"); /*sprawdza czy ten plik istnieje*/
	else
	{
		for (i = 0; i < 100; ++i)
		{
		if (feof(alive_cells)) break;      /* jeśli koniec pliku, to kończymy pobieranie */
			fscanf(alive_cells, "%d", &al_cells[i]);   /* wczytuje liczby z pliku do tablicy*/
			printf("Oto %d. liczba: %d\n", i+1, al_cells[i]);
		}
		fclose(alive_cells);
	}
	return 0;
}
