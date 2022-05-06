#include <xc.h>
#include "Usuaris.h"
#include "LcTLCD.h"

unsigned char numUsuaris = 0;
__bit do_check_exists = 0;
unsigned char indexNextUser;
unsigned char indexNextUserStruct;
__bit do_register = 0;
unsigned char i;
unsigned char j;
signed char usuariLoguejat = -1;
__bit borram;
__bit do_showUsers = 0;
__bit do_updateScores = 0;
signed char nouIndex = -1;
unsigned char novaScore = -1;

signed char arrayShowUsers[8];

char tmpUsername[9];
char tmpPassword[9];

typedef struct{
    char username[9];
    char password[9];
    unsigned char scores[5];
}Usuari;
Usuari usuaris[8];

typedef struct{
    signed char indexStruct;
    unsigned char score;
}Scores;
Scores puntuacions[5];


//funcio que actualitza les millors puntuacions
void UchangeScore(char quin, char score){
    novaScore = score;
    nouIndex = quin;
}


char UgetNumUsuaris(void){
    return numUsuaris;
}

void UcheckExists(void){    //començar a comprovar
    do_check_exists = 1;
}
__bit UcheckExistsNotFinished(void){    //saber si ha acabat de comprovar
    return do_check_exists;
}
signed char UcheckExistsGetError(){//saber si existeix -1 o ID
    return usuariLoguejat;
}
void UenviaChar(char c, char pos){
    tmpUsername[pos] = c;
}
void UenviaPas(char c, char pos){
    tmpPassword[pos] = c;
}

void URegister(void){
    do_register = 1;
}

char* UgetUserName(char quin){
    return usuaris[quin].username;
}

char* getArrayShowUsers(char quin){
    
    return usuaris[arrayShowUsers[quin]].username;
}
void escriureCharEEPROM(char c, char pos){
    EEADR = pos;
    EEDATA = c;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    EECON2 = 85;
    EECON2 = 170;
    EECON1bits.WR = 1;
    while(EECON1bits.WR){}
    INTCONbits.GIE = 1;
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
    
    EEADR++;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    
    while(EECON1bits.RD == 1){}//bucle al init
    indexNextUser = EEDATA;
    
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

//BORRAR A FINAL
void escriure2usuarisStruct(void){
    //de manera forçada crea dos usuaris
    usuaris[0].username[0] = '1';
    usuaris[0].username[1] = '\0';
    
    usuaris[1].username[0] = 'A';
    usuaris[1].username[1] = '\0';
    
    usuaris[2].username[0] = 'D';
    usuaris[2].username[1] = '\0';
    
    usuaris[3].username[0] = 'G';
    usuaris[3].username[1] = '\0';
    usuaris[4].username[0] = 'c';
    usuaris[4].username[1] = '\0';

    puntuacions[0].indexStruct = 4;
    puntuacions[1].indexStruct = 3;
    puntuacions[2].indexStruct = 2;
    puntuacions[3].indexStruct = 1;
    puntuacions[4].indexStruct = 0;
    puntuacions[5].indexStruct = -1;
    puntuacions[0].score = 10;
    puntuacions[1].score = 11;
    puntuacions[2].score = 12;
    puntuacions[3].score = 13;
    puntuacions[4].score = 1;
            
    numUsuaris = 4;
    indexNextUserStruct = 4;
}
    
char compareStrings(const char *a, const char *b){
    while (*a){
        if (*a != *b)break; 
        a++;
        b++;
    }
    return *(const unsigned char*)a - *(const unsigned char*)b;
}

void calculateShowUsers(void){
    do_showUsers = 1;
}
__bit UshowUsersCalculat(void){
    return !do_showUsers;
}

void UmotorUsers(){
    static char state = 0;

	switch(state) {
		case 0:
			if (!do_check_exists && !do_register && !do_updateScores && !do_showUsers) {
				i = 0;
				j = 0;
			}
			else if (do_check_exists == 1) {
				usuariLoguejat = -1;
				state = 1;
			}
			else if (do_register == 1) {
				state = 2;
			}
			else if (do_updateScores) {
				state = 4;
			}
			else if (do_showUsers) {
				arrayShowUsers[0] = usuariLoguejat;
				arrayShowUsers[1] = -1;
				arrayShowUsers[2] = -1;
				arrayShowUsers[3] = -1;
				arrayShowUsers[4] = -1;
				arrayShowUsers[5] = -1;
				arrayShowUsers[6] = -1;
				arrayShowUsers[7] = -1;
				i = 1;
				if(numUsuaris < 7) {
				  j = 0;
				}
				else{
				  j = indexNextUserStruct;
				}
				state = 5;
			}
		break;
		case 1:
			if (i < numUsuaris) {
				if(compareStrings(tmpUsername, usuaris[i].username) == 0){
					usuariLoguejat = i;
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
		case 2:
			if (i != 8) {
				escriureCharEEPROM(tmpUsername[i],indexNextUser++);
				usuaris[indexNextUserStruct].username[i] = tmpUsername[i];
				i++;
				state = 2;
			}
			else if (i == 8) {
				i = 0;
				state = 3;
			}
		break;
		case 3:
			if (i != 8) {
				escriureCharEEPROM(tmpPassword[i],indexNextUser++);
				usuaris[indexNextUserStruct].password[i] = tmpPassword[i];
				i++;
				state = 3;
			}
			else if (i == 8) {
				do_register = 0;
				indexNextUser++;
				if(indexNextUser == 159) indexNextUser = 16;
				indexNextUserStruct++;
				if(indexNextUserStruct == 8) indexNextUserStruct = 0;
				if(numUsuaris != 8) numUsuaris++;
				state = 0;
			}
		break;
		case 4:

		break;
		case 5:
			if (i < numUsuaris) {
				if(j != usuariLoguejat){
				  arrayShowUsers[i] = j;
					i++;
                    j++;
				}else{
                    j++;
                }
				
				if(j > 7) j=0;
				state = 5;
			}
			else if (i == numUsuaris) {
				do_showUsers = 0;
				state = 0;
			}
		break;
	}
    
}

unsigned char UgetScore(char quin){
    return puntuacions[quin].score;
}
signed char UgetTop5(char quin){
    return puntuacions[quin].indexStruct;
}
void UnewScore(char score){
    if(puntuacions[0].score < score){
        puntuacions[0].indexStruct = usuariLoguejat;
    }else{
        if(puntuacions[1].score < score){
            puntuacions[1].indexStruct = usuariLoguejat;
        }else{
            if(puntuacions[2].score < score){
                puntuacions[2].indexStruct = usuariLoguejat;
            }else{
                if(puntuacions[3].score < score){
                    puntuacions[3].indexStruct = usuariLoguejat;
                }else{
                    if(puntuacions[4].score < score){
                        puntuacions[4].indexStruct = usuariLoguejat;
                    }
                }
            }
        }
    }
    //UupdateScoresEEPROM();//guardar a la eeprom tot de raaaaca les scores i indexs
    
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

