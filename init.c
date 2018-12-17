
#include "init.h"


int file_mess, mem_part, semap;


int main(int argc, char * argv[], char ** envp){

  
  srand(time(NULL));
  
  sigset_t masque;

  if(argc != 3){
    usage(argv[0]);
    exit(-1);
  }
  
  if(arg_valid(atoi(argv[1]),atoi(argv[2]))==0){
    printf("%s <nb archiviste> <nb themes> entre 2 et 100\n",argv[0]);
    exit(-1);
  }

  mon_sigaction(SIGUSR1, arret);
  mon_sigaction(SIGUSR2, arret);
  mon_sigaction(SIGINT, arret);
  mon_sigaction(SIGSTOP, arret);
  mon_sigaction(SIGTSTP, arret);
  
  /* masque vide */
  sigemptyset(&masque);
  /* on ajoute ce que l'on ne veut pas */
//  sigaddset(&masque,SIGKILL);
 // sigaddset(&masque,SIGCHLD);
  /* on bloque */
  sigprocmask(SIG_BLOCK,&masque,NULL);

  

  

  /* on peut commencer
   * les parametres sont bons
   */

  creer_IPC(atoi(argv[1]),atoi(argv[2]));
  exit(0);
}


void creer_IPC(int argv1, int argv2){
    key_t cle;
    struct stat st;
    int res_init;
    int * tab;
    unsigned short val_init[1]={1};
    pid_t pid[argv1];
    pid_t pid_journal;
    int i;

    int demande_archive;
    int identite_journal = 10000;
    char * argv_journal[] = {"journaliste",NULL,NULL,NULL, NULL};
    char * argv_archiv[] = {"archiviste",NULL,NULL,NULL};
    char arg1[50]; char arg2[50];
  
    if ((stat(FICHIER_CLE,&st) == -1) &&
	(open(FICHIER_CLE, O_RDONLY | O_CREAT | O_EXCL, 0660) == -1)){
      fprintf(stderr,"Pb creation fichier cle\n");
      exit(-1);
    }

    cle = ftok(FICHIER_CLE,LETTRE_CODE);
    if (cle==-1){
      printf("Pb creation cle\n");
      exit(-1);
    }

    /* On cree le SMP et on teste s'il existe deja :    */
    mem_part = shmget(cle,sizeof(int),IPC_CREAT | IPC_EXCL | 0660);
    if (mem_part==-1){
	    printf("Pb creation SMP ou il existe deja\n");
	    exit(-1);
    }
    
    /* Attachement de la memoire partagee :          */
    tab = shmat(mem_part,NULL,0);
    if (tab==(int *) -1){
	    printf("Pb attachement\n");
      /* Il faut detruire le SMP puisqu'on l'a cree : */
      shmctl(mem_part,IPC_RMID,NULL);
      exit(-1);
    }

       /* On cree le semaphore (meme cle) :                     */
    semap = semget(cle,1,IPC_CREAT | IPC_EXCL | 0660);
    if (semap==-1){
	    printf("Pb creation semaphore ou il existe deja\n");
	    /* Il faut detruire le SMP puisqu'on l'a cree : */
	    shmctl(mem_part,IPC_RMID,NULL);
	    /* Le detachement du SMP se fera a la terminaison */
	    exit(-1);
    }

    /* On l'initialise :                                     */
    res_init = semctl(semap,1,SETALL,val_init);
    if (res_init==-1){
    printf("Pb initialisation semaphore\n");
    /* On detruit les IPC deje crees : */
    semctl(semap,1,IPC_RMID,NULL);
    shmctl(mem_part,IPC_RMID,NULL);
    exit(-1);
    }

    file_mess = msgget(cle,IPC_CREAT | IPC_EXCL | 0660);
    if (file_mess==-1){
    fprintf(stderr,"Pb creation file de message\n");
    semctl(semap,1,IPC_RMID,NULL);
    shmctl(mem_part,IPC_RMID,NULL);
    exit(-1);
    }

    /* creation des archivistes */
    sprintf(arg1,"%i",argv1);
    sprintf(arg2,"%i",argv2);
    argv_archiv[1] = arg1;
    argv_archiv[2] = arg2;

    for(i = 0; i< argv1;i++){
      pid[i] = fork();
      if(pid[i] == -1)
	      break;
      if(pid[i] == 0){
        execve("./archiviste",argv_archiv,NULL);
        fprintf(stderr,"Creation des archivistes\n");
        execve("./journaliste",argv_journal,NULL);
        
        exit(-1);
      }
    }
    sleep(0.5);

    /* on cree des journalistes indefiniment */
      while(1){
        pid_journal = fork();
        if(pid_journal==-1){
           //error
        } 
        if(pid_journal==0){
            sprintf(arg1,"%i",identite_journal);
            argv_journal[1] = arg1;
            sprintf(arg2,"%i",argv1);
            argv_journal[2] = arg2;
            if(demande_archive==0){ //effacement
                argv_journal[3] = "e";
                execve("./journaliste",argv_journal,NULL);
            }
            else if(demande_archive<3){ // publication
              argv_journal[3] = "p";
              execve("./journaliste",argv_journal,NULL);
            }
            else { //consultation
              argv_journal[3] = "c";
              execve("./journaliste",argv_journal,NULL);
            }
            exit(-1);
        }
        else{
          demande_archive = rand()%(10);

            /* test valeur identite */
          if(identite_journal<20000)
              identite_journal+=1;
            else
              identite_journal= 10000;
        }
         //fprintf(stderr,"fdgfdg\n");
         sleep(1);
	    }



    //sleep(20);

    /*provisoire */
    semctl(semap,1,IPC_RMID,NULL);
    shmctl(mem_part,IPC_RMID,NULL);
    msgctl(file_mess,IPC_RMID, NULL);
     
  }

    
void usage(char * s){
    printf("Usage : %s <nb archiviste> <nb themes>\n",s);  
  }

int arg_valid(int  argv1, int  argv2){
    if(argv1 < 2 || argv1>100 || argv2 < 2 || argv2 >100)
      return 0;
    return 1; 
}

void mon_sigaction(int signal, void (*f)(int)){
    struct sigaction action;
 
    action.sa_handler = f;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(signal,&action,NULL);
}
void arret(int i ){
  fprintf(stderr,"arret %i", i);

   semctl(semap,1,IPC_RMID,NULL);
   shmctl(mem_part,IPC_RMID,NULL);
   msgctl(file_mess,IPC_RMID, NULL);
   sleep(1); // pour eviter de finir avant la suppression des IPCs
  // kill(getpid(),SIGKILL);
   exit(0);

}

