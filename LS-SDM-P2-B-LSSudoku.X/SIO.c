#include <xc.h>
#include "SIO.h"
#include "Usuaris.h"

char *userPtr = 0;
signed char usuariActualSIO = -1;
signed char direccio = -1;
unsigned char rebut;
signed char novaTeclaSIO = -1;
__bit jugant = 0;


void SIONovaDireccio(char num){
    direccio = num;
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
__bit SIOJugant(void){
    return jugant;
}

void motorSIO(void){
    static char state = 0;

	switch(state) {
		case 0:
			if (usuariActualSIO > -1) {
				userPtr = UgetUserName(usuariActualSIO);
				jugant = 1;
				state = 1;
			}
		break;
		case 1:
			if (*userPtr == '\0' && TXSTAbits.TRMT) {
				TXREG = '\0';
				novaTeclaSIO = 0;
				state = 2;
			}
			else if (*userPtr != '\0' && TXSTAbits.TRMT) {
				TXREG = *userPtr;
				userPtr++;
			}
		break;
		case 2:
			if (RCREG == 'K') {
				state = 3;
			}
		break;
		case 3:
			if (direccio != -1 && TXSTAbits.TRMT) {
				TXREG = direccio;
				direccio = -1;
				state = 3;
			}
			else if (novaTeclaSIO > 0 && novaTeclaSIO < 10 && TXSTAbits.TRMT) {
				TXREG = novaTeclaSIO;
				novaTeclaSIO = 0;
				state = 3;
			}
			else if (novaTeclaSIO == 10 && TXSTAbits.TRMT) {
				TXREG = 'F';
				novaTeclaSIO = 0;
				jugant = 0;
				state = 4;
			}
		break;
		case 4:

		break;
	}
    
    
}

