
int init_connection(int difPrt);
int init_session(const char *name, const char *password,int difPrt);
void save_user(char* name, char* password, int sock);
void endStoreUser(int sock);
char* wait_users(int sock);
void end_wait_users(int sock);
void send_selected_users(int sock, char id1, char id2);
void send_matrix(int sock,char* buffer, int idPlayer);
void send_Pos(int sock, char* position, int idPlayer);
char* wait_for_matrix(int sock);
int wait_for_turn(int sock);