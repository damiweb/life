/* POSIX multithreading */
/* Simple program using pthread library to  */    
/* Created by Daniel Miksa */
/* Copyright (c) 2014 Daniel Miksa. All rights reserved. */
    

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
            /*printf("   I'm working!");*/
        	int temp = getch();
        	if(keypress == 0) keypress = temp;
            /*printf("\n\n Pressed key: %d", keypress);*/
        	temp = 0;
        }while(keypress != 27);
        return NULL;
}

main(){

    int life = 1;

    pthread_t mythread;

    if ( pthread_create( &mythread, NULL, ThreadFunction, NULL) ) {
        printf("błąd przy tworzeniu wątku\n"); abort();
    }

    do{
        if(keypress == 32){
            do{
                keypress = 0;
                system("clear");
                printf("Space pressed, ALGORYTM PAUSED. What do you want to do?\n- CONTINUE (press C key).");
                printf("\n\nAlready pressed key: %d \n", keypress);
                sleep(1);
            } while(keypress != 99);
            keypress = 0;
        }
        if(keypress == 27)
        {
            life = 0;
        }
        if(keypress != 0)
        {
            keypress = 0; /* ignore other -> unusual keycodes */
        }
        printf("SOME TEXT MESSAGE UNTIL KEY WILL BE PRESSED\nnew line\nnew line\nnew line\nnew line\nnew line\nnew line\netc...\n");
        sleep(1);
        system("clear");
    } while(life);

    if ( pthread_join ( mythread, NULL ) ) {
    printf("błąd w kończeniu wątku\n");
    abort();
    }

}