#include <xc.h>
#include "SIO.h"
#include "Usuaris.h"
#include "TiTTimer.h"
#include "CtoA.h"
#include "Hora.h"
#include "LcTLCD.h"
#define neg -1
char newLineChar = '\r';

char *userPtr = 0;
signed char usuariActualSIO = neg;
signed char direccioSIO = neg;
signed char novaTeclaSIO = neg;
__bit jugantSIO = 0;
__bit Krebut = 0;
char timerSIO;
unsigned char score;
__bit hi_ha_hora_sio2 = 0;
char horaTX[6];
unsigned char charActu;
unsigned char mask;

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
void SIOEnviaHora(void){
    hi_ha_hora_sio2 = 1;
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
				state++;
			}
		break;
		case 1:
			if (*userPtr == '\0' && TXSTAbits.TRMT) {
				TXREG = '\0';
				novaTeclaSIO = 0;
				state++;
			}
			else if (*userPtr != '\0' && TXSTAbits.TRMT) {
				TXREG = *userPtr;
				userPtr++;
			}
		break;
		case 2:
			if (RCREG == 'K') {
				Krebut = 1;
				state++;
			}
		break;
		case 3:
			if (direccioSIO != -1 && TXSTAbits.TRMT) {
				TXREG = direccioSIO;
				direccioSIO = -1;
				
			}
			else if (novaTeclaSIO > '0' && novaTeclaSIO <= '9' && TXSTAbits.TRMT) {
				TXREG = novaTeclaSIO;
				novaTeclaSIO = 0;
				
			}
			else if ((!jugantSIO && TXSTAbits.TRMT) || PIR1bits.RCIF) {
				LcClear();
				LcNewString("ERRORS:");
				TXREG = 'F';
				novaTeclaSIO = 0;
				state++;
			}
			else if (hi_ha_hora_sio2) {
				horaTX[0] = HGetTime()[0];
                horaTX[1] = HGetTime()[1];
                horaTX[2] = HGetTime()[2];
                horaTX[3] = HGetTime()[3];
                horaTX[4] = HGetTime()[4];
				horaTX[5] = newLineChar;
				mask = 1;
				TiResetTics(timerSIO);
				charActu = 0;
				state = 10;
			}
		break;
		case 4:
			if (PIR1bits.RCIF) {
				score = RCREG;
				UnewScore(score);
				CToAConverteix(score);
				TiResetTics(timerSIO);
				state++;
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
				state++;
			}
		break;
		case 7:
			if (LcLliure()) {
				LcNewString(HGetTime());
				state++;
			}
		break;
		case 6:
			if (PIR1bits.RCIF) {
				LcPutChar(RCREG);
				TiResetTics(timerSIO);
				state--;
			}
		break;
		case 8:
			if (LcLliure()) {
				LcGotoXY(0,1);
				LcNewString("SCORE: ");
				state++;
			}
		break;
		case 9:
			if (LcLliure() && CToAHaAcabat() == 250) {
				LcNewString(CToAobtenir());
				usuariActualSIO = -1;
				state = 0;
			}
		break;
		case 10:
			if (charActu == 6) {
				hi_ha_hora_sio2 = 0;
				state = 3;
			}
			else if (charActu < 6 && TiGetTics(timerSIO) > 0) {
				LATBbits.LATB1 = 0;
				TiResetTics(timerSIO);
                
				state++;
			}
		break;
		case 11:
			if (mask < 1 && TiGetTics(timerSIO) > 0) {
				mask = 1;
				charActu++;
				LATBbits.LATB1 = 1;
				TiResetTics(timerSIO);
				state--;
			}
			else if (TiGetTics(timerSIO) > 0 && mask) {
				if(mask & horaTX[charActu]){
							LATBbits.LATB1 = 1;
						}else{
							LATBbits.LATB1 = 0;
						}
						mask = mask<<1;
				TiResetTics(timerSIO);
				
			}
		break;
	}
}

