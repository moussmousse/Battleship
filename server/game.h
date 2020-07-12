
void setCharFromBattleShipMatrix(int** battleShipMatrix, char * caseMatrix,int value);
char* getCharFromBattleShipMatrix(int** battleShipMatrix, int i, int j);
int** initBattleShipMatrix(int** battleShipMatrix);
int setMatrixbeetwen2Case(int** battleShipMatrix, char * caseMatrix1,char * caseMatrix2);
void stringToMatrix(char* buffer,int ** battleShipMatrix);
char * matirixToBuffer(int ** battleShipMatrix);
int verifyMatrixbeetwen2Case(int** battleShipMatrix, char * caseMatrix1,char * caseMatrix2);
int verifyGameEnd(int ** battleShipMatrix);
void printBattle2Matrix(int ** battleShipMatrix,int ** battleShipMatrixShow);