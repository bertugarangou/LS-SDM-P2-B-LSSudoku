#include <xc.h>
#include "Usuaris.h"

unsigned char numUsuaris;
__bit do_check_exists = 0;
unsigned char indexNextUser;
unsigned char indexNextUserStruct;
__bit do_register = 0;
unsigned char i = 0;
unsigned char j = 0;
signed char usuariLoguejat = -1;
__bit do_showUsers = 0;
__bit do_updateScore = 0;
unsigned char scoreUsr;
unsigned char tmp;

signed char arrayShowUsers[8];

char tmpUsername[9];
char tmpPassword[9];
/*
    for(int i = 0; i < 5; i++){
	if(puntuacions[i].indexUsuari == indexNextUserStruct){
		puntuacions[i].indexUsuari = -1;
	}
}
 
 */
typedef struct{
    char username[9];
    char password[9];
}Usuari;
Usuari usuaris[8];

typedef struct{
    signed char indexStruct;
    unsigned char score;
}Scores;
Scores puntuacions[5];

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
    EECON1bits.WREN = 0;
}
char llegirCharEEPROM(char pos){
    EEADR = pos;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    return EEDATA;
}
void Uinit(){
    /*
     | num totals 8 | index last 8 | user1 8 | pass1 8 | user2 9 | pass2 9 |
     */
    i = 0;
    /*
    escriureCharEEPROM(0,0); //resetejador de la eeprom per usuaris
    escriureCharEEPROM(2,1);
    */
    numUsuaris = llegirCharEEPROM(0);
    
    if(numUsuaris > 8) numUsuaris = 0;


    
    i++;
    indexNextUser = llegirCharEEPROM(1);
    if(indexNextUser == 255) indexNextUser = 2;
    indexNextUserStruct = indexNextUser/18;//estem en un init no importa la cooperativitat rtos
    i++;
    for(j = 0;j<numUsuaris;j++){
        for(unsigned char k = 0; k<9; k++){
            usuaris[j].username[k] = llegirCharEEPROM(i);
            i++;
        }
        for(unsigned char k = 0; k<9;k++){
            usuaris[j].password[k] = llegirCharEEPROM(i);
            i++;
        }
    }
    
    
    
    
    //legir init puntuacions
    puntuacions[0].indexStruct = -1;
    for(i = 0; i<5; i++){
        puntuacions[i].indexStruct = llegirCharEEPROM(200+i+i);
        puntuacions[i].score = llegirCharEEPROM(200+i+i+1);
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
			if (!do_check_exists && !do_register && !do_showUsers && !do_updateScore) {
				i = 0;
				j = 0;
			}
			else if (do_check_exists == 1) {
				usuariLoguejat = -1;
				state++;
			}
			else if (do_register == 1) {
				if(numUsuaris != 8) numUsuaris++;
				escriureCharEEPROM(numUsuaris,0);
				state = 2;
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
			else if (do_updateScore) {
				do_updateScore = 0;
				state = 6;
			}
		break;
		case 1:
			if (i < numUsuaris) {
				if(compareStrings(tmpUsername, usuaris[i].username) == 0){
					usuariLoguejat = i;
				//si son iguals;
				}
				i++;
				
			}
			else {
				do_check_exists = 0;
				state--;
			}
		break;
		case 2:
			if (i < 9) {
				escriureCharEEPROM(tmpUsername[i],indexNextUser);
				indexNextUser++;
				usuaris[indexNextUserStruct].username[i] = tmpUsername[i];
				i++;
				
			}
			else {
				i = 0;
				state++;
			}
		break;
		case 3:
			if (i < 9) {
				escriureCharEEPROM(tmpPassword[i],indexNextUser);
				indexNextUser++;
				usuaris[indexNextUserStruct].password[i] = tmpPassword[i];
				i++;
				
			}
			else if (i > 8) {
				do_register = 0;
				if(indexNextUser > 140) indexNextUser = 2;
				indexNextUserStruct++;
				if(indexNextUserStruct == 8) indexNextUserStruct = 0;
				escriureCharEEPROM(indexNextUser,1);
				i = 0;
				state = 9;
			}
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
				
			}
			else if (i == numUsuaris) {
				do_showUsers = 0;
				state = 0;
			}
		break;
		case 6:
			if (i < 5 && puntuacions[i].indexStruct != -1) {
				i++;
				
			}
			else if (i < 5 && puntuacions[i].indexStruct == -1) {
				state++;
			}
			else if (i > 4) {
				i = 0;
				state = 8;
			}
		break;
		case 7:
			puntuacions[i].score = scoreUsr;
			puntuacions[i].indexStruct = usuariLoguejat;
			i = i+i;
			escriureCharEEPROM(usuariLoguejat,200+i);
			i++;
			escriureCharEEPROM(scoreUsr,200+i);
			state = 0;
		break;
		case 8:
			if (puntuacions[i].score >= scoreUsr && i < 5) {
				i++;
				
			}
			else if (i > 4) {
				state = 0;
			}
			else if (puntuacions[i].score < scoreUsr && i < 5) {
				state--;
			}
		break;
		case 9:
			if (indexNextUser != 2 || i > 4) {
				
				state = 0;
			}
			else if (indexNextUser == 2 && i < 5) {
				if(indexNextUserStruct == 0){
				  tmp = 7;
				}else{
				 tmp = indexNextUserStruct-1;
				}
				if(puntuacions[i].indexStruct == tmp){
				  puntuacions[i].indexStruct = -1;
				}
				i++;
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
void UnewScore(char scoretmp){
    scoreUsr = scoretmp;
    do_updateScore = 1;    
}

__bit miraPassword(){
    return !compareStrings(tmpPassword, usuaris[usuariLoguejat].password);
}