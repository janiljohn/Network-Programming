#include <arpa/inet.h>
 #include <netinet/in.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/socket.h>
 #include <unistd.h>
 
 #define PORT 8080
 #define MSG  "Testing from Client CS231P"
 
 // TCP client that connects to a server and sends a message
 int main(int argc, char *argv[])
 {

    // Check for correct number of arguments
     if (argc != 2) {
         fprintf(stderr, "Usage: %s <server‑IP>\n", argv[0]);
         exit(EXIT_FAILURE);
     }
 
    // Create a socket for the client
     int sock = socket(AF_INET, SOCK_STREAM, 0);
 
     // Check if the socket was created successfully
     struct sockaddr_in srv = {0};
     srv.sin_family = AF_INET;
     srv.sin_port   = htons(PORT);
 
     inet_pton(AF_INET, argv[1], &srv.sin_addr);
 
     // Connect to the server
     connect(sock, (struct sockaddr *)&srv, sizeof(srv));
 
     // Check if the connection was successful
     ssize_t n = send(sock, MSG, sizeof(MSG), 0);
     if (n < 0) { 
        perror("send"); 
    } else { 
        printf("Sent %zd bytes: \"%s\"\n", n, MSG);
    }
 
    // Close the socket
     close(sock);
     return 0;
 }
 