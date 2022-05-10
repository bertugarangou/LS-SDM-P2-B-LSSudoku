#include "TeTeclat.h"
#include "TiTTimer.h"
#include "Menu.h"
#include <xc.h>
#include <pic18f4321.h>
#include "Ssms.h"
char zero = 0;
static char tiTeclat=0;

char teclaPremuda[4][3] = {
    {1,2,3},
    {4,5,6},
    {7,8,9},
    {10,0,11}
};

void TeInit(void){
    tiTeclat=TiGetTimer();
}

void TeTeclat(void) {
	static char state = 0;
    static char cols=0;
    static char files = 0;
    files = PORTDbits.RD0 + PORTDbits.RD1*2 + PORTDbits.RD2*3 + PORTDbits.RD3*4;
	switch(state) {
		case 0:
			if (files==zero) {
				Col1=1;
				Col2=zero;
				Col3=zero;
				cols=zero;
				state++;
			}
			else if (files!=zero) {
				TiResetTics(tiTeclat);
				state = 3;
			}
		break;
		case 1:
			if (files==zero) {
				Col1=zero;
				Col2=1;
				Col3=zero;
				cols=1;
				state++;
			}
			else if (files!=zero) {
				TiResetTics(tiTeclat);
				state = 3;
			}
		break;
		case 2:
			if (files==zero) {
				Col1=zero;
				Col2=zero;
				Col3=1;
				cols=2;
				state = zero;
			}
			else if (files!=zero) {
				TiResetTics(tiTeclat);
				state++;
			}
		break;
		case 3:
			if (TiGetTics(tiTeclat)>=TREBOT) {
				state++;
			}
		break;
		case 4:
			if (files==zero) {
				state = 1;
			}
			else if (files!=zero) {
                MsetNovaTecla(teclaPremuda[files-1][cols]);//comentat falta menu
                SsetNovaTecla(teclaPremuda[files-1][cols]);
				state++;
			}
		break;
		case 5:
			if (files==zero) {
				TiResetTics(tiTeclat);
				state++;
			}
		break;
		case 6:
			if (TiGetTics(tiTeclat)>=TREBOT) {
				state++;
			}
		break;
		case 7:
			if (files!=zero) {
				state = 5;
			}
			else{
				state = zero;
			}
		break;
	}
}