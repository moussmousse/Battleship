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

void login(SOCKET sock, const char *name, const char *pwd)
{

      size_t  fullSize = strlen( name ) + 1 +  strlen( pwd );

      /* Memory allocation and copy */
      char * logstring = (char *) malloc( fullSize );
      strcpy( logstring, name );
      strcat( logstring, ";" );
      strcat( logstring, pwd );
      printf("Conn string : %s\n",logstring);
   if(send(sock, logstring, 10, 0) < 0)
   {
      perror("send()");
      exit(errno);
   }

   int con = 0;
   int buffer[10];
   while(con == 0){
     if (recv(sock, buffer, 10 - 1, 0) != -1)
     {
          if (buffer[0] == 0)
          {
            printf ("Nope, t'es pas connecte.");
            con = -1;
          }
          else{
            printf ("Bien joué t'es connecte !");
            con = 1;
          }
      }

   }

}

int init_connection(const char *address)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };
   struct hostent *hostinfo;

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   hostinfo = gethostbyname(address);
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", address);
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

void game(const char *address, const char *name, const char *password)
{
  printf("address : %s \nUsername : %s \nPassword : %s \n", address, name, password);

   SOCKET sock = init_connection(address);
   login(sock, name, password);


}


int main(int argc, char **argv)
{
   if(argc < 3)
   {
      printf("Usage : %s [address] [pseudo] [password]\n", argv[0]);
      return EXIT_FAILURE;
   }


   game(argv[1], argv[2], argv[3]);


   return EXIT_SUCCESS;
}
