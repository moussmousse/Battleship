//server
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
#define BUF_SIZE 1024 //full random

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

int init_connection(void)
{
  //Creation du socket
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKET csock;
   SOCKADDR_IN sin = { 0 };

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }
   // Creation de l'interface pour écouter

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }
// Ecoute avec une limite à 5 clients
   if(listen(sock, 5) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }

   int is_present = 0;
  while (is_present == 0){  //temps qu'il n'y a pas d'admin identifié
        
      SOCKADDR_IN csin = { 0 };
      socklen_t sinsize = sizeof (csin);

      
      
      if (csock = accept(sock,  (SOCKADDR *)&csin, &sinsize) == -1 ){
        perror("accept");

        continue;
      }
      
      printf("Incomming connexion...\n");

      if (csock < 0) {
          printf("server acccept failed...\n");
          exit(0);
      }
      else
          printf("server acccept the client...\n");

      char buffer[14] = {};

      read(csock, buffer, sizeof(buffer));
      printf ("recieved from the client : %s\n",buffer);
  }

}




void init_sessions(){
  SOCKET sock_admin = init_connection(); // Créer le socket d'écoute

 


  close(sock_admin);
}

int main(){

  init_sessions();


  exit(0);
}
