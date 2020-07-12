#include <stdio.h>
#include "comsrv.h"
#include "game.h"
#include <unistd.h>

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


  main(0);
}