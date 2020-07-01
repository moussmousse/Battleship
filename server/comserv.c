/*    comserv.c  */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "comserv.h"


int init_connection(void)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);


   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }
   SOCKADDR_IN sin = { 0 };
   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }

   return sock;
}
