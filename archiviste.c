
#include "archiviste.h"


int main (int argc, char *argv[]){

key_t cle;
pid_t pid;
int file_mess;
int nb_archi = atoi(argv[1]);
int nb_theme = atoi(argv[2]);
int mem_part;
int *tab;
int semap;



if(argc != 3){
	usage(argv[0]);
	exit(-1);
 }  

fprintf(stderr,"good\n");
/* Creation cle */
 cle = ftok(FICHIER_CLE,'a');
    if (cle==-1){
	fprintf(stderr,"Pb creation cle\n");
	exit(-1);
    }

/* Recuperation SMP :    */
    mem_part = shmget(cle,sizeof(int),0);
    if (mem_part==-1){
	printf("(%d) Pb recuperation SMP\n",pid);
	exit(-1);
    }

    /* Attachement SMP :      */
    tab = shmat(mem_part,NULL,0);
    if (tab==(int *)-1){
	printf("(%d) Pb attachement SMP\n",pid);
	exit(-1);
    }

    /* Recuperation semaphore :         */
    semap = semget(cle,1,0);
    if (semap==-1){
	printf("(%d) Pb recuperation semaphore\n",pid);
	exit(-1);
    }
    /* Recuperation file de message:          */		
     file_mess = msgget(cle,0);
	if (file_mess ==-1){
	printf("(%d) Pb recuperation file de message\n",pid);
	exit(-1);
     }

/* Indefiniment :*/

/*while(1){ 
	
	
}*/



}


void usage (char *s){
	printf("Usage : %s <nb_activistes> <nb_themes> \n",s);
}




















