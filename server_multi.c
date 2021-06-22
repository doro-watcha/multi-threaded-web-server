#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <string.h>

#define PORT 5000

char *ROOT;
void respond (int sock);

void sendall(int sock, char* msg) {
  int length = strlen(msg);
  int bytes;
  while(length > 0) {
    /* printf("send bytes : %d\n", bytes); */
    bytes = send(sock, msg, length, 0);
    length = length - bytes;
  }
}

int main( int argc, char *argv[] ) {
  int newsockfd[50];
  int sockfd, portno = PORT;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;

  clilen = sizeof(cli_addr);
  ROOT = getenv("PWD");

  /* First call to socket() function */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(1);
  }

  // port reusable
  int tr = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
  }

  /* TODO : Initialize socket structure */

  /* TODO : Now bind the host address using bind() call.*/

  /* TODO : Listen on socket you created */


  printf("Server is running on port %d\n", portno);
  int client_count = 0;
  while (1) {
    newsockfd[client_count] = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if ( newsockfd[client_count] == -1 ){
      perror("accept error");
      exit(1);
    }

    /* TODO : Multi threading part */

    client_count++;
  }

  return 0;
}

/* TODO : Getting request and respond to clinet */
void respond(int sock) {

}
