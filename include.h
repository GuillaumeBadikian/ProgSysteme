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
#include <time.h>


#define FICHIER_CLE "cle.serv"
#define LETTRE_CODE 'a'


/* Couleurs dans xterm */
#define couleur(param) printf("\033[%sm",param)

#define NOIR  "30"
#define ROUGE "31"
#define VERT  "32"
#define JAUNE "33"
#define BLEU  "34"
#define CYAN  "36"
#define BLANC "37"
#define REINIT "0"


/* 
   Utilisation (pour ecrire en rouge) :
   
   couleur(ROUGE); fprintf(stdout,"Hello\n"); couleur(REINIT);
 
*/