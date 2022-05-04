#include <xc.h>
#include "Joc.h"
#include "Hora.h"
#include "SIO.h"
#include "LcTLCD.h"

signed char usuariJoc = -1;
signed char novaDireccioJOC = 0;
signed char novaTeclaJOC = -1;


void JJuguem(char usuari){
    usuariJoc = usuari;
}
void JnovaTecla(char tecla){
    novaTeclaJOC = tecla;
}
void JnovaDireccio(char dir){
    novaDireccioJOC = dir;
}

signed char JUsuari(void){
    return usuariJoc;
}

void motorJoc(void){
    static char state = 0;

	switch(state) {
		case 0:
			if (usuariJoc != -1) {
				SIOStartGame(usuariJoc);
				state = 2;
			}
		break;
		case 1:
			if (!SIOJugant()) {
				usuariJoc = -1;
				HnoJugant();
				state = 0;
			}
			else if (novaDireccioJOC > -1) {
				SIONovaDireccio(novaDireccioJOC);
				novaDireccioJOC = -1;
				state = 1;
			}
			else if (HNouSegon() && LcLliure()) {
				HClearNouSegon();
				LcGotoXY(0,1);
				LcNewString(HGetTime());
				state = 1;
			}
			else if (novaTeclaJOC > 0) {
				SIONovaTecla(novaTeclaJOC);
				novaTeclaJOC = -1;
				state = 1;
			}
		break;
		case 2:
			if (SIOcheckKrebut()) {
				HJugant();
				state = 1;
			}
		break;
	}
}