#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "interface.h"
#include "degub.h"

char * USER = "";
char* TAB = "               ";
int USERREQUIRED = 0;

const int MATRIXLEN = 6;
char matrix[MATRIXLEN][MATRIXLEN]={{' '}};;




//matrix manipulation
char** initMatrix(){
    char matrix[MATRIXLEN][MATRIXLEN]={{' '}};
    return matrix;
}

char** setValueMatrix(char caseCoordinate[2], char value,char matrixIn[MATRIXLEN][MATRIXLEN]){
    char matrixRes[MATRIXLEN][MATRIXLEN] =  matrixIn;

    for (int i = 0; i < MATRIXLEN; i++)
    {
        for (int j = 0; j < MATRIXLEN; j++)
        {
            if( (int)caseCoordinate[1] == j && caseCoordinate[0]==(char)((int)'A'+i ) ){
                if( matrix[i][j] == 'O' && value=='X'){
                    matrixRes[i][j] = 'X';
                }
                else if(matrix[i][j]==' '){
                    matrixRes[i][j] = 'O';
                }
            }
        }
        
    }
    return matrixRes;
}

char  getValueMatrix(int caseCoordinateLetter, int caseCoordinateNumber,char matrixIn[MATRIXLEN][MATRIXLEN]){
    return matrixIn[caseCoordinateLetter][caseCoordinateNumber];
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



void queued(){
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


void logIn(void){    
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
    USER = username;
    queued();
}



void menuInterface(void){
    clear();
    printf("Welcome to the battleship game !\n%s",TAB);
    
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
        logIn();
    }
    else if (strcmp(awnser,"2")==0){
        exitMenu();
    }  
} 




//Admin
adminMenu(void){
    clear();    
    printf("Welcome back Administrator !\n%s",TAB);
    char awnser[1024];    
    int res = 1;
    while (res){
        printNewBlock();
        printf("Select your awnser\n%s 1 - Add New User\n%s 2 - Start new Game\n%s 3 - log Out\n%s",TAB,TAB,TAB,TAB);
        scanf("%s", awnser);
        printf("%s",TAB);
        char* listValueExpected[] = {"1","2","3",NULL};
        res = verifyAwnser(awnser,listValueExpected);
        if(res){
            printf("please enter a number correspoding with the rigth awnser\n%s",TAB);
        }
    }
    if(strcmp(awnser,"1")==0){
        adminAddUser();
    }
    else if (strcmp(awnser,"2")==0){
        adminInitGame();
    }
    else if (strcmp(awnser,"3")==0){
        exitMenu();
    }

}

adminAddUser(void){
    clear();
    printf("Adding a new User\n%s",TAB);
    //List curent user???
    char passwd[1024];
    char username[1024];    
    printNewBlock(); 
    printf("Enter a Username\n%s",TAB);
    scanf("%s", username);
    printf("\n%s",TAB);
    printf("Enter a password for this user\n%s"),TAB;
    printf("%s",TAB);
    scanf("%s", passwd);
    printf("%s",TAB);

    printf("User added ! Enter to continue\n%s",TAB);
    scanf("");
    adminMenu();
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
    if(strncmp(awnser,"y")){
        return 1;
    }
    else{
        return 0;
    }
}


adminInitGame(void){
    clear();
    printf("Initializing game\n%s",TAB);

    printNewBlock();
    char awnser[1024];    
    int res = 1;
    while (res){
        printNewBlock();
        printf("choose a number of required player to launch the Game [2-9]\n%s ",TAB);
        scanf("%s", awnser);
        printf("%s",TAB);
        char* listValueExpected = "[2-9]{1}";
        res = verifyAwnserRegex(awnser,listValueExpected);
        if(res){
            printf("please enter a number beetwen 2 and 9\n%s",TAB);
        }
    }
    //send player number to server
    USERREQUIRED = awnser;
    while (USERREQUIRED)
    {
        USERREQUIRED -= adminRequestPlayer("user 1");
    }
    printf("Number of player is good !\n%s",TAB);

    clear();
    printf("add a new ship !\n%s",TAB);



    

}






//Game
void game(void){
    clear();
    printf("\n\n\n%sGame Start !\n%s",TAB,TAB);
    int endGame = 1;
    while (endGame){
        
        printBattle();
        newTurn();
        clear();
    }
    
    menuInterface();   
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



void printBattle(void){
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
                    //FIX ME
                    printf("   ");
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