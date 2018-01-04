#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>

struct sockaddr_in addr;

int main () {
char buf[100];

addr.sin_family=PF_INET;
addr.sin_port=htons(1234);
addr.sin_addr.s_addr=inet_addr("192.168.0.15");

int fd=socket(PF_INET,SOCK_STREAM,0);

char indeks2[6]="127245";
char indeks[6]="127237";

connect(fd,(struct sockaddr*)&addr,sizeof(addr));

write(fd,indeks,sizeof(indeks));

int r=read(fd,buf,sizeof(buf));

write(1,buf,r);

close(fd);


}


