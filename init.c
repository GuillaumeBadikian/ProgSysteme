
#include "init.h"


int file_mess, mem_part, semap;


int main(int argc, char * argv[], char ** envp){
  
  //sigset_t masque, masque_attente;

  if(argc != 3){
    usage(argv[0]);
    exit(-1);
  }
  
  if(arg_valid(atoi(argv[1]),atoi(argv[2]))==0){
    printf("%s <nb archiviste> <nb themes> entre 2 et 100\n",argv[0]);
    exit(-1);
  }

   /* Masquage de tous les signaux */
  /*sigfillset(&masque);
  sigprocmask(SIG_SETMASK,&masque,NULL);
*/
  /* SIGUSR1 et SIGUSR2 demasques  */
/*  sigfillset(&masque_attente);
  sigdelset(&masque_attente,SIGUSR1);
  sigdelset(&masque_attente,SIGUSR2);
*/
  mon_sigaction(SIGUSR1, arret);
  mon_sigaction(SIGUSR2, arret);

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
    int i;
    
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

    for(i = 0; i< argv1;i++){
      pid[i] = fork();
      if(pid[i] == -1)
	      break;
      if(pid[i] == 0){
        //execve
        fprintf(stderr,"Creation des archivistes\n");
        exit(-1);
      }
    }


    /* on cree des journalistes indefiniment */
      while(1){
         //kill(pid[0],SIGUSR1);
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
   //sleep(5);
  // kill(getpid(),SIGKILL);
   exit(0);

}

