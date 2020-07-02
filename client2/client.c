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


int init_connection()
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };
   struct hostent *hostinfo;

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   hostinfo = gethostbyname(ADDRESS);
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", ADDRESS);
      exit(EXIT_FAILURE);
   }

   sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
   {
      perror("connect()");
      exit(errno);
   }

   return sock;
}

void init_session(const char *name, const char *password)
{
  printf("Hi %s\n",name);
  SOCKET sock = init_connection();

  //
  size_t  fullSize = strlen( name ) + 1 +  strlen( password );

  /* Memory allocation and copy */
  char * logstring = (char *) malloc( fullSize );
  strcpy( logstring, name );
  strcat( logstring, ";" );
  strcat( logstring, password );
  printf("Conn string : %s\n",logstring);
  if(send(sock, logstring, 10, 0) < 0)
  {
    perror("send()");
    exit(errno);
  }


}


int main(){

  char user[] = "admin";
  char pwd[] = "adminpwd";
  init_session(user,pwd);


  exit (0);
}
