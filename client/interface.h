


int verifyAwnser(char* value, char** listValueExpected);
int verifyAwnserRegex(char* value, char* reg);
//PARTIE MAIN MENU
void menuInterface(void);
void queued(void);
void exitMenu(void);
void logIn(void);

//admin
adminMenu(void);
adminAddUser(void);
adminInitGame(void);

//game
void game(void);
void printBattle(void);
void newTurn(void);
void attack(void);

void clear(void);