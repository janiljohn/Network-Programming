/*
 * server.c  – Parallel & Distributed Computing, HW‑2
 *
 * Listens on TCP port 9090 for one connection, receives a short
 * message from the client, prints it, then exits.
 *
 * Compile (inside the Server VM):
 *     gcc -Wall -Wextra -o server server.c
 *
 * Usage:
 *     ./server
 *
 * Capture hint: start Wireshark *after* you launch ./server but *before*
 * you run the client, and apply the capture filter
 *     ip src <client‑IP> and ip dst <server‑IP>
 */

 #include <arpa/inet.h>   // htons, htonl, inet_ntoa
 #include <netinet/in.h>  // struct sockaddr_in
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/socket.h>
 #include <unistd.h>
 
 #define PORT      9090      /* well‑known port for the assignment        */
 #define BACKLOG   1         /* single client is enough for this exercise */
 #define BUFSIZE   128
 
 int main(void)
 {
     int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
     if (listen_fd < 0) { perror("socket"); exit(EXIT_FAILURE); }
 
     /* Allow quick restart without “Address already in use” */
     int opt = 1;
     setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
 
     struct sockaddr_in srv = {0};
     srv.sin_family      = AF_INET;
     srv.sin_addr.s_addr = htonl(INADDR_ANY);   /* bind to all interfaces */
     srv.sin_port        = htons(PORT);
 
     if (bind(listen_fd, (struct sockaddr *)&srv, sizeof(srv)) < 0) {
         perror("bind"); close(listen_fd); exit(EXIT_FAILURE);
     }
 
     if (listen(listen_fd, BACKLOG) < 0) {
         perror("listen"); close(listen_fd); exit(EXIT_FAILURE);
     }
 
     printf("Server ready on port %d … waiting for a client.\n", PORT);
 
     struct sockaddr_in cli;
     socklen_t cli_len = sizeof(cli);
     int conn_fd = accept(listen_fd, (struct sockaddr *)&cli, &cli_len);
     if (conn_fd < 0) { perror("accept"); close(listen_fd); exit(EXIT_FAILURE); }
 
     char buf[BUFSIZE] = {0};
     ssize_t n = recv(conn_fd, buf, sizeof(buf) - 1, 0);
     if (n < 0) { perror("recv"); }
     else       { printf("Received %zd bytes: \"%s\"\n", n, buf); }
 
     close(conn_fd);
     close(listen_fd);
     return 0;
 }
 