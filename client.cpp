#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080

using namespace std;

int main(){
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    //Create socket
    if((sock = socket(AF_INET , SOCK_STREAM  ,  0))<0){
        cerr<<"socket creation error\n";
        return 1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET , "127.0.0.1",&serv_addr.sin_addr)<=0){
        cerr<<"Invalid address\n";
        return 1; 
    }

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
        cerr<<"Connection failed\n";
        return 1;
    }

    
    
    close(sock);
    return 0;
}