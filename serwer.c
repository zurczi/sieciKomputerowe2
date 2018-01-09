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

typedef struct uzytkownik{
    char nick[10];
    
}uzytkownik;

typedef struct pokoj{
    char nazwa[10];
    
}pokoj;

char wyborCHAR[2];
char odp[100];
char myNick[10];
int wybor;

struct cln {
    int cfd;
    struct sockaddr_in caddr;
};



void* cthread (void* arg) {
    
    uzytkownik *tabUzytkownikow = malloc(20*sizeof(uzytkownik));
    pokoj *tabPokoi = malloc(20*sizeof(pokoj));
   // 
    uzytkownik *tabListaOsobDoDodania = malloc(20*sizeof(uzytkownik));
    uzytkownik *tabListaOsobwPokoju = malloc(20*sizeof(uzytkownik));

    struct cln* c = (struct cln*)arg;
    
    int pom = read(c->cfd, &wyborCHAR, sizeof(wyborCHAR));
    printf("\nPolaczylem sie z:%s." , inet_ntoa((struct in_addr)c->caddr.sin_addr));
    printf("\nOdebralem wybor:%s.",wyborCHAR);
        
    wybor=wyborCHAR[0]-'0';
        
    char odebralemWybor[5]="11\n";
    write(c->cfd,odebralemWybor,3);
    
    //deklaracja zmiennych
    FILE *plik;
    int i,j,k,x;
    int iloscCudzyslowiow;
    int pom2, plik2;
    sleep(1);
    
    switch(wybor){
        case 1:
            printf("\ncase 1"); //logowanie
            plik=fopen("uzytkownicy.txt","rw");
            plik2=0;
            plik2=open("uzytkownicy.txt",O_WRONLY|O_APPEND);
            
            //zczytywanie z pliku do tablicy
            for(i=0;i<20;i++){
                fgets(tabUzytkownikow[i].nick,10,plik);                
            }

            printf("\nPrzed odbiorem");
            
            //odbieranie danych od klienta
            sleep(1);
            pom2=read(c->cfd,myNick,sizeof(myNick));
            printf("\nOdebralem NICK:%s",myNick);
            
            
            //wyodrebnianie ladnej nazwy
            char myNick2[12];
            iloscCudzyslowiow=0;
            j=0;
            
            for(i=2;i<12;i++){
                if(myNick[i]==34){
                    iloscCudzyslowiow++;
                    if(iloscCudzyslowiow==2){ 
                        myNick2[j]='\0';
                        break;
                    }
                        continue;
                }
                myNick2[j]=myNick[i];
                j++;
            }
            
            int ileZnakowMaNick=j;
            printf("\nNICK po usunieciu enterow:%s.",myNick2);
            
            int ile=0;;
            for(i=0;i<20;i++){
                if(strncmp(tabUzytkownikow[i].nick,myNick2,10)==0){
                    ile++;
                }
            }
            
            printf("\nIle=%d.",ile);

            char zajetaNazwa[4]="100\n";
            char UdaloSieZalogowac[4]="101\n";
            
            //wysylanie ststusu zalogowania(czy sie udalo czy jest juz taki uzytkownik)
            if(ile>0){
                write(c->cfd,zajetaNazwa,4);
                printf("\nNazwa zajeta\n");
            }
            else{
                //wpisanie nowego loginu do pliku
                printf("\nZapisuje do pliku\n");
                fflush(plik);
                for(i=0;i<ileZnakowMaNick;i++){
                    if(myNick[i]>0)
                        write(plik2,&myNick2[i],1);
                }
                write(plik2,"\n",1);
                write(c->cfd,UdaloSieZalogowac,4);
            }
            
            //pomocnicze wyswielanie tablicy tabUzytkownikow
            for(i=0;i<20;i++){
                printf("X:%s.",tabUzytkownikow[i].nick);
            }
            
            
            fclose(plik);
            close(plik2);
            printf("\nSkonczylem case 1");
            break;
          
            
            
        case 2:
            printf("\ncase 2"); //wywylanie listy uzytkownikow
            plik=fopen("uzytkownicy.txt","rw");
            
            for(i=0;i<20;i++){
                fgets(tabUzytkownikow[i].nick,10,plik);
            }
            
            //usuwanie enterow
            for(i=0;i<20;i++){
                for(j=0;j<10;j++){
                    if((tabUzytkownikow[i].nick[j]<97 || tabUzytkownikow[i].nick[j]>122) &&(tabUzytkownikow[i].nick[j]>57 ||tabUzytkownikow[i].nick[j]<48)){
                        tabUzytkownikow[i].nick[j]=0;
                    }
                }
            }
            
            
            //tworzenie tablicy nazw uzytkownikow do wyslania do klienta; nazwy uzyt. oddzielone sa tabulatorami
            char nazwyUzytkownikow[300];
            x=0;
            for(i=0;i<20;i++){
                for(j=0;j<=10;j++){
                    if(j==10 || tabUzytkownikow[i].nick[j]==0){
                        nazwyUzytkownikow[x]='\t';
                        x++;
                        break;
                    }
                    if(tabUzytkownikow[i].nick[j]!=0){
                        nazwyUzytkownikow[x]=tabUzytkownikow[i].nick[j];
                        x++;
                    }
                }
            }
            

            printf("\nNazwy uzytkownikow:%s.",nazwyUzytkownikow);
            
            nazwyUzytkownikow[x-1]='\n';
            write(c->cfd,&nazwyUzytkownikow,x);
            
            fclose(plik);
           // close(plik2);
            printf("\nSkonczylem case 2");
            break;
            
        
        case 3:
            printf("\ncase 3"); //wywylanie listy uzytkownikow
            plik=fopen("pokoje.txt","rw");
            
            for(i=0;i<20;i++){
                fgets(tabPokoi[i].nazwa,10,plik);
            }
            
            //usuwanie enterow
            for(i=0;i<20;i++){
                for(j=0;j<10;j++){
                    if((tabPokoi[i].nazwa[j]<97 || tabPokoi[i].nazwa[j]>122) &&(tabPokoi[i].nazwa[j]>57 ||tabPokoi[i].nazwa[j]<48)){
                        tabPokoi[i].nazwa[j]=0;
                    }
                }
            }
            
            
            //tworzenie tablicy nazw uzytkownikow do wyslania do klienta; nazwy uzyt. oddzielone sa tabulatorami
            char nazwyPokoi[300];
            x=0;
            for(i=0;i<20;i++){
                for(j=0;j<=10;j++){
                    if(j==10 || tabPokoi[i].nazwa[j]==0){
                        nazwyPokoi[x]='\t';
                        x++;
                        break;
                    }
                    if(tabPokoi[i].nazwa[j]!=0){
                        nazwyPokoi[x]=tabPokoi[i].nazwa[j];
                        x++;
                    }
                }
            }
            

            printf("\nNazwy pokoi:%s.",nazwyPokoi);
            
            nazwyPokoi[x-1]='\n';
            write(c->cfd,&nazwyPokoi,x);
            
            fclose(plik);
           // close(plik2);
            printf("\nSkonczylem case 3");
            break;
            
            
        case 4:
           // uzytkownik *tabListaOsobwPokoju = malloc(20*sizeof(uzytkownik));
            printf("\ncase 4"); //wysylanie listy uzytkownikow w danym pokoju
            
            char nazwaPokoju[16];
            
            sleep(1);
            
            pom2=read(c->cfd,nazwaPokoju,sizeof(nazwaPokoju));
            
            char nazwaPokoju2[14];
            
            iloscCudzyslowiow=0;
            j=0;
            
             for(i=2;i<12;i++){
                if(nazwaPokoju[i]==34){
                    iloscCudzyslowiow++;
                    if(iloscCudzyslowiow==2){ 
                        nazwaPokoju2[j]='\0';
                        break;
                    }
                        continue;
                }
                nazwaPokoju2[j]=nazwaPokoju[i];
                j++;
            }
            
            printf("\nSciezka po:%s.",nazwaPokoju2);
            
            plik=fopen(nazwaPokoju2,"rw");
            
            printf("\nOtworzylam plik:%s.",nazwaPokoju2);
            
            for(i=0;i<20;i++){printf("\nTablica osob w pokoju (element %d:%s.",i,tabListaOsobwPokoju[i].nick);}
            for(i=0;i<20;i++){
                fgets(tabListaOsobwPokoju[i].nick,10,plik);
            }
            //usuwanie enterow
            for(i=0;i<20;i++){
                for(j=0;j<10;j++){
                    if((tabListaOsobwPokoju[i].nick[j]<97 || tabListaOsobwPokoju[i].nick[j]>122) &&(tabListaOsobwPokoju[i].nick[j]>57 ||tabListaOsobwPokoju[i].nick[j]<48)){
                        tabListaOsobwPokoju[i].nick[j]=0;
                    }
                }
            }
            //for(i=0;i<20;i++){printf("\nTablica osob w pokoju (element %d:%s.",i,tabListaOsobwPokoju[i].nick);}
            //tworzenie tablicy nazw uzytkownikow do wyslania do klienta; nazwy uzyt ossdielone sa tabulatorem
            char listaUzytkownikow[300];
            x=0;
            for(i=0;i<20;i++){
                for(j=0;j<=10;j++){
                    if(j==10 || tabListaOsobwPokoju[i].nick[j]==0){
                        listaUzytkownikow[x]='\t';
                        x++;
                        break;
                    }
                    if(tabListaOsobwPokoju[i].nick[j]!=0){
                        listaUzytkownikow[x]=tabListaOsobwPokoju[i].nick[j];
                        x++;
                    }
                }
            }
            listaUzytkownikow[x-1]='\n';
            
            write(c->cfd,&listaUzytkownikow,x);
            
            //pomocnicze wyswietlanie tablicy nazw uzytkownikow
            printf("\nTablica nazw uzytkownikow:%s.", listaUzytkownikow);
            
            fclose(plik);
            printf("\nSkonczylem case 4");
            break;
            
            
        case 5: //dodawanie grupy
            printf("\ncase 5");
            
            char nazwaPokojuDoDodania[16];
            
            sleep(1);
            pom2=read(c->cfd,nazwaPokojuDoDodania,sizeof(nazwaPokojuDoDodania));
            printf("\nOdebralem nazwe pokoju:%s.",nazwaPokojuDoDodania);
            
            char nazwaPokojuDoDodania2[16];
            
            //wysylanie potwierdzenia odebrania nazwy pokoju
            char odbralemNazwePok[3]="13\n";
            write(c->cfd,odbralemNazwePok,3);
            
            iloscCudzyslowiow=0;
            j=0;
            
             for(i=2;i<12;i++){
                if(nazwaPokojuDoDodania[i]==34){
                    iloscCudzyslowiow++;
                    if(iloscCudzyslowiow==2){ 
                        nazwaPokojuDoDodania2[j]='\0';
                        break;
                    }
                        continue;
                }
                nazwaPokojuDoDodania2[j]=nazwaPokojuDoDodania[i];
                j++;
            }
            
            printf("\nSciezka po:%s.\n",nazwaPokojuDoDodania2);
            
            plik=fopen(nazwaPokojuDoDodania2,"aw+");
            printf("\nDodalem plik z grupa");
            plik2=0;
            plik2=open(nazwaPokojuDoDodania2,O_WRONLY|O_APPEND);
            
            //odczytanie nicka admina pokoju
            char nickAdmina[12];
            pom2=read(c->cfd,nickAdmina,sizeof(nickAdmina));
            printf("\nOdebralem nick admina pokoju:%s.",nickAdmina);
            
            char nickAdmina2[12];
            
            for(i=0;i<12;i++){
                if(nickAdmina[i]!='\n') {
                    nickAdmina2[i]=nickAdmina[i];
                }
                else{
                    nickAdmina2[i]=0;
                    break;
                }
            }
            
            printf("\nNick admina po zmianie:%s.",nickAdmina2);
            
            //zapis nicka admina do pliku
            for(i=0;i<10;i++){
                if(nickAdmina2[i]>0)
                    write(plik2,&nickAdmina2[i],1); 
                if(nickAdmina2[i]<=0) break;
            }
                   
            write(plik2,"\n",1);
                
            char udaloSieZapisacNick[3]="14\n";
            write(c->cfd,udaloSieZapisacNick,3);
            
            //odbieranie listy uzytkownikow w pokoju
            char listaUzytkownikowPokoju[150];
            pom2=read(c->cfd,listaUzytkownikowPokoju,sizeof(listaUzytkownikowPokoju));
            printf("\nOdebralem liste uzytkownikow pokoju:%s.",listaUzytkownikowPokoju);
                
            for(i=0;i<150;i++){
                if(listaUzytkownikowPokoju[i]>0){
                    if(listaUzytkownikowPokoju[i]==9){ 
                        write(plik2,"\n",1);
                    }
                    else {
                        write(plik2,&listaUzytkownikowPokoju[i],1);
                    }  
                }
                else break;
            }
                
            char udaloSieUtworzycPokoj[3]="15\n";
            write(c->cfd,udaloSieUtworzycPokoj,3);
            
            fclose(plik);
            close(plik2);
            
            
            //dodawanie nowoutworzonego pokoju do pliku z pokojami
            plik2=open("pokoje.txt",O_WRONLY|O_APPEND);
            char nowyPokoj[12];
            for(i=0;i<16;i++){
                if(nazwaPokojuDoDodania2[i]!=46)
                    nowyPokoj[i]=nazwaPokojuDoDodania2[i];
                else
                    break;
            }
            
            write(plik2,&nowyPokoj,i);

            close(plik2);           
            
            printf("\nSkonczylem case 5");
            break;
            
            
    }
        
        sleep(1);
        close(c->cfd);
        free(c);
        return 0;
}
int main()
{
    pthread_t tid;
    
    uint16_t port = 1234;
    char ip[30] = "192.168.0.15";//INADDR_ANY;

    addr.sin_family = PF_INET;
    addr.sin_port = htons(port);
    
    printf("\nPolaczylem");
    addr.sin_addr.s_addr = INADDR_ANY;
    
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    int on = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    
    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(fd, 5);         //fd zawsze ten z funkcji socket, liczba oczekujacych polaczen
    
    while(1)
    {
        struct cln* c = malloc(sizeof(struct cln));
        rozmiar = sizeof(c->caddr);
        c->cfd = accept(fd, (struct sockaddr*)&c->caddr, &rozmiar); // blokujaca, przekazuje IP i adres portu klienta, zwraca deskryptor nowego gniazda - sluzy tylko do komunikacji z tym klientem, gniazdo glowne jest stosowane tylko do akceptacji
        //read/write z new_fd(nwd)
        pthread_create(&tid, NULL, cthread, c);
        pthread_detach(tid);
        
    };
    
    close(fd);
    
}
