#include <stdio.h>

#include "comserv.h"

int main() {
  printf("Battleship server\n");

  int test = init_connection();
  return 0;
}
