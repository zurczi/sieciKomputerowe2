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

typedef struct pokoj{
    char nazwa[10];
    
}pokoj;


int main(){
    
   time_t curtime;
   time(&curtime);
   
   
    uzytkownik *tabUzytkownikow=malloc(20*sizeof(uzytkownik));
    pokoj *tabPokoi=malloc(20*sizeof(pokoj));
    uzytkownik *tabListaOsobwPokoju=malloc(20*sizeof(uzytkownik));
    
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
        
        int pom=read(nfd,wyborCHAR,sizeof(wyborCHAR));
        printf("\n\n\nNowe polaczenie: %s\n", inet_ntoa((struct in_addr)adres.sin_addr)); //wyswietla info o tym kto sie podlaczyl
        printf("\nODEBRALEM WYBOR:  \n" );
        write(1,wyborCHAR,pom);
        
        wybor= wyborCHAR[0] - '0';
        
        
        char odebralemWybor[5]="11\n";
        write(nfd,odebralemWybor,3);
        
        
        //deklaracja zmiennych
        FILE *plik;
        int i1,j1,k;
        int d,e;
        int y;
            int y2;
        int pom2;
        int x=0;
        sleep(10);
        switch(wybor){
            case 1:
                ;
                //nfd=accept(fd,(struct sockaddr*)&adres, &len);
                //odczyt pliku
                //FILE *plik;
                plik=fopen("uzytkownicy.txt","rw");
                int plik2=open("uzytkownicy.txt",O_WRONLY|O_APPEND);
                //int k;
                for(k=0;k<20;k++){
                fgets(tabUzytkownikow[k].nick,10,plik);
                }
                
                //usuwanie enterow
                //int i1,j1;
                for (i1=0;i1<20;i1++){
                    for(j1=0;j1<10;j1++){
                        if((tabUzytkownikow[i1].nick[j1] <97 ||  tabUzytkownikow[i1].nick[j1]>122) && (tabUzytkownikow[i1].nick[j1] >57 ||  tabUzytkownikow[i1].nick[j1]<48)){
                    tabUzytkownikow[i1].nick[j1]=0;
                }}} 
  
                printf("Przed odbiorem");
                //odbieranie danych od klienta
                //int pom2;
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
                
                y2=0;
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
                
            case 2: 
                printf("case 2");
                //sleep(1);
                plik=fopen("uzytkownicy.txt","rw");
               
                for(k=0;k<20;k++){
                fgets(tabUzytkownikow[k].nick,10,plik);
                }
                
                //usuwanie enterow
                //int i1,j1;
                for (i1=0;i1<20;i1++){
                    for(j1=0;j1<10;j1++){
                        if((tabUzytkownikow[i1].nick[j1] <97 ||  tabUzytkownikow[i1].nick[j1]>122) && (tabUzytkownikow[i1].nick[j1] >57 ||  tabUzytkownikow[i1].nick[j1]<48)){
                    tabUzytkownikow[i1].nick[j1]=0;
                }}} 
  
                
                
                char nazwyUzytkownikow[300]="100\n";
                
                //tworzenie tablicy nazw uzytkownikow do wyslania do klienta; nazwy uz. oddzielone sa tabulatorem
                x=0;
                for(d=0;d<20;d++){
                    for(e=0;e<=10;e++){
                        if(e==10) {
                            nazwyUzytkownikow[x]='\t';
                            x++;
                            break;
                        }
                        if(tabUzytkownikow[d].nick[e]!=0){
                            nazwyUzytkownikow[x]=tabUzytkownikow[d].nick[e];
                            x++;
                        }
                    }
                }
                nazwyUzytkownikow[x-1]='\n';
                
                write(nfd,&nazwyUzytkownikow,x);
                
              /*
                //wysylanie znak po znaku
                x=0;
                
                while(nazwyUzytkownikow[x]>0){
                    write(nfd,&nazwyUzytkownikow[x],1);
                    x++;
                }
                */
                    
                //pomocnicze wyswietlenie tablicy nazw uzytkownikow
                printf(" X:%s",nazwyUzytkownikow);
                
                close(nfd);
                fclose(plik);
                //close(plik2);
                exit(0);
            
            case 3:
                printf("case 2");
                //sleep(1);
                plik=fopen("pokoje.txt","rw");
               
                for(k=0;k<20;k++){
                fgets(tabPokoi[k].nazwa,10,plik);
                }
                
                //usuwanie enterow
                //int i1,j1;
                for (i1=0;i1<20;i1++){
                    for(j1=0;j1<10;j1++){
                        if((tabPokoi[i1].nazwa[j1] <97 ||  tabPokoi[i1].nazwa[j1]>122) && (tabPokoi[i1].nazwa[j1] >57 ||  tabPokoi[i1].nazwa[j1]<48)){
                    tabPokoi[i1].nazwa[j1]=0;
                }}} 
  
                
                
                char nazwyPokoi[300]="100\n";
                
                //tworzenie tablicy nazw uzytkownikow do wyslania do klienta; nazwy uz. oddzielone sa tabulatorem
                x=0;
                for(d=0;d<20;d++){
                    for(e=0;e<=10;e++){
                        if(e==10) {
                            nazwyPokoi[x]='\t';
                            x++;
                            break;
                        }
                        if(tabPokoi[d].nazwa[e]!=0){
                            nazwyPokoi[x]=tabPokoi[d].nazwa[e];
                            x++;
                        }
                    }
                }
                nazwyPokoi[x-1]='\n';
                
                write(nfd,&nazwyPokoi,x);
                
              /*
                //wysylanie znak po znaku
                x=0;
                
                while(nazwyPokoi[x]>0){
                    write(nfd,&nazwyPokoi[x],1);
                    x++;
                }
                */
                    
                //pomocnicze wyswietlenie tablicy nazw uzytkownikow
                printf(" X:%s",nazwyPokoi);
                
                close(nfd);
                fclose(plik);
                //close(plik2);
                exit(0);
            
            case 4:
                printf("case 4");
                //sleep(1);
                
                //Odbieranie nazwy pokoju
                char nazwaPokoju[10];
                
                sleep(1);
                pom2=read(nfd,nazwaPokoju,sizeof(nazwaPokoju));
                printf("\nOdebralem nazwe pokoju" );
                
                //char odebralemNazwaPokoju[4]="200\n";
                //write(nfd,odebralemNazwaPokoju,4);
                printf("nazwa pokoju przed: %s",nazwaPokoju);
                
                 for(j1=0;j1<10;j1++){
                     printf("W forze:  --%d--",nazwaPokoju[j1]);
                     if((nazwaPokoju[j1] <97 ||  nazwaPokoju[j1]>122) && (nazwaPokoju[j1] >57 ||  nazwaPokoju[j1]<48)) {
                         printf("W forze     5555:  --%d--",nazwaPokoju[j1]);   
                         nazwaPokoju[j1]=0;
                            
                }}
                
                
                y2=0;
                char nazwaPokoju2[10];
                for(y=0;y<10;y++){
                    if(nazwaPokoju2[y]!=0){ 
                        nazwaPokoju2[y2]=nazwaPokoju2[y];
                        y2++;
                    }
                }
                
                
                printf("\nnazwa pokoju po: %s\n",nazwaPokoju2);
                
                
                char sciezka[16] = "\"";
                strcat(sciezka,nazwaPokoju2);
                strcat(sciezka,".txt\"");
                
                printf("Sciezka po: %s",sciezka);
                
                plik=fopen(sciezka,"rw");
                
                
                
                for(k=0;k<20;k++){
                fgets(tabListaOsobwPokoju[k].nick,10,plik);
                }
                
                //usuwanie enterow
                //int i1,j1;
                for (i1=0;i1<20;i1++){
                    for(j1=0;j1<10;j1++){
                        if((tabListaOsobwPokoju[i1].nick[j1] <97 ||  tabListaOsobwPokoju[i1].nick[j1]>122) && (tabListaOsobwPokoju[i1].nick[j1] >57 ||  tabListaOsobwPokoju[i1].nick[j1]<48)){
                    tabListaOsobwPokoju[i1].nick[j1]=0;
                }}} 
  
                
                
                char listaUzytkownikow[300]="100\n";
                
                //tworzenie tablicy nazw uzytkownikow do wyslania do klienta; nazwy uz. oddzielone sa tabulatorem
                x=0;
                for(d=0;d<20;d++){
                    for(e=0;e<=10;e++){
                        if(e==10) {
                            listaUzytkownikow[x]='\t';
                            x++;
                            break;
                        }
                        if(tabListaOsobwPokoju[d].nick[e]!=0){
                            listaUzytkownikow[x]=tabListaOsobwPokoju[d].nick[e];
                            x++;
                        }
                    }
                }
                listaUzytkownikow[x-1]='\n';
                
                write(nfd,&listaUzytkownikow,x);
                
              /*
                //wysylanie znak po znaku
                x=0;
                
                while(listaUzytkownikow[x]>0){
                    write(nfd,&listaUzytkownikow[x],1);
                    x++;
                }
                */
                    
                //pomocnicze wyswietlenie tablicy nazw uzytkownikow
                printf(" X:%s",listaUzytkownikow);
                
                close(nfd);
                fclose(plik);
                //close(plik2);
                exit(0);
                
            
        }
        close(nfd);///
    } //koniec forka
    close(nfd);
    }//koniec while
    
  return 0;
}
