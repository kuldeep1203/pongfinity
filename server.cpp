#include<iostream>
#include<string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<unistd.h>

#define PORT 8080

using namespace std;



int main(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen  = sizeof(address);

    char buffer[1024] = {0};
    const char* hello = "Hello from server";

    //AF_INET - pecifies the type of addresses that your socket can communicate with
    if((server_fd = socket(AF_INET,SOCK_STREAM,0))==0){ //socket file descriptor
        perror("SOCKET FAILED");
        return 1 ;
    }

    //BIND socket to IP/PORT
    address.sin_family = AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd,(struct sockaddr*)&address,sizeof(address))<0){
        perror("Bind failed");
        return 1;
    }

    //listen for connections 

    if(listen(server_fd,3)<0){
        perror("Listen failed");
        return 1; 
    }

    cout<<"Server is listening on port"<< PORT << "....\n";


    if((new_socket =accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen))<0){
        perror("Aceept failed");
        return 1;
    }

    cout<<"client connected!\n";
    int valread = read(new_socket, buffer, 1024);
    std::cout << "📩 Received: " << buffer << "\n";

    const char* message = "connection closed";

    if(buffer=="CLOSE"){
        send(new_socket,message,sizeof(message),0);
        close(new_socket);
        cout<<"connection with client closed server still running...";
    }
    close(server_fd);
    
    return 0;
}