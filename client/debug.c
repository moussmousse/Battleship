#include <stdio.h>
#include <string.h>
#include "degub.h"


void debugLog(char* value){
   
    printf( "DEBUG -> %s\n",value);
}
void debugLogV2(char* value,char* value2){
   
    printf( "DEBUG -> %s | %s\n",value,value2);
}