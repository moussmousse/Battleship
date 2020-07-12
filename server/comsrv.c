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
#include <pthread.h>
#include "game.h"
#include "comsrv.h"

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#define NBCLI 3
#define PORT 3550
#define ADDRESS "192.168.1.31"
#define BUF_SIZE 1024 //full random



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
  User user;
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
      
      users[n] = inituser(buffer);


      printf("Stored user number %d :\n %s \n",n,users[n].constr);
      n++;
  }
  return n;
}

void Display_users(User users[], int num){
  printf("*************\nList of all users : \n");
  for(int i = 0; i<num;i++){
    printf ("\nUser number %d: \n",i);
    printf("User : %s\nSocket : %d\n\n",users[i].constr,users[i].soc);
  }
  printf("*************\nEnd of list\n\n");
}

int check_user_creds(int nUsers, char* creds, User users[]){
  Display_users(users, nUsers);
  for(int i = 0 ; i < nUsers ; i++){
    printf("Match ? %s : %s  -> socket : %d\n",creds, users[i].constr, users[i].soc );
    if (strcmp(creds,users[i].constr) == 0 && users[i].soc == -1){
      return i;
    }
  }
  return -1;
}

void Send_user_to_admin(SOCKET adm_sock, char* constr){
  send(adm_sock, constr, sizeof(constr) , 0); 
}


void *listen_users(void* args){
  ThArgs* thargs = (ThArgs*)args;
  while(1){
     SOCKADDR_IN csin = { 0 };
      socklen_t sinsize = sizeof (csin);

    /*
      if ((thargs->csocka = accept(thargs->sock_listen,  (SOCKADDR *)&csin, &sinsize)) == -1 ){
        perror("accept");
        break;
      }
  */
      thargs->csocka = accept(thargs->sock_listen,  (SOCKADDR *)&csin, &sinsize);
  
      printf("Incomming client...\n");

      char buffer[32];
      memset(buffer,0,32);

      int n = read(thargs->csocka, buffer, sizeof(buffer));
      if (n > 0){
        printf ("recieved from the client : %s\n",buffer);
        int user_ind = check_user_creds(thargs->n_users,buffer,thargs->users) ;
        if ( user_ind >= 0 ) 
        {
          printf("Creds matching :) \n");
          send(thargs->csocka, "OK", 3 , 0); 
          thargs->users[user_ind].soc = thargs->csocka;
          char *awnser = malloc(32 *sizeof(char));
          memset(awnser,0,32);
          sprintf(awnser, "%d", user_ind);
          send(thargs->adm_sock,awnser,sizeof(awnser),0);
          //send(adm_sock,split_constr(buffer),sizeof(buffer),0);
        }
        else
        {
          printf("Creds missmatch :( \n");
           send(thargs->csocka, "KO", 3 , 0);
        }
      }
  }
}

ThArgs init_thargs(SOCKET csocka,SOCKET sock_listen, SOCKET adm_sock, int n_users, User* users){
  ThArgs thargs;
  thargs.csocka = csocka;
  thargs.sock_listen = sock_listen;
  thargs.adm_sock = adm_sock;
  thargs.n_users = n_users;
  thargs.users = users;
  return thargs;
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
  pthread_t thread_users;  //while the admin doesn't close user queue
  ThArgs thargs = init_thargs(csocka,sock_listen,adm_sock,n_users,users);
  if (pthread_create(&thread_users,NULL,&listen_users,&thargs) == -1){
       printf("Error while create listen thread");
      exit(3);
  }
  while (en_adm_queue == 0){
      char buffer[32];
      memset(buffer,0,32);
      int n = read(adm_sock, buffer, sizeof(buffer));
      if (n > 0){
        printf ("recieved from the admin : %s\n",buffer);
        if (strcmp(buffer,"END") == 0){
          pthread_cancel(thread_users);
          close(thargs.sock_listen);
          en_adm_queue = 1;
        }
     }
  }
}

Game_Users select_users(User users[], SOCKET adm_sock){
  char buffer[4];
      memset(buffer,0,4);
      int ind_user1 = 0;
      int ind_user2 = 0;
      char b1[2];
      char b2[2];
      memset(b1,0,2);
      memset(b2,0,2);
      int n = read(adm_sock, buffer, sizeof(buffer));
      b1[0] = buffer[0];
      b1[1] = '\0';
      b2[0] = buffer[1];
      b2[1] = '\0';
     
      printf("Buffer of select users : %s\nb1 : %s\nb2 : %s\n\n",b1,b2);
      if (n > 0){
        ind_user1 = atoi(b1);
        ind_user2 = atoi(b2);
        printf ("index of user 1 : %d\nindex of user 2 : %d\n",ind_user1,ind_user2);
      }
      else {
        perror("read");
        exit(3);
      }
  Game_Users game_users;
  game_users.user1 = users[ind_user1];
  game_users.user2 = users[ind_user2];
  //dégager les autres users # Déconnecition de grosse brute !!!
  return game_users;
}



Game_Users init_game(Game_Users users, SOCKET adm_sock){
    char buffe[38];
    memset(buffe,0,38);
    //le premier char du buffer correspond a l'id du player
    //initailise les deux matrices
    users.user1.matrix = initBattleShipMatrix(users.user1.matrix);
    users.user2.matrix = initBattleShipMatrix(users.user2.matrix);
    users.user1.matrix_show = initBattleShipMatrix(users.user1.matrix_show);
    users.user2.matrix_show = initBattleShipMatrix(users.user2.matrix_show);
    int cpt=0;
    //attends un read 2 fois de l'admin pour récupérer les deux matrice
    while(cpt<2){
      int n = read(adm_sock, buffe, sizeof(char)*38);
      if (n > 0){
      printf("Buffer after read\n");
        for (int i = 0 ; i < 38 ; i++) {printf ("%d ",buffe[i]);}printf("\n");
        //le premier char du buffer correspond a l'id du player
        if(buffe[0]=='1'){
          stringToMatrix(buffe,users.user1.matrix);
          printf ("Matrix of player 1\n");
        }
        else if(buffe[0]=='2'){
          stringToMatrix(buffe,users.user2.matrix);
          printf ("Matrix of player 2\n");
        }
        else {
          printf("ERROR Player ID not good !\n");
        }
      }
      else {
        perror("read");
        exit(3);
      }
      cpt++;
    }
    //envoie les deux matrices aux deux users
    memset(buffe,0,38);
    strcpy(buffe,matirixToBuffer(users.user1.matrix));
    printf("buffer send player1-> |%s|\n",buffe);
    send(users.user1.soc,buffe,sizeof(buffe),0);
    memset(buffe,0,38);
    strcpy(buffe,matirixToBuffer(users.user2.matrix));
    printf("buffer send player2-> |%s|\n",buffe);
    send(users.user2.soc,buffe,sizeof(buffe),0);
  return users;
}

void send_matrixToPlayer(User user, int** matrix){
  char *buffer ;
  buffer = malloc(sizeof(char)*38);
  memset(buffer,0,38);
  buffer = matirixToBuffer(matrix);
  printf("buffer send player -> |%s|\n",buffer);
  send(user.soc,buffer,sizeof(char)*38,0);
  free(buffer);

}

void send_awnser_to_Player(User user,char* awnser){
   char buffer[38];
    memset(buffer,0,38);

    memcpy(buffer,awnser,sizeof(awnser));
    send(user.soc,buffer,sizeof(char)*38,0);
   
}



Game_Users wait_for_pos(Game_Users users, int cpt){
  char buffe[4];
  memset(buffe,0,4);

  User current_user;
  User passive_user;
  if (cpt%2 == 0){
    current_user = users.user1;
    passive_user = users.user2;
  }
  else{
    current_user = users.user2;
    passive_user = users.user1;
  }
  printf("It's turn of : %s\n",current_user.constr);
  send(current_user.soc,"GO\0",sizeof("GO\0"),0);
  send(passive_user.soc,"WAIT\0",sizeof("WAIT\0"),0);

  int n = read(current_user.soc, buffe, sizeof(char)*4);
  if (n > 0){
    char pos[3];
    memset(pos,0,3);
    pos[0]=buffe[1];
    pos[1]=buffe[2];
    pos[2]='\0';
    printf("Position recieved : %s\n",pos);
    printBattle2Matrix(passive_user.matrix,passive_user.matrix_show);
    printBattle2Matrix(current_user.matrix,current_user.matrix_show);
    if(verifyMatrixbeetwen2Case(passive_user.matrix,pos,pos)==0){
      printf ("Hit ! \n");
      setCharFromBattleShipMatrix(passive_user.matrix_show,pos,2);
      setCharFromBattleShipMatrix(passive_user.matrix,pos,2);
      send_awnser_to_Player(passive_user,"HIT :(\0");
      send_awnser_to_Player(current_user,"HIT ! :D \0");
    }
    else{
      printf ("Miss ... \n");
      send_awnser_to_Player(passive_user,"MISS ! :D\0");
      send_awnser_to_Player(current_user,"MISS :(\0");
    }
    send_matrixToPlayer(current_user,passive_user.matrix_show);
    send_matrixToPlayer(passive_user,passive_user.matrix);
  }
  else {
    perror("read");
    exit(3);
  }
  return users;
}

void win(User winer, User looser){
  printf("***************\n\n");
  printf("!! %s WIN !!\n",winer.constr);
  printf("***************\n\n");

  // Send result
  send(winer.soc,"WIN\0",sizeof("WIN\0"),0);
  send(looser.soc,"LOOSE\0",sizeof("LOOSE\0"),0);

  close(winer.soc);
  close(looser.soc);
}



/*

int main(){

  SOCKET sock_admin = init_connection(); // Créer le socket d'écoute
  printf("Admin authenticed.\n");
  User users[10];
  int num_users = Store_users(sock_admin,users);
  Display_users(users, num_users);
  Users_connexion(num_users, users, sock_admin);
  Display_users(users, num_users);
  Game_Users game = select_users(users, sock_admin);
  printf("\n\nUsers selected to play : \n- %s on socket number %d\n - %s on socket number %d\n",game.user1.constr,game.user1.soc,game.user2.constr,game.user2.soc);
  game = init_game(game,sock_admin);
  int cpt = 0;

  while(1){
    if (verifyGameEnd(game.user1.matrix) == 1){
      win(game.user2,game.user1);
      break;
    }
    else if (verifyGameEnd(game.user2.matrix) == 1){
      win(game.user1,game.user2);
      break;
    }
    printf("\n\nTurn number : %d\n",cpt);
    game = wait_for_pos(game,cpt);
    cpt++;

  }
  
  close(sock_admin);


  exit(0);
}
*/