#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "client.h"
int main(){
 
  char *ip = SERVER_ADDRESS;
  int port = 5556;
 
  int sock;
  struct sockaddr_in addr;
  socklen_t addr_size;
  char buffer[1024];
  int n;
 
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");
 
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = inet_addr(ip);
 
  connect(sock, (struct sockaddr*)&addr, sizeof(addr));
  printf("Connected to the server.\n");
  while (1)  {
        
    bzero(buffer, 1024);
    char send1[50];
    printf("\n2. Nome socket \n3. Server IP \n4. Little or big endian \n5. get host name \n6. Client IP\n");
    printf("inserisci un messaggio:");
    scanf("%s", send1);
    strcpy(buffer,send1 );
    printf("Client: %s\n", buffer);
    send(sock, buffer, strlen(buffer), 0);
    
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Risposta: %s\n", buffer);
  }

 
  close(sock);
  printf("Disconnected from the server.\n");
 
  return 0;
}