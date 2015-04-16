/* POSIX multithreading */
/* Simple program using pthread library to  */    
/* Created by Daniel Miksa */
/* Copyright (c) 2014 Daniel Miksa. All rights reserved. */

/*Program check a pressed Key and return its code*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

char getch(){
	/* following function is not my property. It has been founded somewhere in network... */
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

main()
{
	system("clear");
	printf("Press any key to see return code.");
	int getch_out = 0;
	do
	{	
		getch_out = getch();
		system("clear");
		printf("Pressed key return value: %d\n\n\n\n\nPress ESC key to terminate application (ESC returns integer value 27).\n\n", getch_out);
	}while(getch_out != 27);
}