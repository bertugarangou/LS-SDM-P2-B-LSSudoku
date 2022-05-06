#include <xc.h>
#include "SIO.h"
#include "Usuaris.h"
#include "TiTTimer.h"
#include "CtoA.h"
#include "Hora.h"
#include "LcTLCD.h"

char *userPtr = 0;
signed char usuariActualSIO = -1;
signed char direccioSIO = -1;
signed char novaTeclaSIO = -1;
__bit jugantSIO = 0;
__bit Krebut = 0;
char timerSIO;
char score;

void initSIO(void){
    timerSIO = TiGetTimer();
}

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

signed char SIOHaAcabatPuntuacions(void){
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
			else if ((!jugantSIO && TXSTAbits.TRMT) || PIR1bits.RCIF) {
				LcClear();
				LcNewString("ERRORS:");
				TXREG = 'F';
				novaTeclaSIO = 0;
				state = 4;
			}
		break;
		case 4:
			if (PIR1bits.RCIF) {
                score = RCREG;
                UnewScore(score);
				CToAConverteix(score);
				TiResetTics(timerSIO);
				state = 5;
			}
		break;
		case 5:
			if (TiGetTics(timerSIO) > 2490) {
				LcClear();
				LcNewString("TIME LEFT: ");
				state = 7;
			}
			else if (TiGetTics(timerSIO) < 2490 && PIR1bits.RCIF && LcLliure()) {
				LcGotoXY(0,1);
				LcPutChar(RCREG);
				state = 6;
			}
		break;
		case 7:
			if (LcLliure()) {
				LcNewString(HGetTime());
				state = 8;
			}
		break;
		case 6:
			if (PIR1bits.RCIF) {
				LcPutChar(RCREG);
                TiResetTics(timerSIO);
				state = 5;
			}
		break;
		case 8:
			if (LcLliure()) {
				LcGotoXY(0,1);
				LcNewString("SCORE: ");
				state = 9;
			}
		break;
		case 9:
			if (LcLliure() && CToAHaAcabat() == 250) {
				LcNewString(CToAobtenir());
				usuariActualSIO = -1;
				state = 0;
			}
		break;
	}
}

