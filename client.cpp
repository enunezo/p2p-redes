#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>

#include <cstdlib> 
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[]) {
    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;

    if (-1 == SocketFD)
    {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    Res = inet_pton(AF_INET, "192.168.0.111", &stSockAddr.sin_addr);

    if (0 > Res)
    {
      perror("error: first parameter is not a valid address family");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    else if (0 == Res)
    {
      perror("char string (second parameter does not contain valid ipaddress");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("connect failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    // 0000000004 R 444444444444444 6001
    char payLoad[1000];
    char msg[1000];
    strcpy(msg,"6001");
    int msgSize = strlen(msg);
    char msgSizeSTR[11];
    sprintf(msgSizeSTR,"%04d",msgSize); // 0004
    strcat(msgSizeSTR,"R"); // += 'R';
    printf("%s\n", msgSizeSTR );
    // 0004 R 444 6001

    char ip[32];
    strcpy(ip,"1234");

    sprintf(payLoad,"%s%4s%s",msgSizeSTR,ip,msg);
    n = write(SocketFD,payLoad,4+1+4+msgSize);
    if(n < 4+1+4+msgSize){
        printf("ERROR!! ANIMAL!!\n");
    }else{
        printf("%s\n", payLoad);
        printf("OK lo hiciste bien por primera vez\n");

    }


    return 0;
}
