#ifndef UUsuaris_
#define UUsuaris_

char UgetNumUsuaris(void);
void Uinit(void);
void UcreateUser(void);
void UenviaChar(char c, char pos);
void UenviaPas(char c, char pos);
void UmotorUsers(void);
__bit UcheckExistsNotFinished(void);
void UcheckExists(void);
signed char UcheckExistsGetError(void);
void escriureEEPROM(void);
__bit URegisterEnded(void);
void URegister(void);
char* UgetUserName(char quin);
void UchangeScore(char quin, char score);
void initPuntuacions(void);

void escriure2usuarisStruct(void);
#endif