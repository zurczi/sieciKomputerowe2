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
    int nfd;
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

char* pobierzDane(int rozmiar, char *buffor,int poczatek){
            
    char *dane = malloc (sizeof (char) * rozmiar);
    //memset(dane,0,(sizeof(char)*rozmiar));
    printf("\nRozmiar danych:%d.",rozmiar);
    //odczytywanie nazwy
    int n=0;
    int i;
    int fordo=poczatek+rozmiar;
    memset(dane,0,(sizeof(char)*rozmiar));
    for(i=4;i<fordo;i++){
        dane[n]=buffor[i];
        n++;
    }
    printf("\nDane to:%s.\n",dane);
    
    return dane;
}


void wyslijZawartoscPliku(char *nazwaPliku ,struct cln* b,int ktoryCase){
     
    char tablicaDoWyslania[322];
    tablicaDoWyslania[0]=ktoryCase+'0';
    tablicaDoWyslania[1]='\t';
    FILE *plik=fopen(nazwaPliku,"rw");
                
    int i=2; //ilosc znakow w pliku
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
    int i,j;
    char pomCHAR;
    FILE *plik;
    int plik3;
    
    
    while(1){
        memset(buffor,0,sizeof(buffor));
        i=0;
        do{
            read(c->cfd,&pomCHAR,1);
            buffor[i]=pomCHAR;
           // printf("\nPOM:%c.\n",pomCHAR);
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
                printf("\n\n\nCASE 1");
                
                int rozmiarNicka=zamienNaLiczbe(1,buffor);
                
                char * nick = malloc (sizeof (char) * rozmiarNicka);
                nick = pobierzDane(rozmiarNicka,buffor,4);
                printf("\nnick to:%s.",nick);

                
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
                        tabUzytkownikow[i].nfd=c->cfd;
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
                
                char nazwaWolna[3]="11\n";
                char nazwaZajeta[3]="12\n";
                
                //wczytywanie nowej nazwy do pliku
                plik3=open("uzytkownicy.txt", O_WRONLY|O_APPEND);
                
                if(ile==0){
                    write(plik3,nick,rozmiarNicka);
                    write(plik3,"\t",1);
                    printf("\nZapisalem do pliku");
                    write(c->cfd,nazwaWolna,3);
                }
                else{
                    printf("\nNazwa uzytkownika zajeta");
                    //TODO jezeli nie ma tkiego w pliku to go wpisuje, jak jest to loguje, chyba ze jest w tab zalogowanychto wywalam
                    write(c->cfd,nazwaZajeta,3);
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
            case 2:{
                printf("\n\n\nCASE 2");
                wyslijZawartoscPliku("uzytkownicy.txt",c,2);
                printf("\nZakonczylem case 2");
                break;
                
                //TODO czy tu nie musi byc cos pozamykane
                
            }
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                      W Y S Y L A N I E      L I S T Y    P O K O I
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 3:{
                printf("\n\n\nCASE 3");
                wyslijZawartoscPliku("pokoje.txt",c,3);
                printf("\nZakonczylem case 2");
                break;
                              
                //TODO czy tu nie musi byc cos pozamykane
                
            }
                
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                    W Y S Y L A N I E      L I S T Y    U Z Y T K O W N I K O W      W      D A N Y M     P O  K O J U  
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 4:{
                printf("\n\n\nCASE 4");
                int rozmiarPokoju=zamienNaLiczbe(1,buffor);
                
                char * nazwaPokoju = malloc (sizeof (char) * rozmiarPokoju);
                nazwaPokoju = pobierzDane(rozmiarPokoju,buffor,4);
                
                printf("\nNazwa to:%s.\n",nazwaPokoju);
                
                wyslijZawartoscPliku(nazwaPokoju,c,4);
                
                printf("\nZakonczylem case 4");
                break;
                
            }
                
                
                 
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                                  N O W A     G R U P A
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 5:{
                printf("\n\n\nCASE 5");
                
                int rozmiar=zamienNaLiczbe(1,buffor);
                
                //char nazwa[rozmiar];
                //strncpy(nazwa,pobierzDane(rozmiar,buffor,4),rozmiar);
                
                char * nazwa = malloc (sizeof (char) * rozmiar);
                nazwa = pobierzDane(rozmiar,buffor,4);
      
                printf("\nNazwa to:%s.\n",nazwa);
                
                
                //odczyt nicku admina
                int x=4+rozmiar;
                int rozmiarAdmina=zamienNaLiczbe(x,buffor);
                
                //char nickAdmina[rozmiarAdmina];
             
                int pocz=4+rozmiar+rozmiarAdmina+3;
                char * nickAdmina = malloc (sizeof (char) * rozmiarAdmina);
                nickAdmina = pobierzDane(rozmiarAdmina,buffor,pocz);
                
                //strncpy(nickAdmina,pobierzDane(rozmiarAdmina,buffor,pocz),rozmiarAdmina);
                printf("\nNick admina to:%s.",nickAdmina);
                
                
                //odczyt listy uzytkownikow
                int rozmiarListy=zamienNaLiczbe(rozmiar,buffor);
                
                //char lista[rozmiarListy];
 
                pocz=pocz+rozmiarListy+3;
                
                char * lista = malloc (sizeof (char) * rozmiarListy);
                lista = pobierzDane(rozmiarListy,buffor,pocz);
                
                //strncpy(lista,pobierzDane(rozmiarListy,buffor,pocz),rozmiarListy);
                printf("\nLista uzytkownikow to:%s.",lista);
                
                
                //utworzenie i zapis do pliku
                int nowyPlik=open(nazwa,O_WRONLY|O_APPEND);
                write(nowyPlik,nickAdmina,rozmiarAdmina);
                write(nowyPlik,"\t",1);
                write(nowyPlik,lista,rozmiarListy);
                
                //wysylanie potwierdzenia utworzenia
                char potwierdzenie[3]="51\n";
                write(c->cfd,potwierdzenie,3);
                              
                printf("\nZakonczylem case 5");
                close(nowyPlik);
                break;
                
            }
            
                
                
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                               W Y S Y L A N I E      W I A D O M O S C I     D O     P O K O J U
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 6:{
                printf("\n\n\nCASE 6");
                
                //odczyt nicku nadawcy
                int rozmiarNickuNadawcy=zamienNaLiczbe(1,buffor);
                
                //char nickNadawcy[rozmiarNickuNadawcy];
                //strncpy(nickNadawcy,pobierzDane(rozmiarNickuNadawcy,buffor,4),rozmiarNickuNadawcy);
                char * nickNadawcy = malloc (sizeof (char) * rozmiarNickuNadawcy);
                nickNadawcy = pobierzDane(rozmiarNickuNadawcy,buffor,4);
                
                printf("\nNick nadawcy to:%s.\n",nickNadawcy);
                
                
                //odczyt nazwy pokoju
                int x=4+rozmiarNickuNadawcy;
                int rozmiarPokoju=zamienNaLiczbe(x,buffor);
                
                //char nazwaPokoju[rozmiarPokoju];
                
                int pom=4+rozmiarNickuNadawcy+3;
                //strncpy(nazwaPokoju,pobierzDane(rozmiarPokoju,buffor,pom),rozmiarPokoju);
                char * nazwaPokoju = malloc (sizeof (char) * rozmiarPokoju);
                nazwaPokoju = pobierzDane(rozmiarPokoju,buffor,pom);
               
                printf("\nNazwa Pokojuto:%s.",nazwaPokoju);
                
                
                //odczyt tresci wiadomosci
                int rozmiarWiadomosci=zamienNaLiczbe(pom,buffor);
                //char wiadomosc[rozmiarWiadomosci];
                
                pom=pom+rozmiarPokoju+3;
                //strncpy(nazwaPokoju,pobierzDane(rozmiarPokoju,buffor,pom),rozmiarPokoju);
                char * wiadomosc = malloc (sizeof (char) * rozmiarWiadomosci);
                wiadomosc = pobierzDane(rozmiarPokoju,buffor,pom);
                
                printf("\nTresc wiadomosci to:%s.",wiadomosc);
                  
                //odczytywanie z pliku nickow osob znajdujacych sie w pokoju
                FILE *plik=fopen(nazwaPokoju,"rw");
                uzytkownik *tabUzytkownikowwPokoju = malloc(20*sizeof(uzytkownik));
                
                int znak;
                i=0;
                j=0;
                do{
                    znak=getc(plik);
                    if(znak!=9){ //\t =
                        tabUzytkownikowwPokoju[i].nick[j]=znak;
                        j++;
                    }
                    else{
                        i++;
                        j=0;
                    }
                }
                while(znak!=EOF);
                fclose(plik);
                
                //przygotowanie wiadomosci do tablicaDoWyslania
                char * gotowaWiadomosc = malloc (sizeof (char) * (1+1+rozmiarNickuNadawcy+1+rozmiarPokoju+1+rozmiarWiadomosci+1));
                int skip = 0;
                gotowaWiadomosc[0]='6';
                gotowaWiadomosc[1]='\t';
                skip += 2;
                for(i=0;i<rozmiarNickuNadawcy;i++){
                    gotowaWiadomosc[skip+i]=nickNadawcy[i];
                }
                skip +=rozmiarNickuNadawcy;
                gotowaWiadomosc[skip]='\t';
                skip+=1;
                
                for(i=0;i<rozmiarPokoju;i++){
                    gotowaWiadomosc[skip+i]=nazwaPokoju[i];
                }
                skip +=rozmiarPokoju;
                gotowaWiadomosc[skip]='\t';
                skip+=1;
                
                for(i=0;i<rozmiarWiadomosci;i++){
                    gotowaWiadomosc[skip+i]=wiadomosc[i];
                }
                skip +=rozmiarWiadomosci;
                gotowaWiadomosc[skip]='\n';
                skip+=1;
                     
                printf("\nWiadomosc:%s.",gotowaWiadomosc);
                
                // uzytkownicy z pokoju
                for(i=0;i<20;i++){
                    if(strcmp(tabUzytkownikowwPokoju[i].nick,nickNadawcy)!=0){
                        for(j=0;j<20;j++){
                            if(strcmp(tabUzytkownikow[j].nick,tabUzytkownikowwPokoju[i].nick)==0){
                                write(tabUzytkownikow[j].nfd,gotowaWiadomosc,skip);
                            }
                        }
                    }
                }
                //wysylanie potwierdzenia utworzeniatabUzytkownikow
                //char potwierdzenie[2]="6\n";
                //write(c->cfd,potwierdzenie,2);
                
                printf("\nZakonczylem case 6");
                break;
            }
        
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                                     W Y L O G O W A N I E
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 7:{
                printf("\n\n\nCASE 7");
                
                int rozmiarNicka7=zamienNaLiczbe(1,buffor);
                //char nick7[rozmiarNicka7];
                
                char * nick7 = malloc (sizeof (char) * rozmiarNicka7);
                nick7 = pobierzDane(rozmiarNicka7,buffor,4);
                
                //strncpy(nick7,pobierzDane(rozmiarNicka7,buffor,4),rozmiarNicka7);
                printf("\nNick to:%s.",nick7);
                
                //TODO usuwanie z tablicy zalogowanych
                
                close(c->cfd);
                free(c);
                break;
            }
            
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                                       E D Y C J A    G R U P Y
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //case 8:
            
                default : {
                printf("\n\nNie znaleziono polcenia\n");
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
    //char ip[30] = "192.168.0.15";//INADDR_ANY;
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