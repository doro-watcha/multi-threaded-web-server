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

  /* First call to socket() function , TCP */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  printf("sockfd = %d\n", sockfd);

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
  bzero( (char * ) &serv_addr, sizeof(serv_addr) );

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  /* TODO : Now bind the host address using bind() call.*/
  if ( bind(sockfd, ( struct sockaddr *)&serv_addr, sizeof(serv_addr) ) == -1) {
    perror("bind error");
    exit(1);
  }

  /* TODO : Listen on socket you created */
  if ( listen(sockfd, 10) == -1 ){
    perror("listen error");
    exit(1);
  }

  printf("Server is running on port %d\n", portno);
  
  int client_count = 0;
  while (1) {

    newsockfd[client_count] = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);



    if ( newsockfd[client_count] == -1 ){
      perror("accept error");
      exit(1);
    

    }

        respond(newsockfd[client_count]);

    printf("%d\n", client_count);

    client_count++;
  }
  printf("return");

  return 0;
}

/* TODO : Getting request and respond to clinet */
void respond(int sock) {

  int offset, bytes;

  char buffer[9000];
  bzero( buffer, 9000);


  offset = 0;
  bytes = 0;

  printf("respond\n");


  char* message;

  int size = 0;
  int count = 0;

  FILE *fp = fopen("index.html", "r");

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);

  message = malloc(size+1);
  memset(message, 0 , size + 1); 

  fseek(fp,0,SEEK_SET);

  count = fread(message,size,1,fp);

  // printf("%s size: %d, count: %d\n", message, size, count);

  // for ( int i = 0 ; i < 3000; i++){
  //   printf("%c",message[i]);
  // }

   fclose(fp);

  int length = strlen(message);

  do {

    bytes = recv ( sock , buffer + offset , 5000, 0);

    printf("receive : %d \n" , bytes);
    offset += bytes;

    if ( strncmp(buffer + offset -4, "\r\n\r\n", 4 ) == 0) break;


  } while ( bytes > 0 );


  while ( length > 0 ) {

    printf(" %d",length);


    bytes = send ( sock, message, length, 0);

    printf(" send bytes : %d \n" , bytes);

    if ( bytes == -1 ) break;
    length = length - bytes;
  }





  // char message[] = "HTTP/1.1 200 OK\r\nContentType: text/html;\r\n\r\n<html><body>Hello World! </body></html>\r\n\r\n";


  printf("close\n");
  shutdown(sock,SHUT_RDWR);
  close(sock);
 



}
