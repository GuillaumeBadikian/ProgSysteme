#ifndef _init_h_
#define _init_h_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <signal.h>

#define FICHIER_CLE "cle.serv"
#define LETTRE_CODE 'a'


void usage(char * s);
int arg_valid(int argv1, int  argv2);
void creer_IPC(int argv1, int  argv2);




#endif
