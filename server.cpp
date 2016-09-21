#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

map<char *,char *> tracker;
struct sockaddr_in stSockAddr;
int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
ifstream * is;
char * buffer;



bool readFileIntoMemory (string fileName) {
    is = new ifstream (fileName, ifstream::binary);
    if (*is) {
        // get length of file:
        is->seekg (0, is->end);
        int length = is->tellg();
        is->seekg (0, is->beg);

        buffer = new char [length];

        std::cout << "Reading " << length << " characters... ";
        // read data as a block:
        is->read (buffer,length);
        std::cout << "all characters read successfully.";
        return true;
    }
    else{
        std::cout << "error: only " << is->gcount() << " could be read";
        return false;
    }
    // ...buffer contains the entire file...
}

void splitBuffer (){
    int bufferSize = sizeof(buffer);
    int n = bufferSize/tracker.size();
    int pos =0;
    for (int i = 0; i<tracker.size(); ++i){
        //send to each peer
        if (pos <= bufferSize){
            pos+= n;
        }
    }
}


bool newRegister(char * IP, char * PORT){
    map<char *, char *>::iterator it = tracker.find(IP);
    if(it != tracker.end()){
        cout<<"The IP is already registed"<<endl;
        return false;
    }
    tracker.insert( pair<char *,char *>(IP,PORT) );
    return true;

}

map<char *,char *> showRegister(){
    for (map<char*,char*>::iterator it=tracker.begin(); it!=tracker.end(); ++it)
        cout << *(it->first) << " => " << *(it->second) << '\n';
    return tracker;
}

bool remove(char * IP ){
    map<char *, char *>::iterator it = tracker.find(IP);
    if(it == tracker.end()){
        return false;
    }
    tracker.erase(it);
    return true;
}


void interpretador(int ConnectFD,char tipo, int msgSize){
    int n;
    char* buffer;
    //Register
    if(tipo == 'R'){
        n = read(ConnectFD,buffer,15); // IP
        buffer[15] = '\0';
        char IP[15];
        strcpy(IP,buffer);

        n = read(ConnectFD,buffer,msgSize); // PORT

        newRegister(IP,buffer);
        cout << "hola" << endl;

    }
    //Logout
    else if(tipo == 'L'){
        n = read(ConnectFD,buffer,15); // IP
        buffer[15] = '\0';
        char IP[15];
        strcpy(IP,buffer);
        remove(IP);
    }

    else if(tipo == 'O'){
        cout << "OK!" << endl;

    }
    //Read all clients
    else if(tipo == 'P'){
        showRegister();
    }

    else if(tipo == 'E'){
        cout << "ERROR" << endl;
    }

    else{
        cout << "me voy .... !!!!: " << tipo << endl;
        return;
    }
}

int main(int argc, char const *argv[]) {
    char buffer[256];
    int n;

    cout<<readFileIntoMemory("prueba.txt");
    splitBuffer();

    /*if(-1 == SocketFD)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("error bind failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    if(-1 == listen(SocketFD, 10))
    {
      perror("error listen failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    for(;;){
        int ConnectFD = accept(SocketFD, NULL, NULL);
        if(0 > ConnectFD){
            perror("error accept failed");
            close(SocketFD);
            exit(EXIT_FAILURE);
        }

        n = read(ConnectFD,buffer,4); // SIZE
        buffer[4] = '\0';
        int msgSize = atoi(buffer);

        n = read(ConnectFD,buffer,1); // TYPE
        char msgType = buffer[0];

        interpretador(ConnectFD,msgType,msgSize);
        
        shutdown(ConnectFD, SHUT_RDWR);
        close(ConnectFD);
    }

    close(SocketFD);
*/
    return 0;
}
