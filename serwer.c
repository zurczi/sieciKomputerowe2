#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>


struct sockaddr_in addr;
socklen_t rozmiar;

char buffor[125];
char odp[100];

struct cln {
    int cfd;
    struct sockaddr_in caddr;
};

typedef struct uzytkownik{
    char nick[15];
}uzytkownik;

typedef struct pokoj{
    char nazwa[15];
}pokoj;


int zamienNaLiczbe(int poczatekLiczby, char *buffor){
    
    int setki=buffor[poczatekLiczby]-'0';
    poczatekLiczby++;
    int dziesiatki=buffor[poczatekLiczby]-'0';
    poczatekLiczby++;
    int jednosci=buffor[poczatekLiczby]-'0';
    int liczba=100*setki+10*dziesiatki+jednosci;
    
    return liczba;
}


void wyslijZawartoscPliku(char *nazwaPliku ,struct cln* b){
     
    char tablicaDoWyslania[320];
                
    FILE *plik=fopen(nazwaPliku,"rw");
                
    int i=0; //ilosc znakow w pliku
    int pom;
    int znak;
    
    do{
        znak=getc(plik);
        tablicaDoWyslania[i]=znak;
        i++;
    }
    while(znak!=EOF);
    tablicaDoWyslania[i-1]='\n';
    fclose(plik);
    
    //wyslanie odczytanej zawartosci pliku do uzytkownika
    write(b->cfd,&tablicaDoWyslania,i);
    
    printf("\nWyslalem tablice:%s.",tablicaDoWyslania);
    
}


void* cthread (void* arg) {
    printf("\nJESTEM W WATKU");
    
    struct cln* c = (struct cln*)arg;
    uzytkownik *tabUzytkownikow = malloc(20*sizeof(uzytkownik));
    printf("\nPolaczylem sie z:%s.\n", inet_ntoa((struct in_addr)c->caddr.sin_addr));
    int pom,i,j,n;
    char pomCHAR;
    FILE *plik;
    FILE *plik2;
    int plik3;
    
    
    while(1){
       
        i=0;
        do{
            pom=read(c->cfd,&pomCHAR,1);
            buffor[i]=pomCHAR;
            //printf("\nPOM:%c.\n",pomCHAR);
            //sleep(3);
            i++;
        }
        while(i<125);
    
        int wybor=buffor[0]-'0';
       
        printf("\nWybrales opcje:%d.",wybor);
    
        switch(wybor){
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                                      L O G O W A N I E
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 1:{
                printf("\nCASE 1");
                int rozmiarNicka=zamienNaLiczbe(1,buffor);
                
                char nick[rozmiarNicka];
                memset(nick,0,sizeof(nick));
                printf("\nRomiar nicka:%d.",rozmiarNicka);
                //odczytywanie nicka
                n=0;
                int fordo=4+rozmiarNicka;
                
                for(i=4;i<fordo;i++){
                    nick[n]=buffor[i];
                    n++;
                }
                printf("\nNick to:%s.",nick);
                
                //odczytywanie obecnych uzytkownikow z pliku
                plik=fopen("uzytkownicy.txt","rw");
                
                //oczytywanie z pliku znak po znaku do tablicy uzytkownikow
                int znak;
                i=0;
                j=0;
                do{
                    znak=getc(plik);
                    if(znak!=9){ //\t =
                        tabUzytkownikow[i].nick[j]=znak;
                        j++;
                    }
                    else{
                        i++;
                        j=0;
                    }
                }
                while(znak!=EOF);
                
                //liczenie czy w pliku sa uzytkownicy o tej samej nazwie
                int ile=0;
                for(i=0;i<20;i++){
                    if(strncmp(tabUzytkownikow[i].nick,nick,rozmiarNicka)==0){
                        ile++;
                        break;
                    }
                }
                
                char nazwaWolna[2]="1\n";
                char nazwaZajeta[2]="2\n";
                
                //wczytywanie nowej nazwy do pliku
                plik3=open("uzytkownicy.txt", O_WRONLY|O_APPEND);
                
                if(ile==0){
                    write(plik3,&nick,rozmiarNicka);
                    write(plik3,"\t",1);
                    printf("\nZapisalem do pliku");
                    write(c->cfd,nazwaWolna,2);
                }
                else{
                    printf("\nNazwa uzytkownika zajeta");
                    write(c->cfd,nazwaZajeta,2);
                }
                
                //pomocnicze wyswietlenie tablicy uzytkownikow
                for(i=0;i<20;i++){
                    printf("X:%s.",tabUzytkownikow[i].nick);
                }
                memset(buffor,0,sizeof(buffor));
                fclose(plik);
                close(plik3);
                printf("\nZakonczylem case 1");
                break;
        }
                //TODO tablica zalogowanych cfd 
                
              
                
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                           W Y S Y L A N I E      L I S T Y     Z A L O G O W A N Y C H     U Z Y T K O W N I K O W 
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 2:
                printf("\nCASE 2");
                wyslijZawartoscPliku("uzytkownicy.txt",c);
                printf("\nZakonczylem case 2");
                break;
                
                //TODO czy tu nie musi byc cos pozamykane
                
          
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                      W Y S Y L A N I E      L I S T Y    P O K O I
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 3:
                printf("\nCASE 3");
                wyslijZawartoscPliku("pokoje.txt",c);
                printf("\nZakonczylem case 2");
                break;
                              
                //TODO czy tu nie musi byc cos pozamykane
                
               
                
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                    W Y S Y L A N I E      L I S T Y    U Z Y T K O W N I K O W      W      D A N Y M     P O  K O J U  
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 4:{
                printf("\nCASE 4");
                int rozmiarPokoju=zamienNaLiczbe(1,buffor);
                
                char nazwaPokoju[rozmiarPokoju];
                memset(nazwaPokoju,0,sizeof(nazwaPokoju));
                printf("\nRomiar pokoju:%d.",rozmiarPokoju);
                //odczytywanie nazwy
                n=0;
                int fordo=4+rozmiarPokoju;
                
                for(i=4;i<fordo;i++){
                    nazwaPokoju[n]=buffor[i];
                    n++;
                }
                printf("\nNazwa to:%s.",nazwaPokoju);
                
                
                wyslijZawartoscPliku(nazwaPokoju,c);
                
                printf("\nZakonczylem case 4");
                break;
                
            }
                
                
                 /* 
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                                  N O W A     G R U P A
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //case 5:
                
                
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                               W Y S Y L A N I E      W I AD O M O S C I     D O     P O K O J U
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //case 6:
         
            */
        
        
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                                     W Y L O G O W A N I E
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 7:{
                printf("\nCASE 7");
                
                int rozmiarNicka7=zamienNaLiczbe(1,buffor);
                
                char nick7[rozmiarNicka7];
                
                //odczytywanie nicka
                n=0;
                for(i=4;i<4+rozmiarNicka7;i++){
                    nick7[n]=buffor[i];
                    n++;
                }
                printf("\nNick to:%s.",nick7);
                
                //TODO usuwanie z tablicy zalogowanych
                
                close(c->cfd);
                free(c);
                break;
            }
       
        }
    

    }


    
    //sleep(3);
   // write(c->cfd, odp, strlen(odp));
    close(c->cfd);
   // printf("Wyslano do: %s message: %s\n", inet_ntoa((struct in_addr)c->caddr.sin_addr), odp);
    free(c);
    return 0;
}



int main()
{
    pthread_t tid;
    
    uint16_t port = 1234;
    char ip[30] = "192.168.0.15";//INADDR_ANY;
    //strcpy(ip,INADDR_ANY);
    addr.sin_family = PF_INET;
    addr.sin_port = htons(port);
    printf("START\n");
    addr.sin_addr.s_addr = INADDR_ANY;
    
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    int on = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    
    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(fd, 5);         //fd zawsze ten z funkcji socket, liczba oczekujacych polaczen
    
    while(1)
    {
        printf("\nJESTEM W WHILE");
        struct cln* c = malloc(sizeof(struct cln));
        rozmiar = sizeof(c->caddr);
        c->cfd = accept(fd, (struct sockaddr*)&c->caddr, &rozmiar); // blokujaca, przekazuje IP i adres portu klienta, zwraca deskryptor nowego gniazda - sluzy tylko do komunikacji z tym klientem, gniazdo glowne jest stosowane tylko do akceptacji
        //read/write z new_fd(nwd)
        pthread_create(&tid, NULL, cthread, c);
        pthread_detach(tid);
        
    };
    
    close(fd);
    
}