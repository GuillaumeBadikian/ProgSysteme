
#include "archiviste.h"


int main (int argc, char *argv[]){

    key_t cle;
    int file_mess;
    int nb_theme = atoi(argv[2]);
    int mem_part[nb_theme];
    int semap;
    requete_t message;
    traitement_t reponse;
    int nb_lus;


    if(argc != 3){
        usage(argv[0]);
        exit(-1);
    }  

    for(int j = 0; j< nb_theme;j++){
    /* Creation cle */
        cle = ftok(FICHIER_CLE,j);
        if (cle==-1){
            fprintf(stderr,"Pb creation cle\n");
            exit(-1);
        }

    /* Recuperation SMP :    */
        mem_part[j] = shmget(cle,NB_ARTICLE*sizeof(char[4]),0);
        if (mem_part[j]==-1){
            printf("Pb recuperation SMP\n");
            exit(-1);
        }
    }


    /* Recuperation semaphore :         */
    cle = ftok(FICHIER_CLE ,LETTRE_CODE_SEM);
    semap = semget(cle,1,0);
    if (semap==-1){
        printf("Pb recuperation semaphore\n");
        exit(-1);
    }
    /* Recuperation file de message:          */
    cle = ftok(FICHIER_CLE ,LETTRE_CODE);		
    file_mess = msgget(cle,0);
	if (file_mess ==-1){
        printf("Pb recuperation file de message\n");
        exit(-1);
     }
    

/* Indefiniment :*/

while(1){ 
	if((nb_lus=msgrcv(file_mess, &message,sizeof(requete_t),0,0))==-1){
        couleur(ROUGE); fprintf(stdout,"Erreur de lecture, erreur : %d\n",errno); couleur(REINIT);
        raise(SIGUSR1);
    }

        /* Attachement SMP :      */

    //couleur(VERT);fprintf(stdout,"%i", message.type); couleur(REINIT);
    /*
    tab = shmat(mem_part[message.type],NULL,0);
    if (tab==(int *)-1){
        printf("(%d) Pb attachement SMP archi\n",pid);
        exit(-1);
    }*/


    /*work */
  //  fprintf(stdout, "(Archiviste) : PID %d | Type %ld | Identity %d |txt %s \n", message.pid,message.type,message.identite,message.txt);
    couleur(CYAN);fprintf(stdout, "Reception du signal de %i\n",message.identite);
    reponse.type = message.type;
    reponse.identite = message.identite;
    reponse.pid = getpid();

    couleur(JAUNE);fprintf(stdout, "Envoie reponse a : %ld\n", reponse.identite);
    /* reponse requete */
    msgsnd(file_mess, &reponse, sizeof(traitement_t),message.identite);

}



}


void usage (char *s){
	printf("Usage : %s <nb_activistes> <nb_themes> \n",s);
}




















