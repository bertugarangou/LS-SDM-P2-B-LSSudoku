#include <xc.h>
#include "Usuaris.h"

unsigned char currentUsrIndex = 0;

unsigned char numUsuaris = 0;
__bit createUser = 0;
unsigned char indexLastUser;

char *tmpUsername;
char *tmpPassword;

typedef struct{
    char username[9];
    char password[9];
}Usuari;
Usuari usuaris[8];

char UgetNumUsuaris(void){
    return numUsuaris;
}

void UcreateUser(void){
    createUser = 1;
}
void UsetData(char user[], char pass[]){
    tmpUsername = user;
    tmpPassword = pass;
}
void Uinit(){
    /*
     | num totals 8 | index last 8 | user1 8 | pass1 8 | user2 9 | pass2 9 |
     */
    EEADR = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    while(EECON1bits.RD == 1){}
    numUsuaris = EEDATA;
    
    EEADR++;
    while(EECON1bits.RD == 1){}
    indexLastUser = EEDATA;
    EEADR++;
    
    for(char i = 0; i< numUsuaris; i++){//i-> usuari
        for(char j = 0; j<9; j++){//j-> caracter
            EECON1bits.RD = 1;
            while(EECON1bits.RD == 1){}
            usuaris[i].username[j] = EEDATA;
            EEADR++;
        }
        for(char j = 0; j<9; j++){//j-> caracter
            EECON1bits.RD = 1;
            while(EECON1bits.RD == 1){}
            usuaris[i].password[j] = EEDATA;
            EEADR++;
        }        
    }
    
    void UmotorUsers(){
        
    }
    //TODO: fer que al primer cop es guardin tot '\0' a la eeprom o almenys als bits de users
    
}
