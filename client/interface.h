

void setCharFromBattleShipMatrix(int** battleShipMatrix, char * caseMatrix,int value);
char* getCharFromBattleShipMatrix(int** battleShipMatrix, int i, int j);
int** initBattleShipMatrix(int** battleShipMatrix);



int verifyAwnser(char* value, char** listValueExpected);
int verifyAwnserRegex(char* value, char* reg);
//PARTIE MAIN MENU
void menuInterface(int difPrt);
void queued(int sock);
void exitMenu(void);
void logIn(int difPrt);


//admin
void adminMenu(int sock);
int adminRequestPlayer(char* playerName);
void adminAddUser(int sock,char** listUser);
void adminAddShip(int ** battleShipMatrix);
void adminInitGame(void);

//game
void game(void);
void printBattle(int ** battleShipMatrix);
void newTurn(void);
void attack(void);

void clear(void);