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
#include <string.h>


#define FICHIER_CLE "cle.serv"
#define LETTRE_CODE 'a'
#define NB_ARTICLE 10
#define LETTRE_CODE_SEM 'z'
enum type{
    TYPE_C = 1,
    TYPE_P = 2,
    TYPE_E = 3
};
typedef struct {
    long type;
    int identite;
    int nb_archiviste;
    pid_t pid;
    char txt[4];
} requete_t;

typedef struct{
    long identite; //expediteur
    int type; //consult ..
    pid_t pid;
    
}traitement_t;


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