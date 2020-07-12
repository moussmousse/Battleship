typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

typedef struct User{
  char constr[32];
  SOCKET soc;
  int** matrix;
  int** matrix_show;
} User;

typedef struct ThArgs{
  SOCKET csocka;
  SOCKET sock_listen;
  SOCKET adm_sock;
  int n_users;
  User* users;
}ThArgs;

typedef struct Game_Users{
  User user1;
  User user2;
}Game_Users;

int init_connection(void);
User inituser(char* constr );
int Store_users(SOCKET csock, User users[]);
void Display_users(User users[], int num);
int check_user_creds(int nUsers, char* creds, User users[]);
void Send_user_to_admin(SOCKET adm_sock, char* constr);
void *listen_users(void* args);
ThArgs init_thargs(SOCKET csocka,SOCKET sock_listen, SOCKET adm_sock, int n_users, User* users);
void Users_connexion(int n_users, User users[], SOCKET adm_sock);
Game_Users select_users(User users[], SOCKET adm_sock);
Game_Users init_game(Game_Users users, SOCKET adm_sock);
void send_matrixToPlayer(User user, int** matrix);
void send_awnser_to_Player(User user,char* awnser);
Game_Users wait_for_pos(Game_Users users, int cpt);
void win(User winer, User looser);