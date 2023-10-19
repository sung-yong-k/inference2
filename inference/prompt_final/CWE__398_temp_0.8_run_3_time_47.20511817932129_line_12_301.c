#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize;
    
    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error in creating server socket");
        exit(EXIT_FAILURE);
    }
    
    // Set server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    // Bind server socket to the server address
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error in binding server socket");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0) {
        perror("Error in listening");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port 8080...\n");
    
    while(1) {
        addrSize = sizeof(clientAddr);
        
        // Accept connection from client
        newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
        if (newSocket < 0) {
            perror("Error in accepting connection");
            exit(EXIT_FAILURE);
        }
        
        printf("New client connected\n");
        
        // Process client request
        
        // Close the connection
        close(newSocket);
    }
    
    // Close the server socket
    close(serverSocket);
    
    return 0;
}