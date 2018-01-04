#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
char myNick[10];

void childend(int signo){
    wait(NULL);
    printf("KONIEC");
}

typedef struct uzytkownik{
    char nick[10];
    
}uzytkownik;


int main(){
    
   time_t curtime;
   time(&curtime);
    uzytkownik *tabUzytkownikow=malloc(20*sizeof(uzytkownik));
    int i;
    for (i=0;i<20;i++){
        strcpy(tabUzytkownikow[i].nick,"5");
    }
    
    struct sockaddr_in adres_serwer;
    struct sockaddr_in adres; //adres klienta
    
    signal(SIGCHLD,childend);
    
    
    adres_serwer.sin_family=PF_INET;
    adres_serwer.sin_port=htons(1234);

    //adres_serwer.sin_addr.s_addr=INADDR_ANY;
    adres_serwer.sin_addr.s_addr=inet_addr("192.168.0.15"); //adres serwera
    
    int fd = socket(PF_INET,SOCK_STREAM,0);
    int on=1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR, (char*)&on,sizeof(on));
    
    bind(fd,(struct sockaddr*)&adres_serwer,sizeof(adres_serwer));
    listen(fd,10);
    socklen_t len;
   
    while(1){
        
    int nfd=accept(fd,(struct sockaddr*)&adres, &len);

    if(fork()==0){
       
        close(fd);
        
        
        
        
        
        printf("Nowe polaczenie: %s\n", inet_ntoa((struct in_addr)adres.sin_addr)); //wyswietla info o tym kto sie podlaczyl
        int pom=read(nfd,myNick,sizeof(myNick));
    
        printf("\n ODEBRALEM NICK:\n" );
        write(1,myNick,pom);
    
        char zajetaNazwa[150]="Nazwa zajeta\n";
        char udaloSieZalogowac[150]="Udalo sie zalogowac\n";
        
        int j;
            int ile=0;
        for (j=0;j<20;j++){
            if(tabUzytkownikow[i].nick==myNick) ile++;
        }
        
        if(ile>0) write(nfd,zajetaNazwa,13);
        else write(nfd,udaloSieZalogowac,20);

    
        close(nfd);
        exit(0);
    }
    close(nfd);
    
    }
    
  return 0;
}
