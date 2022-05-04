#include <xc.h>
#include "Menu.h"

char ultimaTeclaAD = 0;
__bit cap1;
__bit cap2;


/**********ATENCIO DRETA I ESQUERRA DE MOMENT NO ES FA SERVIR*/


void JoystickInit(void){
    ADCON0 = 0x01;//00000001 chA //000001_1 chB
    ADCON1 = 0x0D;//00001101
    ADCON2 = 0x09;//00001110
    ADRESH = 127;
}
void JoystickMotor(void) {
	static char state = 0;
    switch(state) {
		case 0:
			if (ADCON0bits.GODONE == 0 && ADRESH < 200 && ADRESH > 50) {
				ADCON0 = 0x07;
                cap1 = 1;
				state++;
			}
			else if (ADCON0bits.GODONE == 0 && ADRESH >= 200) {
				if(ultimaTeclaAD != 2)MNouJoystick(2);
				ultimaTeclaAD = 2;
				ADCON0 = 0x07;
                cap1 = 0;
				state++;
			}
			else if (ADCON0bits.GODONE == 0 && ADRESH <= 50) {
				if(ultimaTeclaAD != 8)MNouJoystick(8);
				ultimaTeclaAD = 8;
				ADCON0 = 0x07;
                cap1 = 0;
				state++;
			}
		break;
		case 1:
			if (ADCON0bits.GODONE == 0 && ADRESH < 200 && ADRESH > 50) {
				ADCON0 = 0x03;
                cap2 = 1;
				state--;
			}
			else if (ADCON0bits.GODONE == 0 && ADRESH >= 200) {
				if(ultimaTeclaAD != 6)MNouJoystick(6);
				ultimaTeclaAD = 6;
				ADCON0 = 0x03;
                cap2 = 0;
				state--;
			}
			else if (ADCON0bits.GODONE == 0 && ADRESH <= 50) {
				if(ultimaTeclaAD != 4)MNouJoystick(4);
				ultimaTeclaAD = 4;
				ADCON0 = 0x03;
                cap2 = 0;
				state--;
			}
		break;
        
	}
    if(cap1 && cap2 && ultimaTeclaAD != 0){
        MNouJoystick(-1);
        ultimaTeclaAD = -1;
    }
}