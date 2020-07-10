#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "interface.h"
#include "degub.h"
#include "comcli.h"


#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)


char * USER = "";
char* TAB = "               ";
int USERREQUIRED = 2;
int currentUser= 0;
char* listUserConnected[6];
char* listUserCreated[6];




const int MATRIXLEN = 6;
char **matrix;
size_t nb_Case=6;
int CPT_QUEUE = 0;
int resThreadQueueUser = 1;


char * matirixToBuffer(int ** battleShipMatrix){
    char *buffer;
    buffer = malloc(sizeof(char)*38);
    memset(buffer,0,38);
    buffer[0]='0';
    for(int i=0; i<6;i++){
        for(int j=0;j<6;j++){
            buffer[6*i+j+1]=(char) battleShipMatrix[i][j]+48;
        }
    }
    buffer[37]='\0';

    return buffer;
}

//matrix manipulation
void stringToMatrix(char* buffer,int ** battleShipMatrix){
    printf("test");
    for(int i=0;i<6;i++){
        for(int j=0;j<6;j++){
            
            battleShipMatrix[i][j]=(int) buffer[i*6+j+1] -48;
        }
    }
}

void setCharFromBattleShipMatrix(int** battleShipMatrix, char * caseMatrix,int value){
    
    int i = caseMatrix[0] - 65;
    int j = caseMatrix[1] - 49;
    battleShipMatrix[i][j]=value;
}


char* getCharFromBattleShipMatrix(int** battleShipMatrix, int i, int j){
    switch(battleShipMatrix[i][j]){
        case 0:
            return " ";
            break;
        case 1:
            return "O";
            break;
        case 2:
            return "X";
            break;
    }
}

int** initBattleShipMatrix(int** battleShipMatrix){
    battleShipMatrix= malloc(nb_Case*sizeof(int*));

    for (int i=0;i<nb_Case;i++){
        battleShipMatrix[i] = malloc(nb_Case*sizeof(int));
        for (int j=0;j<nb_Case;j++){
            battleShipMatrix[i][j]=0;
        }
    }
    return battleShipMatrix;
}

int verifyMatrixbeetwen2Case(int** battleShipMatrix, char * caseMatrix1,char * caseMatrix2){
    int i1 = caseMatrix1[0] - 65;
    int j1 = caseMatrix1[1] - 49;
    int i2 = caseMatrix2[0] - 65;
    int j2 = caseMatrix2[1] - 49;
    if(i1==i2){
        for(int a =min(j1,j2) ;a<max(j1,j2);a++){
            if (battleShipMatrix[i1][a]==1){
                return 0;
            }
        }
    }
    else if(j1==j2){
        for(int a =min(i1,i2) ;a<max(i1,i2);a++){
            if (battleShipMatrix[a][j1]==1){
                return 0;
            }
        }
    }
    else{
        return 0;
    }
    return 1;
}

int setMatrixbeetwen2Case(int** battleShipMatrix, char * caseMatrix1,char * caseMatrix2) {
    int i1 = caseMatrix1[0] - 65;
    int j1 = caseMatrix1[1] - 49;
    int i2 = caseMatrix2[0] - 65;
    int j2 = caseMatrix2[1] - 49;
    if(i1==i2){
        for(int a =min(j1,j2) ;a<=max(j1,j2);a++){
            if (battleShipMatrix[i1][a]==1){
                
                return 0;
            }
        }
        for(int a =min(j1,j2) ;a<=max(j1,j2);a++){
            char str[3];
            str[0]=(char)  i1 + 65;
            str[1]=(char)  a + 49;
            str[2]='\0';
            setCharFromBattleShipMatrix(battleShipMatrix,str,1);
        }
    }
    else if(j1==j2){
        for(int a =min(i1,i2) ;a<=max(i1,i2);a++){
            if (battleShipMatrix[a][j1]==1){
                return 0;
            }
        }
        for(int a =min(i1,i2) ;a<=max(i1,i2);a++){
            char str[3];
            str[0]=(char)  a + 65;
            str[1]=(char)  j1 + 49;
            str[2]='\0';
            setCharFromBattleShipMatrix(battleShipMatrix,str,1);
        }
    }
    else{
        return 0;
    }
    return 1;
}







int verifyAwnser(char* value, char** listValueExpected){   
    int res = 1;
    while (*listValueExpected && strcmp(value,*listValueExpected)!=0 ){
        listValueExpected++;            
    }
    if(*listValueExpected && strcmp(value,*listValueExpected)==0){
            res--;
    }   
    return res;
}

int verifyAwnserRegex(char* value, char* reg){
    int err;
   regex_t preg;
   const char *str_regex = reg;
    int res=1;
    int match;
   err = regcomp (&preg, str_regex, REG_NOSUB | REG_EXTENDED);
   if (err == 0)
   {      
      match = regexec (&preg, value, 0, NULL, 0);

      regfree (&preg);

      
    }
    if(match==0){
        res=0;
    }
    return res;
}




void printNewBlock(){
    printf("\n%s\n%s\n%s",TAB,TAB,TAB);
    for (int i=0;i<30;i++){
        printf("=");
    }
    printf("\n%s",TAB); 
}

//PARTIE MAIN MENU
void exitMenu(void){
    printf("exit !\n%s",TAB);
}



void queued(int sock){
    char* buffer;
    buffer = malloc(sizeof(char)*38);
    memset(buffer,0,38);
    clear();
    printNewBlock();
    printf("Welcome %s !\n%sYou are now in queue, wait for a new game to start\n%s",USER,TAB,TAB);
    buffer = wait_for_matrix(sock);
    printNewBlock();
    printf("You have been selected for the next game, Please stand by !\n%s",TAB);
    printf("Your game is ready, it's will starting in 3\n%s",TAB);
    sleep(1); 
    printf("2..\n%s",TAB);
    sleep(1); 
    printf("1..\n%s",TAB);
    sleep(1);
    game(sock, buffer);
}


void logIn(int difPrt){    
    char passwd[1024];
    char username[1024];    
    printNewBlock(); 
    printf("Enter your Username\n%s",TAB);
    scanf("%s", username);
    printf("\n%s",TAB);
    printf("Enter your password\n%s",TAB);
    scanf("%s", passwd);
    printf("%s",TAB);
    //
    //FIX ME
    int sock =init_session(username, passwd, difPrt);

    if ( sock== -1){
        printf("Invalid credentials");
         exit(1);
    }
      
    USER = username;
    if(strcmp(USER,"admin")==0){
        adminMenu(sock);
    }
    else
    {
        queued(sock);
    }
    
    
}



void menuInterface(int difPrt){
    clear();
    printf("%sWelcome to the battleship game !\n%s",TAB,TAB);

    char awnser[1024];    
    int res = 1;
    while (res)
    {
        printNewBlock();
        printf("Select your awnser\n%s 1 - log in\n%s 2 - Exit\n%s",TAB,TAB,TAB);
        scanf("%s", awnser);
        printf("%s",TAB);
        char* listValueExpected[] = {"1","2",NULL};
        res = verifyAwnser(awnser,listValueExpected);
        if(res){
            printf("please enter a number correspoding with the rigth awnser\n%s",TAB);
        }
        
    }
    if(strcmp(awnser,"1")==0){
        logIn(difPrt);
    }
    else if (strcmp(awnser,"2")==0){
        exitMenu();
    }  
} 




//Admin
void adminMenu(int sock){
    clear();    
    printf("Welcome back Administrator !\n%s",TAB);
    char** listUser;
    size_t nb_User=6;
    listUser= calloc(nb_User,sizeof(char*));
    for(int i=0;i<nb_User;i++){
        listUser[i] = " "; 
    }
    if (!listUser) { perror("calloc listUser"); exit (EXIT_FAILURE); };
    char awnser[1024];   
    int res = 1;
    while (res){
        printNewBlock();
        printf("Select your awnser\n%s 1 - Add New User\n%s 2 - log Out\n%s",TAB,TAB,TAB);
        scanf("%s", awnser);
        printf("%s",TAB);
        char* listValueExpected[] = {"1","2","3",NULL};
        res = verifyAwnser(awnser,listValueExpected);
        if(res){
            printf("please enter a number correspoding with the rigth awnser\n%s",TAB);
        }
    }
    if(strcmp(awnser,"1")==0){
        int finish =1;
        while (finish)
        {
            clear();
            adminAddUser(sock,listUser);
            currentUser++;
            for(int i=0;i<currentUser;i++){
                printf("   - %s\n%s" ,listUser[i],TAB);
            }
            char awnser2[1024];    
            int res = 1;
            while (res){
                printNewBlock();
                printf("User required %d/%d \n%s",currentUser,USERREQUIRED,TAB);
                printf("Do you want to add another user? y or n\n%s",TAB);
                scanf("%s", awnser2);
                printf("%s",TAB);
                char* listValueExpected[] = {"y","n",NULL};
                res = verifyAwnser(awnser2,listValueExpected);
                if(strcmp(awnser2,"n")==0 && currentUser<USERREQUIRED){
                        printf("\n%s User required %d/%d, pleasse add a new user\n%s",TAB,currentUser,USERREQUIRED,TAB);
                        adminAddUser(sock,listUser);
                }
                else if(res){                  
                    printf("please enter y or n\n%s",TAB);
                }
                
            }
            if(strcmp(awnser2,"n")==0){
                finish=0;
                endStoreUser(sock);
            }
        }
        adminInitGame(sock,listUser);
    }
    else if (strcmp(awnser,"2")==0){
        exitMenu();
    }

}

void adminAddUser(int sock, char** listUser){
    
    printf("Adding a new User\n%s",TAB);
    printf("Curernt User List:\n%s",TAB);
    for(int i=0;i<currentUser;i++){
        printf("   - %s\n%s" ,listUser[i],TAB);
    }
    

    char passwd[32];
    char username[32];    
    memset(passwd,0,32);
    memset(username,0,32);
    printNewBlock(); 
    printf("Enter a Username\n%s",TAB);
    scanf("%s\0", username);
    printf("\n%s",TAB);
    printf("Enter a password for this user\n%s",TAB);
    scanf("%s\0", passwd);
    printf("%s",TAB);

    //send new user to server
    save_user(username,passwd,sock);
    listUser[currentUser]= strdup(username);


    printf("User added !\n%s",TAB);


}


void adminAddShip(int ** battleShipMatrix) {
    clear(); 
    printBattle(battleShipMatrix);
    printNewBlock(); 
    printf("Adding a new Ship between 2 case\n%s",TAB);
    char awnser[64];    
    char awnser2[64];
    int res = 1;
    while (res)
    {
        
        printf("Enter the letter and number of the 1st case like [A-F][1-6] or cancel to go back\n%s",TAB);
        scanf("%s", awnser);
        printf("%s",TAB);
        char* listValueExpected[] = {"cancel",NULL};
        res = verifyAwnser(awnser,listValueExpected);
        if(res){
                res = verifyAwnserRegex(awnser,"[A-F]{1}[1-6]{1}");
            if(res){
                printf("Incorrect Input!\n%s\n%s",TAB,TAB);
            }
        }
        else{

        }
        
    }
    res = 1;
    while (res)
    {
        
        printf("Enter the letter and number of the 2nd case like [A-F][1-6] or cancel to go back\n%s",TAB);
        scanf("%s", awnser2);
        printf("%s",TAB);
        char* listValueExpected[] = {"cancel",NULL};
        res = verifyAwnser(awnser2,listValueExpected);
        if(res){
                res = verifyAwnserRegex(awnser2,"[A-F]{1}[1-6]{1}");
            if(res){
                printf("Incorrect Input!\n%s\n%s",TAB,TAB);
            }
        }
        else{

        }
        
    }
    if(setMatrixbeetwen2Case(battleShipMatrix,awnser,awnser2)==0){
        printf("Coordinate are not good!");
    }

}

int adminRequestPlayer(int sock) {
    char awnser[64];   
    char listUserAccepted[2];
    int cptAccept=0;
    while(cptAccept<2){
        printNewBlock();
        printf("List of connected User :\n%s",TAB);
        int cpt=0;
        for(int i=0;i<6;i++){
            if(strcmp(listUserConnected[i],"")>3){
                if(cptAccept==1 && listUserAccepted[0]==(char)i+48 ){
                    printf(" -%d[X] %s\n%s",i+1,listUserConnected[i],TAB);
                }
                else{
                    printf(" -%d[ ] %s\n%s",i+1,listUserConnected[i],TAB);
                }                  
                cpt++;
            }
            
        }
        char str[12];
        sprintf(str,"%d",cpt);
        
        int res = 1; 
        while (res){
            printf("Select 2 user, enter a number beetwen 1 and %s\n%s",str,TAB);
            scanf("%s", awnser);
            printf("\n%s",TAB);
            char* listValueExpected[] = {"y","n",NULL};
            char str2[12];
            sprintf(str2,"[1-%d]{1}",cpt);
            res = verifyAwnserRegex(awnser,str2);
            if(res){
                printf("please enter a number beetwen 1 and %s\n%s",str,TAB);
            }
            else{
                if(cptAccept==1 && listUserAccepted[0]==(char)atoi(awnser)+47 ){
                    res=1;
                    printf("This user is already selected, please add a new user\n%s",TAB);
                }
                else{
                    listUserAccepted[cptAccept]=(char)atoi(awnser)+47;
                    cptAccept++;
                }
                
            }
        }
    }
    char string1[2] ="";
    char string2[2] ="";
    string1[0]= listUserAccepted[0];
    string2[0]=listUserAccepted[1];
    debugLogV2(string1,string2);
    //send awnser to server
    send_selected_users(sock,listUserAccepted[0],listUserAccepted[1]);
}

void *userQueue(void *sock){
    while(resThreadQueueUser) {
         
        char *str=malloc(sizeof(char)*64);
        str=wait_users(*((int*) sock));       
        if (str!=NULL){
            debugLog("new User"); 
            strcpy(listUserConnected[CPT_QUEUE],listUserCreated[(int)str[0]-48]);
            CPT_QUEUE++;
        }
        else{
         usleep(1000);
        }
    }
    pthread_exit(NULL);
   
}


void adminInitGame(int sock, char** listUser){
    clear();
    
    for(int i=0;i<6;i++){
        listUserConnected[i]=calloc(32,sizeof(char));
        listUserCreated[i]=calloc(32,sizeof(char));
    }

    for(int i=0;i<6;i++){
        strcpy(listUserCreated[i],listUser[i]);
    }

    pthread_t thread_id;
    int rc = pthread_create(&thread_id, NULL, userQueue, (void *)&sock);
    if (rc) {
        printf("Error:unable to create thread, %d\n", rc);
        exit(-1);
    }


    printf("%sInitializing game\n%s",TAB,TAB);
    printf("waiting for player to join game, press enter to continue\n%s",TAB);
    
    getchar();
    getchar();
    
    int ** battleShipMatrixPlayer1;
    int ** battleShipMatrixPlayer2;
    battleShipMatrixPlayer1 = initBattleShipMatrix(battleShipMatrixPlayer1);
    battleShipMatrixPlayer2 = initBattleShipMatrix(battleShipMatrixPlayer2);



    int finish =1;
    while(finish){
        clear();       
        printf("Add some ship !\n%s",TAB);
        int res = 1;
        char awnser[64];
        while (res){
            printNewBlock();
            printf("Select your awnser\n%s 1 - Add New Ship for player 1 \n%s 2 - Add New Ship for player 2\n%s 3 - finishSetup\n%s",TAB,TAB,TAB,TAB);
            scanf("%s", awnser);
            printf("%s",TAB);
            char* listValueExpected[] = {"1","2","3",NULL};
            res = verifyAwnser(awnser,listValueExpected);
            if(res){
                printf("please enter a number correspoding with the rigth awnser\n%s",TAB);
            }
        }
        if(strcmp(awnser,"1")==0){
            adminAddShip(battleShipMatrixPlayer1);
            
        }
        else if (strcmp(awnser,"2")==0) {
            adminAddShip(battleShipMatrixPlayer2);
        }
        else if (strcmp(awnser,"3")==0){
           finish=0;
        }
    }
    
    printf("waiting for player to join game, press enter to continue\n%s",TAB);

    getchar();
    getchar();
    

    //end user queue
    resThreadQueueUser = 0;
    end_wait_users(sock);
    

    //Select User   
    adminRequestPlayer(sock);
    printf("Number of player is good !, press enter to continue\n%s",TAB);
    getchar();
    getchar();

    char *buffer1 ;
    buffer1 = malloc(sizeof(char)*38);
    memset(buffer1,0,38);
    buffer1=matirixToBuffer(battleShipMatrixPlayer1);
    char *buffer2;
    buffer2 = malloc(sizeof(char)*38);
    memset(buffer2,0,38);
    buffer2=matirixToBuffer(battleShipMatrixPlayer2);
    send_matrix(sock,buffer1,1);  
    send_matrix(sock,buffer2,2);
    free(buffer1);
    free(buffer2);
 

}






//Game Normal User
void game(int sock, char* buffer){
    clear();
    printf("\n\n\n%sGame Start !\n%s",TAB,TAB);
    int endGame = 1;
    int ** battleShipMatrix;
    battleShipMatrix = initBattleShipMatrix(battleShipMatrix);
    int ** battleShipMatrixShow;
    battleShipMatrixShow = initBattleShipMatrix(battleShipMatrixShow);
    stringToMatrix(buffer,battleShipMatrix);
    while (endGame){
        
        printBattle2Matrix(battleShipMatrix,battleShipMatrixShow);
        endGame = newTurn(sock,battleShipMatrix,battleShipMatrixShow);
        clear();
    }
    
    menuInterface(1);   
}

int newTurn(int sock, int **battleShipMatrix,int ** battleShipMatrixShow){
    //wait for server instruction
    int turn = wait_for_turn(sock);
    if(turn == 1){
        attack(sock);
        char* buffer;
        buffer = malloc(sizeof(char)*38);
        memset(buffer,0,38);
        buffer = wait_for_matrix(sock);       
        printf("%s\n%s",buffer,TAB);
        char* buffer1;
        buffer1 = malloc(sizeof(char)*38);
        memset(buffer1,0,38);
        buffer1 = wait_for_matrix(sock);
        stringToMatrix(buffer1,battleShipMatrixShow);  
        free(buffer);
        free(buffer1);
         sleep(4);
        return 1;
        
    }
    else if (turn == 0) {
        printf("It's the other player turn, wait for his action!\n%s",TAB);
        char* buffer1;
        buffer1 = malloc(sizeof(char)*38);
        memset(buffer1,0,38);
        buffer1 = wait_for_matrix(sock);
        printf("%s\n%s",buffer1,TAB);
        char* buffer;
        buffer = malloc(sizeof(char)*38);
        memset(buffer,0,38);
        buffer = wait_for_matrix(sock);
        stringToMatrix(buffer,battleShipMatrix);             
        free(buffer);
        free(buffer1); 
         sleep(4); 
        return 1;
    }
    else if (turn == 2){
        printf("\n\nCongratialtion you win !!! \n\n");
        sleep(15); 
        return 0;
    }
    else if (turn == 3){
        printf("\n\nYou're a looser. \n\n");
        sleep(15); 
        return 0;
    }
   
    //your turn or waiting for other player
}

void attack(int sock){
    printNewBlock();
    printf("It's your turn!\n%s",TAB);
    char awnser[64];    
    int res = 1;
    while (res)
    {
        
        printf("Enter the letter and number of case like [A-F][1-6] \n%s",TAB);
        scanf("%s", awnser);
        printf("%s",TAB);
        res = verifyAwnserRegex(awnser,"[A-F]{1}[1-6]{1}");
        if(res){
            printf("Incorrect Input!\n%s\n%s",TAB,TAB);
        }
    }
    send_Pos(sock,awnser,1);
   
}



void printBattle(int ** battleShipMatrix){
    printf("\n%s",TAB);
    for (int i = 0; i < 13; i++){
       if(i%2==0){
            printf(" ");
        }
        else{
            printf("  %d", (i/2)+1) ;
            
        }
        
    }
    printf("\n%s ",TAB);
    for (int i = 0; i < 13; i++){
       if(i%2==0){
            printf("+");
        }
        else{
            printf("---") ;
        }
        
    }
    printf("\n%s",TAB);
    for(int i=1;i<12;i++){
        
        for (int j = -1; j < 13; j++)
        {
            if(j == -1){
                if(i%2!=0){
                    printf("%c", (char)((int)'A'+((i)/2)));
                }
                else{
                    printf(" ");
                }
            }
            else if(i%2==0){
                if(j%2==0){
                    printf("+");
                }
                else{
                    printf("---") ;
                }
                
            }
            else {
                if(j%2==0){
                    printf("|");
                }
                else {
                    
                    printf(" %s ",getCharFromBattleShipMatrix(battleShipMatrix,i/2,j/2));
                }
            }
            
        }
        printf("\n%s",TAB);
        
    }
    printf(" ");
    for (int i = 0; i < 13; i++){
        if(i%2==0){
            printf("+");
        }
        else{
            printf("---") ;
        }
    }
    printf("\n%s",TAB);
}

void printBattle2Matrix(int ** battleShipMatrix,int ** battleShipMatrixShow){
    printf("\n%s",TAB);
    for (int i = 0; i < 13; i++){
       if(i%2==0){
            printf(" ");
        }
        else{
            printf("  %d", (i/2)+1) ;
            
        }
        
    }
    printf("%s ",TAB);
    for (int i = 0; i < 13; i++){
       if(i%2==0){
            printf(" ");
        }
        else{
            printf("  %d", (i/2)+1) ;
            
        }
        
    }
    printf("\n%s ",TAB);
    for (int i = 0; i < 13; i++){
       if(i%2==0){
            printf("+");
        }
        else{
            printf("---") ;
        }
        
    }
    printf("%s ",TAB);
    for (int i = 0; i < 13; i++){
       if(i%2==0){
            printf("+");
        }
        else{
            printf("---") ;
        }
        
    }
    printf("\n%s",TAB);
    for(int i=1;i<12;i++){
        
        for (int j = -1; j < 13; j++)
        {
            if(j == -1){
                if(i%2!=0){
                    printf("%c", (char)((int)'A'+((i)/2)));
                }
                else{
                    printf(" ");
                }
            }
            else if(i%2==0){
                if(j%2==0){
                    printf("+");
                }
                else{
                    printf("---") ;
                }
                
            }
            else {
                if(j%2==0){
                    printf("|");
                }
                else {
                    
                    printf(" %s ",getCharFromBattleShipMatrix(battleShipMatrix,i/2,j/2));
                }
            }
            
            
        }
        printf("%s",TAB);
        for (int j = -1; j < 13; j++)
        {
            if(j == -1){
                if(i%2!=0){
                    printf("%c", (char)((int)'A'+((i)/2)));
                }
                else{
                    printf(" ");
                }
            }
            else if(i%2==0){
                if(j%2==0){
                    printf("+");
                }
                else{
                    printf("---") ;
                }
                
            }
            else {
                if(j%2==0){
                    printf("|");
                }
                else {                   
                    printf(" %s ",getCharFromBattleShipMatrix(battleShipMatrixShow,i/2,j/2));
                }
            }
            
            
        }
        printf("\n%s",TAB);
        
    }
    printf(" ");
    for (int i = 0; i < 13; i++){
        if(i%2==0){
            printf("+");
        }
        else{
            printf("---") ;
        }
    }
    printf("%s ",TAB);
    for (int i = 0; i < 13; i++){
        if(i%2==0){
            printf("+");
        }
        else{
            printf("---") ;
        }
    }
    printf("\n%s",TAB);
}
void clear(void){
    system("clear");
}