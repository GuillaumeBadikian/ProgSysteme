
#include "journaliste.h"

int file_mess, mem_part, semap;
requete_t message;
traitement_t reponse;
pid_t pid;
int th_concerne;
int main(int argc, char * argv[], char ** envp){
    char arg5[4];
    strcpy(arg5,argv[5]);
    pid = getpid();

    if(argc!=6){
        usage(argv[0]);
        exit(-1);
    }
    th_concerne = atoi(argv[4]);

      /* init message */
    message.pid = pid;
    message.nb_archiviste = atoi(argv[2]);
    message.identite = atoi(argv[1]);
    //couleur(ROUGE); fprintf(stdout, "%s | %s | %s | %s | %s\n",argv[1],argv[2],argv[3],argv[4], argv[5]); couleur(REINIT);
    switch (argv[3][0]){
        case 'c':
          message.type = TYPE_C;
            break;
        case 'p':
            message.type = TYPE_P;
            strcpy(message.txt,arg5);
            break;
        case 'e':
         message.type = TYPE_E;
            break;

    }
    recup_IPC();
    sleep(1);
    exit(0);
   // return 0;

}


void recup_IPC(){
    key_t cle;
    int nb_lus;

/* Creation cle */
 cle = ftok(FICHIER_CLE,LETTRE_CODE);
    if (cle==-1){
	fprintf(stderr,"Pb creation cle\n");
    kill(getppid(),SIGUSR1);
	//exit(-1);
    }

    file_mess = msgget(cle,0666);
	if (file_mess ==-1){
	printf("(%d) Pb recuperation file de message journal\n",pid);
	exit(-1);
     }

    /* envoie requete */
    couleur(VERT);fprintf(stdout, "Envoie d'une requete %ld\n",message.type);
    msgsnd(file_mess, &message, sizeof(requete_t),0);


    /* reception reponse */
    if((nb_lus=msgrcv(file_mess, &reponse,sizeof(traitement_t),message.identite,0))==-1){
        couleur(ROUGE); fprintf(stdout,"Erreur de lecture, erreur : %d\n",errno); couleur(REINIT);
        raise(SIGUSR1);
    }
    couleur(CYAN);fprintf(stdout, "recup reponse \n");


}


void usage(char * s){
    printf("Usage : %s <identite> <nb archiviste> <type requete>\n",s);  
  }