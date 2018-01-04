#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

int main(){
    
    char *tabUzytkownikow=malloc(20*sizeof(int));
    int i;
    //for (i=0;i<20;i++){
      //  strncpy(&tabUzytkownikow[i],"8\n",1);
   // }
    
        FILE *plik;
        plik=fopen("test.txt","rw");
        
        int k;
        for(k=0;k<20;k++){
            fgets(&tabUzytkownikow[k],10,plik);
            printf("\nX: %s",&tabUzytkownikow[k]);
        }
        
        //fprintf ("2",plik);
        fprintf(plik,"%s","9");

        int j=0;
        for(j=0;j<20;j++){
         //  printf("\nX: %s",&tabUzytkownikow[j]);
        }
       // fflush(plik);
        fclose(plik);
    
  return 0;
}
