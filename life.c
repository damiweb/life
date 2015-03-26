// table reader
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  **int ptr;
  int row;
  int col;
} array;

int **table(int x, int y)
{
	int **tab = (int**) malloc(x * sizeof(int*));
	if(tab != NULL)
	{
		int i = 0;
		for(i = 0; i < x; i++)
		{
			int *col = (int*) malloc(y * sizeof(int));
			if(col == NULL)
			{
				free(tab);
				return NULL;
			}
			else
			{
				tab[i] = col;
				int row = 0;
				for(row = 0; row<y; row++)
				{
                    tab[i][row] = 0;
				}
			}
		}
		return tab;
	}
	else
	{
		free(tab);
		return NULL;
	}
}

void write(int **table, int x, int y)
{
	int r = 0;
	for(r = 0; r < y; r++)
	{
		int c = 0;
		for(c = 0; c < x; c++)
		{
			// printf("%3i", table[c][r]);
			if(table[c][r] == 0)
			{
				printf("\u2578 ");
			}
			else
			{
				printf("\u2588 ");
			}
			
		}printf("\n");
	}
}

void change_state(int **table, array table)
{
	printf("Podaj współrzędną X:");
	int x;
	scanf("%d", &x);
	printf("Podaj współrzędną Y:");
	int y;
	scanf("%d", &y);
	if(x <)	
		if(table[x][y] == 0) table[x][y] = 1;
		else table[x][y] = 0;
}

main()
{
	int **array = table(20, 10);
	array my{array, 10, 20};
	int condition = 1;
	while(condition)
	{
		fflush(stdin);
		write(array, 20, 10);
		int option = 0;
		scanf("%d", &option);
		switch(option)
		{
			case 1:
				change_state(table);
				system("clear");
				break;
			case 0:
				condition = 0;
				break;
			default:
				condition = 0;
				break;
		}
	}
}
