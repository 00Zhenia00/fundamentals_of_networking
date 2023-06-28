#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>


#define PORT 8888
#define SIZE 1024


void server(){

    int server_fd, client_socket;
    struct sockaddr_in address;
    char buffer[SIZE] = {0};

    pid_t childpid;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        std::cout << "bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0) {
        std::cout << "listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Waiting for a client..." << std::endl;

    while(true){

        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            std::cout << "accept failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        std::cout << "Client accepted.\n";

        // Creates a child process
        if ((childpid = fork()) == 0) {
 
            // Closing the server socket id
            close(server_fd);
            
            while(true){

                if (read(client_socket , buffer, SIZE) < 0) {
                    std::cout << "Error in receiving data.\n";
                }
                else {

                    if (strcmp(buffer, "hello") == 0){
                        send(client_socket, "world", strlen("world"), 0);
                    }
                    else if (strcmp(buffer, "disconnect") == 0){
                        break;
                    }
                    else{
                        send(client_socket, buffer, SIZE, 0);
                    }

                }
            }
        }

    }

    close(client_socket);
}


void client(){

    int client_fd;
    struct sockaddr_in address;
    char buffer[SIZE] = {0};

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cout << "socket failed";
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (connect(client_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        std::cout << "connection failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Successful connection!\n";

    while(true){

        std::cout << "Please enter your message:\n > ";

        std::cin.getline(buffer, sizeof(buffer));

        if (strcmp(buffer, "disconnect") == 0)
            break;

        send(client_fd, buffer, sizeof(buffer), NULL);
        
        if (read( client_fd , buffer, sizeof(buffer)) < 0){
            std::cout << "Error in receiving data.\n";
        }
        else{
           std::cout << " > " << buffer << std::endl;
        }
    }

    std::cout << "Disconnected.\n";

}


int main(int argc, char** argv) {

    if (argc < 2){
        std::cout << "specify the mode - client or server!\n";
        exit(1);
    }

    if (strcmp(argv[1], "server") == 0){
        server();
    }

    if (strcmp(argv[1], "client") == 0){
        client();
    }

    return 0;
}
