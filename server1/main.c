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

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
}Client;

void debuglog(char *log)
{
  printf("Degug -> %s\n",log);
}

int init_connection(void)
{
  //Creation du socket
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
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

   return sock;
}

 char* check_creds(char *cred_str)
{
  printf("Cred string : .%s.\n",cred_str);
  const char *lst[10];
  lst[0] = "user0;pwd0";
  lst[1] = "user1;pwd1";
  lst[2] = "user2;pwd2";
  lst[3] = "user3;pwd3";
  lst[4] = "user4;pwd4";
  lst[5] = "user5;pwd5";
  lst[6] = "user6;pwd6";
  lst[7] = "user7;pwd7";
  lst[8] = "user8;pwd8";
  lst[9] = "user9;pwd9";
  char scred_str[10];
  char name[5] ;
  int find = 0;
  printf ("Con string (2) : %s\n",cred_str);
  for (int i = 0 ; i < 10 ; i++)
  {
    if (strcmp (cred_str, lst[i]) == 0 )
    {
      memcpy (name, lst[i],5);

      printf ("User find !");
      find = 1;
    }
  }
  if (find == 1)
  {
    printf ("User logon! \n");
    printf ("%s\n",name);
   return "OK";
  }
  printf ("User NOT logon! \n");
  return "NULL";
}

void test(char *string){
  printf (string);
}

int read_client(SOCKET sock, char *buffer)
{
   int n = 0;
   if((n = recv(sock, buffer, 10 , 0)) < 0)
   {
      perror("recv()");
      /* if recv error we disonnect the client */
      n = 0;
   }


   printf("read_client (get client name & pwd : )\n");

 const char* res_check;
  char crestr[10] ;

  memcpy(crestr, buffer, 10);
  printf ("Con string (1) : %s\n",crestr);

  res_check = check_creds(crestr);
  char nl[4] = "NULL";
  printf ("User connected? : %s\n",res_check);
  int *response = malloc(sizeof(int));

  if (strcmp (res_check, nl) == 0)
   {
     *response = 0;
      printf("Unautorized\n");
       perror("Unautorized ! ");
       n = 0;
   }
   else{
     *response = 1;
   }
   if(send(sock, response ,1, 0) < 0)
   {
      perror("send()");
      exit(errno);
   }


   buffer[n] = 0;

   return n;
}

void game(){
  SOCKET sock = init_connection();
  char buffer[BUF_SIZE];
  /* the index for the array */
  int actual = 0;
  int max = sock;
  /* an array for all clients */
  Client clients[5];

  fd_set rdfs;

  while(1)
  {
    int i = 0;
    FD_ZERO(&rdfs);

    /* add STDIN_FILENO */
    FD_SET(STDIN_FILENO, &rdfs);

    /* add the connection socket */
    FD_SET(sock, &rdfs);

    /* add socket of each client */
    for(i = 0; i < actual; i++)
    {
       FD_SET(clients[i].sock, &rdfs);
    }

    if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
    {
       perror("select()");
       exit(errno);
    }

    /* something from standard input : i.e keyboard */
    if(FD_ISSET(STDIN_FILENO, &rdfs))
    {
       /* stop process when type on keyboard */
       break;
    }
    else if(FD_ISSET(sock, &rdfs))
    {
       /* new client */
       printf("New client incomming...\n");
       SOCKADDR_IN csin = { 0 };
       size_t sinsize = sizeof csin;
       int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
       if(csock == SOCKET_ERROR)
       {
          perror("accept()");
          continue;
       }

       /* after connecting the client sends its name */
       if(read_client(csock, buffer) == -1)
       {
          /* disconnected */
          continue;
       }

       /* what is the new maximum fd ? */
       max = csock > max ? csock : max;

       FD_SET(csock, &rdfs);

       Client c = { csock };

       strncpy(c.name, buffer, BUF_SIZE - 1);
       clients[actual] = c;
       actual++;
    }


  }



}



int main ()
{
    game();
    exit (0);
}
