#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)
size_t nb_Case=6;
char* TAB = "               ";
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
    for(int i=0;i<6;i++){
        for(int j=0;j<6;j++){
            battleShipMatrix[i][j]=(int) buffer[i*6+j+1] - 48;
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
        for(int a =min(j1,j2) ;a<=max(j1,j2);a++){
            if (battleShipMatrix[i1][a]==1){
                return 0;
            }
        }
    }
    else if(j1==j2){
        for(int a =min(i1,i2) ;a<=max(i1,i2);a++){
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

int verifyGameEnd(int ** battleShipMatrix){
    for(int i=0;i<6;i++){
        for(int j=0;j<6;j++){
            if(battleShipMatrix[i][j]==1){
                return 0;
            }
        }
    }
    return 1;
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