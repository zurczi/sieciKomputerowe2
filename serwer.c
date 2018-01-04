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
#include <fcntl.h>


char myNick[10];
char wyborCHAR[2];
int wybor;

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
       //nfd=accept(fd,(struct sockaddr*)&adres, &len);
        close(fd);
        printf("\n\n\nNowe polaczenie: %s\n", inet_ntoa((struct in_addr)adres.sin_addr)); //wyswietla info o tym kto sie podlaczyl
        int pom=read(nfd,wyborCHAR,sizeof(wyborCHAR));
        
        printf("\nODEBRALEM WYBOR:  \n" );
        write(1,wyborCHAR,pom);
        
        wybor= wyborCHAR[0] - '0';
        
        
        char odebralemWybor[5]="11\n";
        write(nfd,odebralemWybor,3);
        
        switch(wybor){
            case 1:
                ;
                //nfd=accept(fd,(struct sockaddr*)&adres, &len);
                //odczyt pliku
                FILE *plik;
                plik=fopen("uzytkownicy.txt","rw");
                int plik2=open("uzytkownicy.txt",O_WRONLY|O_APPEND);
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
  
                printf("Przed odbiorem");
                //odbieranie danych od klienta
                //int pom=read(nfd,myNick,sizeof(myNick));
                int pom2;
                sleep(1);
                   // printf("Jestem w whiel\n");
                    pom2=read(nfd,myNick,sizeof(myNick));//}
                   // int pom=read(nfd,myNick,sizeof(myNick));}
                printf("\nODEBRALEM NICK:" );
                printf("Tu : %s",myNick);
                char zajetaNazwa[150]="100\n";
                char udaloSieZalogowac[150]="101\n";
        printf(" I tu :%d",myNick[0]);
                myNick[0]=0;
                //usuwanie entera z myNick
                for(j1=0;j1<10;j1++){
                     printf(" \nI tu :%d\n",myNick[j1]);
                    if((myNick[j1] <97 ||  myNick[j1]>122) && (myNick[j1] >57 ||  myNick[j1]<48)) {
                    myNick[j1]=0;
                    printf(" \nZmieninoe :%d\n",myNick[j1]);
                }}
                
                int y;
                int y2=0;
                char myNick2[10];
                for(y=0;y<10;y++){
                    if(myNick[y]!=0){ 
                        myNick2[y2]=myNick[y];
                        y2++;
                    }
                }
                
            printf(" \nI tu :%s\n",myNick2);
                int j;
                int ile=0;
                for (j=0;j<20;j++){
                    if(strncmp(tabUzytkownikow[j].nick,myNick2,10)==0) {
                        ile++;
                        //break;
                    }
                }

                //wywylanie ststusu zalogowania(czy udalo sie czy jest juz taki uzytkownik)
                if(ile>0) {
                    write(nfd,zajetaNazwa,4);
                    printf("Nazwa zajeta\n");
                }
                //zapisywanie do pliku
                else { 
                    printf("\nI AM HERE, login to: ");
                    printf("%s",myNick2);                    
            
                    //wpisanie nowego nicku do pliku
                    fflush(plik);
                    int a;
                    for(a=0;a<10;a++){
                        if(myNick2[a]>0) write(plik2,&myNick2[a],1);  
                    }
                    //<-dziala
                    write(plik2,"\n",1);
                    //fwrite(myNick,sizeof(myNick),10,plik);
                    //fprintf(plik,"%s",myNick);
            
                    write(nfd,udaloSieZalogowac,4);
                    }

        
                //pomocnicze wyswietlenie tablicy uzytkownikow
                int i=0;
                for(i=0;i<20;i++){
                    printf(" X:%s",tabUzytkownikow[i].nick);
                }
        
               close(nfd);
                fclose(plik);
                close(plik2);
                exit(0);
                
            case 2: printf("case 2");
            
        }
        close(nfd);///
    } //koniec forka
    close(nfd);
    }//koniec while
    
  return 0;
}
