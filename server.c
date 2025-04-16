#include <arpa/inet.h>
 #include <netinet/in.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/socket.h>
 #include <unistd.h>
 
 #define PORT      8080
 #define BACKLOG   1
 #define BUFSIZE   128
 
 // Simple TCP server that listens for incoming connections on a specified port
 int main(void)
 {
    // Create a socket for the server
     int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
     if (listen_fd < 0) {
        perror("socket"); 
        exit(EXIT_FAILURE); 
    }
 
     // Set socket options to allow reuse of the address
     int opt = 1;
     setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
 
    // Bind the socket to an address and port
     struct sockaddr_in srv = {0};
     srv.sin_family      = AF_INET;
     srv.sin_addr.s_addr = htonl(INADDR_ANY);   /* bind to all interfaces */
     srv.sin_port        = htons(PORT);
     bind(listen_fd, (struct sockaddr *)&srv, sizeof(srv));
 
     // Listen for incoming connections
     listen(listen_fd, BACKLOG);
 
     printf("Server up at %d … awating client.\n", PORT);
 
     // Accept a connection from a client
     struct sockaddr_in client;
     socklen_t client_len = sizeof(client);
     int conn_fd = accept(listen_fd, (struct sockaddr *)&client, &client_len);
     if (conn_fd < 0) { perror("accept"); close(listen_fd); exit(EXIT_FAILURE); }
 
     // Print the client's address
     char payload[BUFSIZE] = {0};
     ssize_t n = recv(conn_fd, payload, sizeof(payload) - 1, 0);
     if (n < 0) { perror("recv"); }
     else       { printf("Received %zd bytes: \"%s\"\n", n, payload); }
 
     // Close the connection
     close(conn_fd);
     close(listen_fd);
     return 0;
 }
 