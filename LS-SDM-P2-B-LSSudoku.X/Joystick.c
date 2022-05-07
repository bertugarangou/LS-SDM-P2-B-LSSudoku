#include <xc.h>
#include "Menu.h"
#define ADCON0valor 0x01
#define ADCON1valor 0x0D
#define ADCON2valor 0x09
#define ADRESHvalor 127

char ultimaTeclaAD = 0;
__bit cap1;
__bit cap2;

void JoystickInit(void){
    ADCON0 = ADCON0valor;//00000001 chA //000001_1 chB
    ADCON1 = ADCON1valor;//00001101
    ADCON2 = ADCON2valor;//00001110
    ADRESH = ADRESHvalor;
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