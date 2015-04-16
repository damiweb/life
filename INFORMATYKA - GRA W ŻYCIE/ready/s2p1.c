#include "s2p1.h"

main()
{
system("clear");

/* zapytaj o zmienne */

array life = create();

/* DISPLAY EMPTY TABLE - TESTFUNCTION */

fill(life);

/* add alive cells */

/*switch, editor etc...*/

system("clear");

/* prompt the game table */

game(life);

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