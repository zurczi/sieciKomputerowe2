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


uzytkownik *tabUzytkownikow;// = malloc(20*sizeof(uzytkownik));
//memset(tabUzytkownikow,0,(20*sizeof(uzytkownik)));

int zamienNaLiczbe(int poczatekLiczby, char *buffor){
    
    int setki=buffor[poczatekLiczby]-'0';
    poczatekLiczby++;
    int dziesiatki=buffor[poczatekLiczby]-'0';
    poczatekLiczby++;
    int jednosci=buffor[poczatekLiczby]-'0';
    int liczba=100*setki+10*dziesiatki+jednosci;
    
    return liczba;
}

char* pobierzDane(int rozmiar, char buffor1[],int poczatek){
            
    char *dane = malloc (sizeof (char) * (rozmiar+1));
    //memset(dane,0,(sizeof(char)*rozmiar));
   // printf("\nRozmiar danych:%d.",rozmiar);
    //odczytywanie nazwy
    int n=0;
    int i;
    int fordo=poczatek+rozmiar;
    memset(dane,0,(sizeof(char)* (rozmiar+1)));
    for(i=poczatek;i<fordo;i++){
       // printf("\n%d %d. %c.",n,i,buffor1[i]);
        dane[n]=buffor1[i];
        n++;
    }
    dane[n]='\0';

   // printf("\nDane to:%s.\n",dane);
    
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
    
    //tablica aktualnie zalogowanych uzytkownikow
    //uzytkownik *tabUzytkownikow = malloc(20*sizeof(uzytkownik));
    //memset(tabUzytkownikow,0,(20*sizeof(uzytkownik)));
    
    
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
        
        if(wybor<0) {
            exit(0);
            printf("Jestem mniejszy od zera");
        }
        
        
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
                
                
                uzytkownik *tabUzytkownikowWPliku = malloc(20*sizeof(uzytkownik));
                //oczytywanie z pliku znak po znaku do tablicy uzytkownikow
                int znak;
                i=0;
                j=0;
                do{
                    znak=getc(plik);
                    if(znak!=9){ //\t =
                        tabUzytkownikowWPliku[i].nick[j]=znak;
                        tabUzytkownikowWPliku[i].nfd=c->cfd;
                        j++;
                    }
                    else{
                        i++;
                        j=0;
                    }
                }
                while(znak!=EOF);
                
                //liczenie czy w tablicy zalogowanych sa uzytkownicy o tej samej nazwie
                int ile=0;
                for(i=0;i<20;i++){
                    if(strncmp(tabUzytkownikow[i].nick,nick,rozmiarNicka)==0){
                        ile++;
                        break;
                    }
                }
                
                
                //liczenie czy w tablicy wszystkich uzytkownikow sa uzytkownicy o tej samej nazwie
                int ileWPliku=0;
                for(i=0;i<20;i++){
                    if(strncmp(tabUzytkownikowWPliku[i].nick,nick,rozmiarNicka)==0){
                        ileWPliku++;
                        break;
                    }
                }
                
                char udaloSieZalogowac[3]="11\n";
                char juzZalogowany[3]="12\n";
                
                //wczytywanie nowej nazwy do pliku
                plik3=open("uzytkownicy.txt", O_WRONLY|O_APPEND);
               
                //znajdowanie indeksu pierwszego wolnego miejsca w tab uzyta
                int indeks=0;
                for(i=0;i<20;i++){
                    if(tabUzytkownikow[i].nick[0]==0)
                       break;
                }
                indeks=i;
                
                printf("\nIle w pliku:%d",ileWPliku);
                printf("\nIle:%d",ileWPliku);
                
                if(ileWPliku==0){
                    write(plik3,nick,rozmiarNicka);
                    write(plik3,"\t",1);
                    printf("\nZapisalem do pliku");
                    write(c->cfd,udaloSieZalogowac,3);
                    strcpy(tabUzytkownikow[indeks].nick,nick);
                    tabUzytkownikow[indeks].nfd=c->cfd;
                    printf("\nCFD UZYTKOWNIKA:%d\n",tabUzytkownikow[indeks].nfd); 
                }
                if(ileWPliku>0 && ile==0){
                    printf("\nNazwa istnieje, ale nie jest zalogowany, loguje");
                    strcpy(tabUzytkownikow[indeks].nick,nick);
                    tabUzytkownikow[indeks].nfd=c->cfd;
                    write(c->cfd,udaloSieZalogowac,3);
                    printf("\nCFD UZYTKOWNIKA:%d\n",tabUzytkownikow[indeks].nfd); 
                }
                
                if(ileWPliku>0 && ile>0){
                    printf("\nUzytkownik o podanej nazwie jest juz zalogowany");
                    write(c->cfd,juzZalogowany,3);
                    close(c->cfd);
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
                
                
              
                
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                           W Y S Y L A N I E      L I S T Y     Z A L O G O W A N Y C H     U Z Y T K O W N I K O W 
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 2:{
                printf("\n\n\nCASE 2");
                wyslijZawartoscPliku("uzytkownicy.txt",c,2);
                printf("\nZakonczylem case 2");
                break;                
            }
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                      W Y S Y L A N I E      L I S T Y    P O K O I
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 3:{
                printf("\n\n\nCASE 3");
                wyslijZawartoscPliku("pokoje.txt",c,3);
                printf("\nZakonczylem case 2");
                break;                
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
                
                int rozmiarNazwy=zamienNaLiczbe(1,buffor);
                char * nazwa = malloc (sizeof (char) * rozmiarNazwy);
                nazwa = pobierzDane(rozmiarNazwy,buffor,4);
                printf("\nNazwa to:%s.\n",nazwa);
                
                
                //odczyt nicku admina
                int polozenieRozmiaru=4+rozmiarNazwy;
                int rozmiarAdmina=zamienNaLiczbe(polozenieRozmiaru,buffor);
                
             
                int skadOdczytywac=polozenieRozmiaru+3;
                char * nickAdmina = malloc (sizeof (char) * rozmiarAdmina);
                nickAdmina = pobierzDane(rozmiarAdmina,buffor,skadOdczytywac);
                printf("\nNick admina to:%s.",nickAdmina);
                
                
                
                //odczytywanie nazwy pokoju bez .txt
                int polozenieCzystejNazwy=skadOdczytywac+rozmiarAdmina;
                int rozmiarCzystejNazwy=zamienNaLiczbe(polozenieCzystejNazwy,buffor);
                
                skadOdczytywac=polozenieCzystejNazwy+3;
                char * czystaNazwa = malloc (sizeof (char) * rozmiarCzystejNazwy);
                czystaNazwa = pobierzDane(rozmiarCzystejNazwy,buffor,skadOdczytywac);
                printf("\nCzysta nazwa pokoju to:%s.",czystaNazwa);
                
                //zapis do pliku nazwy pokoju
                plik3=open("pokoje.txt", O_WRONLY|O_APPEND);
                write(plik3,czystaNazwa,rozmiarCzystejNazwy);
                write(plik3,"\t",1);
                close(plik3);
                
                //odczyt listy uzytkownikow
                polozenieRozmiaru=skadOdczytywac+rozmiarCzystejNazwy;
                skadOdczytywac=polozenieRozmiaru+3;
                
                int rozmiarListy=zamienNaLiczbe(polozenieRozmiaru,buffor);
                char * lista = malloc (sizeof (char) * rozmiarListy);
                lista = pobierzDane(rozmiarListy,buffor,skadOdczytywac);
                printf("\nLista uzytkownikow to:%s.",lista);
                
                //sleep(10);
                //utworzenie i zapis do pliku
                plik=fopen(nazwa,"aw+");  
                fclose(plik);
                int nowyPlik=open(nazwa,O_WRONLY|O_APPEND);        //dwa razy ten sam plik ??   tu dopisalam create
                write(nowyPlik,nickAdmina,rozmiarAdmina);
                write(nowyPlik,"\t",1);
                write(nowyPlik,lista,rozmiarListy);
                close(nowyPlik);
                
                //wysylanie potwierdzenia utworzenia
                char potwierdzenie[4]="5\t1\n";
                write(c->cfd,potwierdzenie,4);
                              
                printf("\nZakonczylem case 5");
                
              //  fclose(plik);   TU ZAKOMENTOWALAM 
                break;
                
            }
            
                
                
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                               W Y S Y L A N I E      W I A D O M O S C I     D O     P O K O J U
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 6:{
              //  printf("\n\n\nCASE 6");
                
                //odczyt nicku nadawcy
                int rozmiarNickuNadawcy=zamienNaLiczbe(1,buffor);
                
                //char nickNadawcy[rozmiarNickuNadawcy];
                //strncpy(nickNadawcy,pobierzDane(rozmiarNickuNadawcy,buffor,4),rozmiarNickuNadawcy);
                char * nickNadawcy = malloc (sizeof (char) * rozmiarNickuNadawcy);
                nickNadawcy = pobierzDane(rozmiarNickuNadawcy,buffor,4);
                
              // printf("\nNick nadawcy to:%s.\n",nickNadawcy);
                
                
                //odczyt nazwy pokoju
                int x=4+rozmiarNickuNadawcy;
                int rozmiarPokoju=zamienNaLiczbe(x,buffor);
                
                //char nazwaPokoju[rozmiarPokoju];
                
                int pom=4+rozmiarNickuNadawcy+3;
                //strncpy(nazwaPokoju,pobierzDane(rozmiarPokoju,buffor,pom),rozmiarPokoju);
                char * nazwaPokoju = malloc (sizeof (char) * rozmiarPokoju);
                nazwaPokoju = pobierzDane(rozmiarPokoju,buffor,pom);
               
            //    printf("\nNazwa Pokojuto:%s.",nazwaPokoju);
                
                
                //odczyt tresci wiadomosci
                int rozmiarWiadomosci=zamienNaLiczbe(pom,buffor);
                //char wiadomosc[rozmiarWiadomosci];
                
                pom=pom+rozmiarPokoju+3;
                //strncpy(nazwaPokoju,pobierzDane(rozmiarPokoju,buffor,pom),rozmiarPokoju);
                char * wiadomosc = malloc (sizeof (char) * rozmiarWiadomosci);
                wiadomosc = pobierzDane(rozmiarPokoju,buffor,pom);
                
               // printf("\nTresc wiadomosci to:%s.\n",wiadomosc);
                  
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
                char * gotowaWiadomosc = malloc (sizeof (char) * (1+1+rozmiarNickuNadawcy+1+rozmiarPokoju+1+rozmiarWiadomosci+1+1));
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
                gotowaWiadomosc[skip]='\0';
                skip+=1;
                     
              //  printf("\nWiadomosc:%s.\n",gotowaWiadomosc);
               // printf("\nLAMBADA\n");
                // uzytkownicy z pokoju
                for(i=0;i<20;i++){
                    if(strcmp(tabUzytkownikowwPokoju[i].nick,nickNadawcy)!=0){
                        for(j=0;j<20;j++){
                            if(strncmp(tabUzytkownikow[j].nick,tabUzytkownikowwPokoju[i].nick,15)==0 && tabUzytkownikow[j].nick[0]!=0){
                                printf("\n .%d. .%s. \n",tabUzytkownikow[j].nfd,tabUzytkownikow[j].nick);
                                write(tabUzytkownikow[j].nfd,gotowaWiadomosc,skip);
                                //printf("CFD GLOWNEGO:%d");
                                //write(c->cfd,"6\t2\n",4);
                                //printf("\nWyslalem wiadomosc do:%s.",tabUzytkownikow[j].nick);
                            }
                        }
                    }
                    else{
                        write(c->cfd,"6\t1\n",4);
                    }
                }
                //wysylanie potwierdzenia utworzeniatabUzytkownikow
                //char potwierdzenie[2]="6\n";
                //write(c->cfd,potwierdzenie,2);
                
               // printf("\nZakonczylem case 6");
                break;
            }
        
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                                     W Y L O G O W A N I E
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 7:{
                int rozmiarNicka=zamienNaLiczbe(1,buffor);
                
                char * nick = malloc (sizeof (char) * rozmiarNicka);
                nick = pobierzDane(rozmiarNicka,buffor,4);
                printf("\nnick do usuniecia to:%s.",nick);
                
                for(i=0;i<20;i++){
                    if(strncmp(tabUzytkownikow[i].nick,nick,15)==0 && tabUzytkownikow[i].nick[0]!=0){
                        tabUzytkownikow[i].nfd=0;
                        for(j=0;j<15;j++){
                            tabUzytkownikow[i].nick[j]='\0';
                        }
                        
                    }
                }
               close(c->cfd);
               goto EndWhile;
               printf("\nWYLOGOWALEM");
               break;
            }
            
            //------------------------------------------------------------------------------------------------------------------------------------------------
            //                                                       E D Y C J A    G R U P Y
            //------------------------------------------------------------------------------------------------------------------------------------------------
            case 8:{
                int rozmiarNazwy=zamienNaLiczbe(1,buffor);
                char * nazwa = malloc (sizeof (char) * rozmiarNazwy);
                nazwa = pobierzDane(rozmiarNazwy,buffor,4);
                printf("\nNazwa pokoju do usuniecia to:%s.\n",nazwa);
                
                
                //odczyt nicku admina
                int polozenieRozmiaru=4+rozmiarNazwy;
                int rozmiarUzytkownika=zamienNaLiczbe(polozenieRozmiaru,buffor);
                
             
                int skadOdczytywac=polozenieRozmiaru+3;
                char * nickUzytkownika = malloc (sizeof (char) * rozmiarUzytkownika);
                nickUzytkownika = pobierzDane(rozmiarUzytkownika,buffor,skadOdczytywac);
                printf("\nNick uzytkownika:%s.",nickUzytkownika);
                
            }
            
            default : {
                printf("\n\nNie znaleziono polcenia\n");
                goto EndWhile;
                break;
            }
        }
    }
EndWhile: ;

    //sleep(3);
   // write(c->cfd, odp, strlen(odp));
    close(c->cfd);
   // printf("Wyslano do: %s message: %s\n", inet_ntoa((struct in_addr)c->caddr.sin_addr), odp);
    free(c);
    return 0;
}



int main()
{
    
    tabUzytkownikow = malloc(20*sizeof(uzytkownik));
    
    pthread_t tid;
    
    uint16_t port = 1237;
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