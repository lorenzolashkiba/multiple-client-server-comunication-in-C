#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "client.h"

int main(int argc, char **argv){

    // create socket info
    char *ip = "127.0.0.1";
    int port = 5556;
    //create socket
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    int n;
    char clientIP[16];
    char buffer[1024];
    int numberOfClients = 0;
    int option = 1;
    int pid;
    
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0){
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");
    
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
     
    //Bind
    n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n < 0){
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);
    
    //listening for connections
    listen(server_sock, 5);
    printf("Listening...\n");

    addr_size = sizeof(client_addr);
    while(1){
      if ((client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size)) == -1) {
        perror("[-]Accept failed");
        continue;
      }
      numberOfClients++;
      printf("[+]Client connected.\n");
      inet_ntop(AF_INET, &client_addr.sin_addr, clientIP, 16);
      fprintf(stderr, "[+]Client #%d IP: %s connected.\n", numberOfClients, clientIP);
      if ((pid = fork()) == 0) {
        while(1){
          bzero(buffer, 1024);
          recv(client_sock, buffer, sizeof(buffer), 0);
          printf("Client: %s\n", buffer);
          if(strcmp(buffer,"2")==0){
            int d1 = getsockname(server_sock,(struct sockaddr*)&client_addr,addr_size);
            char c[50];
            snprintf(c,sizeof(c),"nome socket = %s : %d \n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
            strcpy(buffer,c);
            send(client_sock, buffer, strlen(buffer), 0);
           
            
          }else if(strcmp(buffer,"3")==0){
           
            int d1 = getsockname(server_sock,(struct sockaddr*)&server_addr,addr_size);
            char c[50];
            snprintf(c,sizeof(c),"Server IP = %s : %d \n", inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port));
            strcpy(buffer,c);
            send(client_sock, buffer, strlen(buffer), 0);
           

          }else if(strcmp(buffer,"4")==0){
            union {
              long lungo;
             char ch[sizeof(long)];
            } unione;
             unione.lungo = 1;
             if (unione.ch[sizeof(long)-1] == 1)
              strcpy(buffer,"big endian");
             else
              strcpy(buffer,"little endian");
           send(client_sock, buffer, strlen(buffer), 0);

          }else if(strcmp(buffer,"5")==0){
            char c[50];
            int d1 = gethostname(c,50);
            strcpy(buffer,c);
            send(client_sock, buffer, strlen(buffer), 0);

          }else if(strcmp(buffer,"6")==0){
            int serv_peer_err = getpeername(server_sock,(struct sockaddr *)&client_addr,addr_size);
            char c[50];
            snprintf(c,sizeof(c)," Client IP = %s : %d \n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
            strcpy(buffer,c);
            send(client_sock, buffer, strlen(buffer), 0);
          }
          bzero(buffer, 1024);
          send(client_sock, buffer, strlen(buffer), 0);
        }
       } else {
            printf("Parent Process: PID of Child Process is %d\n", pid);
          }
      close(client_sock);
      printf("[+]Client disconnected.\n\n");
      numberOfClients--;
    }
  close(server_sock);
  return 0;

}