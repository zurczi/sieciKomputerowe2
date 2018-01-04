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

char znaki[6];
void childend(int signo){
wait(NULL);
printf("  KONIEC ");
    
}

int main(){
    
    struct sockaddr_in adres_serwer;
    struct sockaddr_in adres;
    
    signal(SIGCHLD,childend);
    
    
    adres_serwer.sin_family=PF_INET;
    adres_serwer.sin_port=htons(1234);

    //adres_serwer.sin_addr.s_addr=INADDR_ANY;
    adres_serwer.sin_addr.s_addr=inet_addr("192.168.0.15");
    
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
        int pom=read(nfd,znaki,sizeof(znaki));
    
        printf("\n ODEBRALEM INDEKS NR \n" );
        write(1,znaki,pom);
    
        char ciag1[150]="Piotr Pawlaczyk\n";
        char ciag2[150]="Kasia Jozwiak\n";
        char else1[10]="BLAD\n";
    
    
        if(strncmp(znaki,"127245",6)==0) {write(nfd,ciag1,16);}
        else if (strncmp(znaki,"127237",6)==0) {write(nfd,ciag2,14);}
        else (write(nfd,else1,5));
    
        close(nfd);
        exit(0);
    }
    close(nfd);
    
    }
    
  return 0;
}
