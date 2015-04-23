/* The Game of Life */
/* ANSI C implementation of the game of life */    
/* Created by Daniel Miksa */
/* Copyright (c) 2014 Daniel Miksa. All rights reserved. */

#include "s2p1.h"

main()
{
	pthread_t mythread; /* new thread variable */
	system("clear");

	/* zapytaj o zmienne */

	array life = create();
	fflush(stdin);

	/* DISPLAY EMPTY TABLE - TESTFUNCTION */

	fill(life);

	/* add alive cells */

	/*switch, editor etc...*/

	system("clear");

	/* starting second thread for key handling */

	if ( pthread_create( &mythread, NULL, ThreadFunction, NULL) ) {
		printf("błąd przy tworzeniu wątku\n"); abort();
	}

	int gameresult = game(life);
	printf("You successfully left a game. Press ESC to CONTINUE saving process.\n");
	if ( pthread_join ( mythread, NULL ) ) {
	    printf("błąd w kończeniu wątku\n");
	}
	if(gameresult == 0){
	    int end_result = save(life);
	    if(end_result == 0) printf("File saved successfully. GOOD BYE.\n");
	    else printf("An error occured. Program abnormal terminated.\n");
	}

}

/* SOURCES:

http://unicode.org/charts/PDF/U2580.pdf
http://pl.wikipedia.org/wiki/Gra_w_%C5%BCycie#Opis_regu.C5.82_gry
http://en.wikipedia.org/wiki/Escape_sequences_in_C
http://www.tutorialspoint.com/c_standard_library/c_function_rand.htm
http://en.cppreference.com/w/c/io


*/