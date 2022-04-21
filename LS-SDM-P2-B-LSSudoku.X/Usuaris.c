#include <xc.h>
#include "Usuaris.h"

unsigned char currentUsrIndex = 0;

unsigned char numUsuaris = 0;
char tmpEEPROM[9];
__bit createUser = 0;
unsigned char j = 0;

char UgetNumUsuaris(void){
    return numUsuaris;
}
void UAfegirLletraUsername(char novaLletra){
    tmpEEPROM[j] = novaLletra;
    j++;
}
void UcreateUser(){
    createUser = 1;
}
void motorUsuaris(void) {
    static char state = 0;

	switch(state) {
		case 1:
			if (j < 9) {
				EEADR = currentUsrIndex;
				EEDATA = tmpEEPROM[j];
				EECON1bits.EEPGD = 0;
				EECON1bits.CFGS = 0;
				EECON1bits.WREN = 1;
				INTCONbits.GIE = 0;
				EECON2 = 0x55;
				EECON2 = 0x0AA;
				EECON1bits.WR = 1;
				state = 2;
			}
			else if (j == 9) {
				createUser = 0;
				state = 0;
			}
		break;
		case 2:
			if (EECON1bits.WR == 0) {
				INTCONbits.GIE = 1;
				currentUsrIndex++;
				state = 3;
			}
		break;
		case 3:
			if (tmpEEPROM[j] != '\0') {
				j++;
				state = 1;
			}
			else if (tmpEEPROM[j] == '\0') {
				j++;
				state = 4;
			}
		break;
		case 0:
			if (createUser == 1) {
				state = 1;
                j = 0;
			}
		break;
		case 4:
			if (j < 9) {
			}
			else if (j == 9) {
				j = 0;
				state = 0;
			}
		break;
	}
}