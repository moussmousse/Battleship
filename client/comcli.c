//client
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
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

int guard(int n, char * err) { if (n == -1) { perror(err); exit(1); } return n; }

char* wait_users(int sock){
      char* buffer = malloc(32*sizeof(char));
      memset(buffer,0,32);
      int flags =fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK);
      int n = recv(sock, buffer, 32, flags | O_NONBLOCK);
      if (n>0){
         return buffer;
       }
      else{
         return NULL;
      }
}




void end_wait_users(int sock){

   send(sock, "END\0", sizeof("END\0") , 0);
}

void send_selected_users(int sock, char id1, char id2){
      char buffer[4];
      memset(buffer,0,4);
      buffer[0] = id1;
      buffer[1] = id2;
      buffer[2] = '\0';
      send(sock, buffer, sizeof(buffer), 0);
}

void send_matrix(int sock,char* buffer, int idPlayer){
   buffer[0]= (char) idPlayer +48;
   buffer[37]='\0';
   send(sock, buffer, sizeof(char)*38, 0);
   
}

void send_Pos(int sock, char* position, int idPlayer){
   char buffer[4];
   memset(buffer,0,4);
   buffer[0] = (char) idPlayer +48;
   buffer[1] = position[0];
   buffer[2] = position[1];
   buffer[3]= '\0';
   printf("socket ->%d, buffer ->|%s|\n",sock,buffer);
   send(sock, buffer, sizeof(char)*4, 0);
}

char* wait_for_matrix(int sock){
   char *buffer;
   buffer = malloc(sizeof(char)*38);
   memset(buffer,0,38);
   int n = read(sock, buffer, 38);
   if (n>0){
      return buffer;
   }
   else{
      return NULL;
   }

}

int wait_for_turn(int sock){
   char buffer[32];
   memset(buffer,0,32);
   int n = read(sock, buffer, 38);
   if (n>0){
      if(strcmp(buffer,"WAIT")==0){
         return 0;
      }
      else if(strcmp(buffer,"GO")==0){
         return 1;
      }
      else if(strcmp(buffer,"WIN")==0){
         return 2;
      }
      else if(strcmp(buffer,"LOOSE")==0){
         return 3;
      }
      else{
         return 0;
      }
   }
   else{
      return 0;
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
