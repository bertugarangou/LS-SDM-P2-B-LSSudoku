#include <xc.h>
#include "Joc.h"
#include "Hora.h"
#include "SIO.h"
#include "LcTLCD.h"
#define neg -1
signed char usuariJoc = neg;
signed char direccioJoc = 0;
signed char novaTeclaJOC = neg;
__bit jugantJoc;


void JJuguem(char usuari){
    usuariJoc = usuari;
}
void JnovaTecla(signed char tecla){
    novaTeclaJOC = tecla;
}
void JnovaDireccio(char dir){
    direccioJoc = dir;
}

void JendGame(void){
    jugantJoc = 0;
}

char conversorJoystick(char direc){
    
    if (direc==2) {
        return 'W';
    }
    if (direc==6) {
        return 'D';
    }
    if (direc==8) {
        return 'S';
    }
    if (direc==4) {
        return 'A';
    }
    return 0;
}

void motorJoc(void){
    static char state = 0;

	switch(state) {
		case 0:
			if (usuariJoc != neg) {
				SIOStartGame(usuariJoc);
				jugantJoc = 1;
				state = 2;
			}
		break;
		case 1:
			if (!jugantJoc) {
				usuariJoc = neg;
				HnoJugant();
				SIOendGame();
				
				state--;
			}
			else if (direccioJoc > neg) {
				SIONovaDireccio(conversorJoystick(direccioJoc));
				direccioJoc = neg;
			}
			else if (HNouSegon() && LcLliure()) {
				HClearNouSegon();
				LcGotoXY(0,1);
				LcNewString(HGetTime());
                SIOEnviaHora();
                
			}
			else if (novaTeclaJOC > 0) {
				SIONovaTecla(novaTeclaJOC+48);
				novaTeclaJOC = neg;
			}
		break;
		case 2:
			if (SIOcheckKrebut()) {
				HJugant();
                
				state--;
			}
		break;
	}
}