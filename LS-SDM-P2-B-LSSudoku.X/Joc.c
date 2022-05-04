#include <xc.h>
#include "Joc.h"
#include "Hora.h"
#include "SIO.h"
#include "LcTLCD.h"

signed char usuariJoc = -1;
signed char novaDireccio = 0;
signed char novaTecla = -1;

void JJuguem(char usuari){
    usuariJoc = usuari;
}
void JnovaTecla(char tecla){
    novaTecla = tecla;
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
                    HJugant();
                    state = 1;
                }
            break;
            case 1:
                if (!SIOJugant()) {
                    usuariJoc = -1;
                    HnoJugant();
                    state = 0;
                }
                else if (novaDireccio > -1) {
                    SIONovaDireccio(novaDireccio);
                    novaDireccio = -1;
                    state = 1;
                }
                else if (HNouSegon() && LcLliure()) {
                    HClearNouSegon();
                    LcGotoXY(0,1);
                    LcNewString(HGetTime());
                    state = 1;
                }
                else if (novaTecla > 0) {
                    SIONovaTecla(novaTecla);
                    novaTecla = -1;
                    state = 1;
                }
            break;
        }
}