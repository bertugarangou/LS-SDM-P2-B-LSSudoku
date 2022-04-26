#include <xc.h>
#include "Usuaris.h"

unsigned char currentUsrIndex = 0;

unsigned char numUsuaris = 0;
__bit do_check_exists = 0;
unsigned char indexLastUser;
__bit return_error;
unsigned char i;
unsigned char j;

char tmpUsername[9];
char tmpPassword[9];

typedef struct{
    char username[9];
    char password[9];
    unsigned char scores[5];
}Usuari;
Usuari usuaris[8];


char UgetNumUsuaris(void){
    return numUsuaris;
}

void UcheckExists(void){    //començar a comprovar
    do_check_exists = 1;
}
__bit UcheckExistsNotFinished(void){    //saber si ha acabat de comprovar
    return do_check_exists;
}
__bit UcheckExistsGetError(){//saber si existeix 1 o no 0
    return return_error;
}
void UenviaChar(char c, char pos){
    tmpUsername[pos] = c;
}
void UenviaPas(char c, char pos){
    tmpPassword[pos] = c;
}
void Uinit(){
    //escriureEEPROM();
    /*
     | num totals 8 | index last 8 | user1 8 | pass1 8 | user2 9 | pass2 9 |
     */
    EEADR = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    while(EECON1bits.RD == 1){}//bucle al init
    numUsuaris = EEDATA;
    //numUsuaris = 8; //canviar, mirar el comentari de just sota
    /*******************CAL ESCRIURE UN 0 AQUI AL PRIMER COP*********/
    
    EEADR++;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    
    while(EECON1bits.RD == 1){}//bucle al init
    indexLastUser = EEDATA;
    EEADR++;
    
    for(char i = 0; i< numUsuaris; i++){//i-> usuari
        for(char j = 0; j<9; j++){//j-> caracter
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.RD = 1;
            while(EECON1bits.RD == 1){}
            usuaris[i].username[j] = EEDATA;
            EEADR++;
        }
        for(char j = 0; j<9; j++){//j-> caracter
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.RD = 1;
            while(EECON1bits.RD == 1){}
            usuaris[i].password[j] = EEDATA;
            EEADR++;
        }
    }
        
}
    
char compareStrings(const char *a, const char *b){
    while (*a){
        if (*a != *b)break; 
        a++;
        b++;
    }
    return *(const unsigned char*)a - *(const unsigned char*)b;
}


void UmotorUsers(){
static char state = 0;

	switch(state) {
		case 0:
			if (do_check_exists == 0) {
			}
			else if (do_check_exists == 1) {
				return_error = 0;
				i = 0;
				j = 0;
				state = 1;
			}
		break;
		case 1:
			if (i < numUsuaris) {
				if(compareStrings(tmpUsername, usuaris[i].username) == 0){
					return_error = i;/**************************************************RETORNA L'INDEX DEL USER*******/
				//si son iguals;
				}
				i++;
				state = 1;
			}
			else if (i == numUsuaris) {
				do_check_exists = 0;
				state = 0;
			}
		break;
	}
    
}

void escriureEEPROM(){
    for(unsigned char caca = 0; caca < 160; caca++){
        EEADR = 0;
        EEDATA = 0;
        EECON1bits.EEPGD = 0;
        EECON1bits.CFGS = 0;
        EECON1bits.WREN = 1;
        INTCONbits.GIE = 0;
        EECON2 = 85;
        EECON2 = 170;
         EECON1bits.WR = 1;
        while(EECON1bits.WR){
            
           

        }
    }
   INTCONbits.GIE = 1;
}

