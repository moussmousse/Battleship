#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"


void usage(){
  printf("Args : \n-u for user\n-a for admin\n ");
  exit(1);
}

int check_args(int ln, char* args[]){
  if (ln != 2){
    printf("Invalid argument number\n");
    usage();
  }
  if (strcmp(args[1],"-u") != 0 && strcmp(args[1],"-a") != 0){
    printf("Invalid argument \n");
    usage();
  }
  if (strcmp(args[1],"-u") == 0){
    return 1;
  }
  if (strcmp(args[1],"-a") == 0){
    return 0;
  }
}

int main(int argc, char *argv[]) {
  printf("Battleship client\n");
  int difPrt = check_args(argc, argv);
  menuInterface(difPrt);
  return 0;
}
