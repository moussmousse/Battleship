//client
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <errno.h>
#include <string.h>
#include "comcli.h"

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#define NBCLI 3
#define PORT 3550
#define ADDRESS "192.168.1.31"

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

//client
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <errno.h>
#include <string.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#define NBCLI 3
#define PORT 3550
#define ADDRESS "192.168.1.31"

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;


int init_connection(int prt)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };
   struct hostent *hostinfo;

   if(sock == INVALID_SOCKET)
   {
      perror("socket");
      exit(errno);
   }

   hostinfo = gethostbyname(ADDRESS);
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", ADDRESS);
      exit(EXIT_FAILURE);
   }

   sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   sin.sin_port = htons(PORT + prt) ;
   sin.sin_family = AF_INET;

   if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
   {
      perror("connect");
      exit(errno);
   }

   return sock;
}

int init_session(const char *name, const char *password, int difPrt)
{
  printf("Hi %s\n",name);
  SOCKET sock = init_connection(difPrt);

  size_t  fullSize = strlen( name ) + 1 +  strlen( password ) +1;

  /* Memory allocation and copy */
  char * logstring = (char *) malloc( fullSize );
  strcpy( logstring, name );
  strcat( logstring, ";" );
  strcat( logstring, password );
  strcat( logstring, "\0" );
  printf("Conn string : %s\n",logstring);
  if(send(sock, logstring, fullSize + 1, 0) < 0)
  {
    perror("send");
    exit(errno);
  }
  else{
     printf("creds succesfully send\n");
  }
   char buffer[8];
   memset(buffer,0,8);
   int n = read(sock, buffer, sizeof(buffer));
   if (strcmp(buffer,"OK") == 0){
      return sock;
   }
   return -1;
}

void save_user(char* name, char* password, SOCKET sock){
 size_t  fullSize = strlen( name ) + 1 +  strlen( password ) +1;

  /* Memory allocation and copy */
  char * logstring = (char *) malloc( fullSize );
  strcpy( logstring, name );
  strcat( logstring, ";" );
  strcat( logstring, password );
  strcat( logstring, "\0");
  printf("Conn string : %s\n",logstring);
  int ln = send(sock, logstring, fullSize + 1, 0);
  if( ln < 0)
  {
    perror("send");
    exit(errno);
  }
  else{
     printf("User creds succesfully send\n%d Bysts send.\n",ln);
  }
}

void endStoreUser(int sock){
    int ln = send(sock, "end", 4, 0);
  if( ln < 0)
  {
    perror("send");
    exit(errno);
  }
  else{
     printf("End of store users\n");
  }
}


/*
int main(){

  char user[] = "admin";
  char pwd[] = "adminpwd";
  printf("auth : %d\n",init_session(user,pwd));


  exit (0);
}

*/