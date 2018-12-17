
#include "journaliste.h"

int main(int argc, char * argv[], char ** envp){
    if(argc!=4){
        usage(argv[0]);
        exit(-1);
    }

    fprintf(stderr, "%s | %s",argv[1], argv[3]);
    switch (argv[3][0]){
        case 'c':
            fprintf(stderr,"c");
            break;
        case 'p':
         fprintf(stderr,"p");
            break;
        case 'e':
         fprintf(stderr,"e");
            break;

    }
    fprintf(stderr,"test\n");
    sleep(1);
    exit(0);
   // return 0;

}

void usage(char * s){
    printf("Usage : %s <identite> <nb archiviste> <type requete>\n",s);  
  }