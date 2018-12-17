#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "init.h"


int main(int argc, char * argv[], char ** envp){
  if(argc != 3){
    usage(argv[0]);
    exit(-1);
  }
  
  if(arg_valid(atoi(argv[1]),atoi(argv[2]))==0){
    printf("%s <nb archiviste> <nb themes> entre 2 et 100\n",argv[0]);
    exit(-1);
  }

  /* on peut commencer
   * les parametres sont bons
   */

  
  
}


void usage(char * s){
  printf("Usage : %s <nb archiviste> <nb themes>\n",s);  
}

int arg_valid(int  argv1, int  argv2){
  if(argv1 < 2 || argv1>100 || argv2 < 2 || argv2 >100)
    return 0;
  return 1; 
}
