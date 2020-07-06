#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include "interface.h"
#include "degub.h"
#include "comcli.h"



char * USER = "";
char* TAB = "               ";
int USERREQUIRED = 2;
int currentUser= 0;




const int MATRIXLEN = 6;
char **matrix;
size_t nb_Case=6;



//matrix manipulation


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
    char passwd[1024];
    clear();
    printNewBlock();
    printf("Welcome %s !\n%sYou are now in queue, wait for a new game to start\n%s",USER,TAB,TAB);
    //FIX ME
    //waiting for game
    scanf("%s", passwd);
    printNewBlock();
    printf("You have been selected for the next game, Please stand by !\n%s",TAB);
    //FIX ME
    //waiting for admin
    scanf("%s", passwd);
    printNewBlock();
    printf("Your game is ready, it's will starting in 3\n%s",TAB);
    sleep(1); 
    printf("2..\n%s",TAB);
    sleep(1); 
    printf("1..\n%s",TAB);
    sleep(1);
    game();
}


void logIn(int difPrt){    
    char passwd[1024];
    char username[1024];    
    printNewBlock(); 
    printf("Enter your Username\n%s",TAB);
    scanf("%s", username);
    printf("\n%s",TAB);
    printf("Enter your password\n%s"),TAB;
    printf("%s",TAB);
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
    size_t nb_User=10;
    listUser= calloc(nb_User+1,sizeof(char*));
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
                        printf("User required %d/%d, pleasse add a new user\n%s",currentUser,USERREQUIRED,TAB);
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
        adminInitGame();
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
    printf("Enter a password for this user\n%s"),TAB;
    printf("%s",TAB);
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
    printf("Adding a new Ship\n%s",TAB);
    char awnser[1024];    
    int res = 1;
    while (res)
    {
        
        printf("Enter the letter and number of case like [A-F][1-6] or cancel to go back\n%s",TAB);
        scanf("%s", awnser);
        printf("%s",TAB);
        char* listValueExpected[] = {"cancel",NULL};
        res = verifyAwnser(listValueExpected);
        if(res){
                res = verifyAwnserRegex(awnser,"[A-F]{1}[1-6]{1}");
            if(res){
                printf("Incorrect Input!\n%s\n%s",TAB,TAB);
            }
            else{
                setCharFromBattleShipMatrix(battleShipMatrix,awnser,1);
            }
        }
        else{

        }
        
    }
    

}

int adminRequestPlayer(char* playerName) {
    char awnser[1024];    
    int res = 1;
    while (res){
        printNewBlock();
        printf("%s requested to join do you accept? y or n\n%s",playerName,TAB);
        scanf("%s", awnser);
        printf("%s",TAB);
        char* listValueExpected[] = {"y","n",NULL};
        res = verifyAwnser(awnser,listValueExpected);
        if(res){
            printf("please enter y or n\n%s",TAB);
        }
    }
    //send awnser to server
    if(strcmp(awnser,"y")==0){
        return 1;
    }
    else{
        return 0;
    }
}


void adminInitGame(void){
    clear();
    printf("Initializing game\n%s",TAB);
    int ** battleShipMatrixPlayer1;
    int ** battleShipMatrixPlayer2;
    battleShipMatrixPlayer1 = initBattleShipMatrix(battleShipMatrixPlayer1);
    battleShipMatrixPlayer2 = initBattleShipMatrix(battleShipMatrixPlayer2);

    //FIX ME
    adminRequestPlayer("user 2");
    adminRequestPlayer("user 1");
    printf("Number of player is good !\n%s",TAB);
    
    int finish =1;
    while(finish){
        clear();       
        printf("Add some ship !\n%s",TAB);
        int res = 1;
        char awnser[1024];
        while (res){
            printNewBlock();
            printf("Select your awnser\n%s 1 - Add New Ship for player 1 \n%s 2 - Add New Ship for player 2\n%s 3 - finishSetup\n%s",TAB,TAB,TAB,TAB);
            scanf("%s", awnser);
            printf("%s",TAB);
            char* listValueExpected[] = {"1","2",NULL};
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
    

}






//Game Normal User
void game(){
    clear();
    printf("\n\n\n%sGame Start !\n%s",TAB,TAB);
    int endGame = 1;
    int ** battleShipMatrix;
    while (endGame){
        
        printBattle(battleShipMatrix);
        newTurn();
        clear();
    }
    
    menuInterface(1);   
}

void newTurn(void){
    //wait for server instruction

    //your turn or waiting for other player
    attack();
}

void attack(void){
    printNewBlock();
    printf("It's your turn!\n%s",TAB);
    char awnser[1024];    
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


void clear(void){
    system("clear");
}