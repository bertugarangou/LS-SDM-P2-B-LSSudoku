#include <xc.h>
#include "SIO.h"
#include "Usuaris.h"

char *userPtr = 0;
signed char usuariActualSIO = -1;
signed char direccioSIO = -1;
unsigned char rebut;
signed char novaTeclaSIO = -1;
__bit jugantSIO = 0;
__bit Krebut = 0;

void SIOendGame(void){
    jugantSIO = 0;
}
__bit SIOcheckKrebut(void){
    return Krebut;
}
void SIONovaDireccio(char num){
    direccioSIO = num;
}

void SIOStartGame(char usuari){
    usuariActualSIO = usuari;
}

signed char SIOUsuariActual(void){
    return usuariActualSIO;
}
void SIONovaTecla(signed char tecla){
    novaTeclaSIO = tecla;
}

void motorSIO(void){
    static char state = 0;

	switch(state) {
		case 0:
			if (usuariActualSIO > -1) {
				userPtr = UgetUserName(usuariActualSIO);
				jugantSIO = 1;
				Krebut = 0;
				state = 1;
			}
		break;
		case 1:
			if (*userPtr == '\0' && TXSTAbits.TRMT) {
				TXREG = '\0';
				novaTeclaSIO = 0;
				LATBbits.LATB3 = 1;
				state = 2;
			}
			else if (*userPtr != '\0' && TXSTAbits.TRMT) {
				TXREG = *userPtr;
				userPtr++;
			}
		break;
		case 2:
			if (RCREG == 'K') {
				Krebut = 1;
				state = 3;
			}
		break;
		case 3:
			if (direccioSIO != -1 && TXSTAbits.TRMT) {
				TXREG = direccioSIO;
				direccioSIO = -1;
				state = 3;
			}
			else if (novaTeclaSIO > '0' && novaTeclaSIO <= '9' && TXSTAbits.TRMT) {
				TXREG = novaTeclaSIO;
				novaTeclaSIO = 0;
				state = 3;
			}
			else if (!jugantSIO && TXSTAbits.TRMT) {
				TXREG = 'F';
				novaTeclaSIO = 0;
				state = 4;
			}
		break;
		case 4:

		break;
	}
}

