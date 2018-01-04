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


char myNick[10];

void childend(int signo){
    wait(NULL);
    printf("\nKONIEC");
}

typedef struct uzytkownik{
    char nick[10];
    
}uzytkownik;


int main(){
    
   time_t curtime;
   time(&curtime);
   
   
    uzytkownik *tabUzytkownikow=malloc(20*sizeof(uzytkownik));
    int i;

    
    
    //UZUPELNIC O WYPELNIANIE UZYTKOWNIKOW Z PLIKU
    
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
        
        
        FILE *plik;
        plik=fopen("uzytkownicy.txt","rw");
        
        int k;
        for(k=0;k<20;k++){
            fgets(tabUzytkownikow[k].nick,10,plik);
        }
        
        //usuwanie enterow
        
        int i1,j1;
        for (i1=0;i1<20;i1++){
            for(j1=0;j1<10;j1++){
                if((tabUzytkownikow[i1].nick[j1] <97 ||  tabUzytkownikow[i1].nick[j1]>122) && (tabUzytkownikow[i1].nick[j1] >57 ||  tabUzytkownikow[i1].nick[j1]<48)){
                    tabUzytkownikow[i1].nick[j1]=0;
                }}}
  

        
  
        printf("\n\n\nNowe polaczenie: %s\n", inet_ntoa((struct in_addr)adres.sin_addr)); //wyswietla info o tym kto sie podlaczyl
        int pom=read(nfd,myNick,sizeof(myNick));
    
        printf("\nODEBRALEM NICK:\n" );
        write(1,myNick,pom);
    
        char zajetaNazwa[150]="100\n";
        char udaloSieZalogowac[150]="101\n";
        
        
        //usuwanie entera z myNick
            for(j1=0;j1<10;j1++){
                if((myNick[j1] <97 ||  myNick[j1]>122) && (myNick[j1] >57 ||  myNick[j1]<48)){
                    myNick[j1]=0;
                }}
        
        int j;
        int ile=0;
        for (j=0;j<20;j++){
            if(strncmp(tabUzytkownikow[j].nick,myNick,10)==0) {
                ile++;
                break;
            }
        }
        printf("\nIle = %d\n",ile);

        
        if(ile>0) {
            write(nfd,zajetaNazwa,4);
            printf("Nazwa zajeta");
        }
        else { //zapisywanie do pliku
            
            printf("\nI AM HERE, login to: ");
            printf("%s",myNick);
            //strcpy(tabUzytkownikow[i].nick,myNick);
                    
            //wpisanie nowego nicku do pliku
            fprintf(plik,"%c",'h');        
            //tabUzytkownikow[i].nick="2";
          
            write(nfd,udaloSieZalogowac,4);
        }

        
        int i=0;
        for(i=0;i<20;i++){
            printf("X: %s",tabUzytkownikow[i].nick);
        }
        
        close(nfd);
        exit(0);
    } //koniec forka
    close(nfd);
    }//koniec while
    
  return 0;
}
