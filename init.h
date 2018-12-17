#ifndef _init_h_
#define _init_h_

#include "include.h"


void usage(char * s);
int arg_valid(int argv1, int  argv2);
void creer_IPC(int argv1, int  argv2);
void mon_sigaction(int signal, void (*f)(int));
void arret(int i);



#endif
