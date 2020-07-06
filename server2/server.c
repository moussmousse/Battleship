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

typedef struct User{
  char constr[32];
  SOCKET soc;
} User;



int init_connection(void){
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
      perror("bind");
      exit(errno);
   }
  // Ecoute avec une limite à 5 clients
   if(listen(sock, 5) == SOCKET_ERROR)
   {
      perror("listen");
      exit(errno);
   }

   int is_present = 0;
  while (is_present == 0){  //temps qu'il n'y a pas d'admin identifié
        
      SOCKADDR_IN csin = { 0 };
      socklen_t sinsize = sizeof (csin);

      if ((csock = accept(sock,  (SOCKADDR *)&csin, &sinsize)) == -1 ){
        perror("accept");
        continue;
      }
  
      printf("server acccept the client...\n");

      char buffer[32];
      memset(buffer,0,32);

      int n = read(csock, buffer, sizeof(buffer));
      if (n > 0){
        printf ("recieved from the client : %s\n",buffer);
        if (strcmp(buffer,"admin;adminpwd") == 0 ) 
        {
          printf("Creds matching \n");
          send(csock, "OK", 3 , 0); 
          is_present =1;
          close(sock);
          return csock;
        }
        else
        {
           send(csock, "KO", 3 , 0);
        }
      }
      
  }
  exit(2);
}

User inituser(char* constr ){
  User user;// = malloc(sizeof(User));
  memset(user.constr,0,32);
  memcpy(user.constr,constr,32);
  user.soc = -1;
  return user;
}

int Store_users(SOCKET csock, User users[]){
  int n = 0;
  while (n<10){
     printf("Listen for new user to save\n");
      char buffer[32];
      memset(buffer,0,32);
      int k =  read(csock, buffer, sizeof(buffer));
      if (strcmp(buffer,"end") == 0 ) 
        {
          printf("End of users add\n");
          return n;
        }
      
      //users[n] = malloc(sizeof(User));
      users[n] = inituser(buffer);
      //memcpy(users[n].constr, buffer,sizeof(buffer));
      //users[n].constr = buffer;*
      
      printf("Buffer : \n");
  for (int i = 0; i < 32; ++i) {
   printf("%d ", (int)buffer[i]); 
   } 
   printf("\n");
      printf("What's stored in my struct : \n");
  for (int i = 0; i < 32; ++i) {
   printf("%d ", (int)users[n].constr[i]); 
   } 
   printf("\n");


      printf("Stored : %s \n",users[n].constr);
      n++;
  }
  return n;
}

void Display_users(User users[], int num){
   printf ("\n*************\nUser number %d: \n",num);
  for(int i = 0; i<num;i++){
    printf("User : %s\nSocket : %d",users[i].constr,users[i].soc);
  }
}

int check_user_creds(int nUsers, char* creds, User users[]){
  
  printf("\n\nUsers :\n");


  for(int i = 0 ; i < nUsers ; i++){
    Display_users(users, i);
    printf("\n----\n");
    if (strcmp(creds,users[i].constr) == 0 && users[i].soc == -1){
      return i;
    }
  }
  return -1;
}

void Users_connexion(int n_users, User users[], SOCKET adm_sock){
   //Creation du socket
   printf("Awaiting client connexion...\n");
   SOCKET sock_listen = socket(AF_INET, SOCK_STREAM, 0);
   SOCKET csocka;
   SOCKADDR_IN sin = { 0 };

   if(sock_listen == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }
   // Creation de l'interface pour écouter

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT+1);
   sin.sin_family = AF_INET;

   if(bind(sock_listen,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind");
      exit(errno);
   }
  // Ecoute avec une limite à 10 clients
   if(listen(sock_listen, 10) == SOCKET_ERROR)
   {
      perror("listen");
      exit(errno);
   }
  printf("Listening...\n");
  int en_adm_queue = 0;
  while (en_adm_queue == 0){   //while the admin doesn't close user queue
       SOCKADDR_IN csin = { 0 };
      socklen_t sinsize = sizeof (csin);

      if ((csocka = accept(sock_listen,  (SOCKADDR *)&csin, &sinsize)) == -1 ){
        perror("accept");
        continue;
      }
  
      printf("Incomming client...\n");

      char buffer[32];
      memset(buffer,0,32);

      int n = read(csocka, buffer, sizeof(buffer));
      if (n > 0){
        printf ("recieved from the client : %s\n",buffer);
        int user_ind = check_user_creds(n_users,buffer,users) ;
        if ( user_ind > 0 ) 
        {
          printf("Creds matching :) \n");
          send(csocka, "OK", 3 , 0); 
          users[user_ind].soc = csocka;
          
        }
        else
        {
          printf("Creds missmatch :( \n");
           send(csocka, "KO", 3 , 0);
        }
      }




  }

}

int main(){

  SOCKET sock_admin = init_connection(); // Créer le socket d'écoute
  printf("Admin authenticed.\n");

  User users[10];

  int num_users = Store_users(sock_admin,users);
  Display_users(users, num_users);
  Users_connexion(num_users, users, sock_admin);



  close(sock_admin);


  exit(0);
}
