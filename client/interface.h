

void setCharFromBattleShipMatrix(int** battleShipMatrix, char * caseMatrix,int value);
char* getCharFromBattleShipMatrix(int** battleShipMatrix, int i, int j);
int** initBattleShipMatrix(int** battleShipMatrix);
int setMatrixbeetwen2Case(int** battleShipMatrix, char * caseMatrix1,char * caseMatrix2);
void stringToMatrix(char* buffer,int ** battleShipMatrix);
char * matirixToBuffer(int ** battleShipMatrix);


int verifyAwnser(char* value, char** listValueExpected);
int verifyAwnserRegex(char* value, char* reg);
//PARTIE MAIN MENU
void menuInterface(int difPrt);
void queued(int sock);
void exitMenu(void);
void logIn(int difPrt);


//admin
void adminMenu(int sock);
int adminRequestPlayer(int sock);
void adminAddUser(int sock,char** listUser);
void adminAddShip(int ** battleShipMatrix);
void adminInitGame(int sock, char** listUser);

//game
void game(int sock, char* buffer);
void printBattle(int ** battleShipMatrix);
void printBattle2Matrix(int ** battleShipMatrix,int ** battleShipMatrixShow);
int newTurn(int sock, int **battleShipMatrix,int ** battleShipMatrixShow);
void attack(int sock);

void clear(void);