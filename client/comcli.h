
int init_connection(int difPrt);
int init_session(const char *name, const char *password,int difPrt);
void save_user(char* name, char* password, int sock);
void endStoreUser(int sock);