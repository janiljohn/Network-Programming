/*
 * client.c  – Parallel & Distributed Computing, HW‑2
 *
 * Connects to the server’s IP (first command‑line argument) on TCP
 * port 9090, sends the literal string "Hello Sockets", then exits.
 *
 * Compile (inside the Client VM):
 *     gcc -Wall -Wextra -o client client.c
 *
 * Usage:
 *     ./client <server‑IP>
 * Example:
 *     ./client 192.168.56.101
 */

 #include <arpa/inet.h>   // inet_pton, htons
 #include <netinet/in.h>  // struct sockaddr_in
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/socket.h>
 #include <unistd.h>
 
 #define PORT 9090
 #define MSG  "Hello Sockets"
 
 int main(int argc, char *argv[])
 {
     if (argc != 2) {
         fprintf(stderr, "Usage: %s <server‑IP>\n", argv[0]);
         exit(EXIT_FAILURE);
     }
 
     int sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock < 0) { perror("socket"); exit(EXIT_FAILURE); }
 
     struct sockaddr_in srv = {0};
     srv.sin_family = AF_INET;
     srv.sin_port   = htons(PORT);
 
     if (inet_pton(AF_INET, argv[1], &srv.sin_addr) != 1) {
         perror("inet_pton"); close(sock); exit(EXIT_FAILURE);
     }
 
     if (connect(sock, (struct sockaddr *)&srv, sizeof(srv)) < 0) {
         perror("connect"); close(sock); exit(EXIT_FAILURE);
     }
 
     ssize_t n = send(sock, MSG, sizeof(MSG), 0);
     if (n < 0) { perror("send"); }
     else       { printf("Sent %zd bytes: \"%s\"\n", n, MSG); }
 
     close(sock);
     return 0;
 }
 